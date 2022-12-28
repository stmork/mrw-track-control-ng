//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QHostInfo>
#include <QDirIterator>
#include <QDebug>

#include <model/signal.h>
#include <model/railpart.h>
#include <model/modelrepository.h>

using namespace mrw::util;
using namespace mrw::model;

const char * ModelRepository::MODEL_GROUP       = "model";
const char * ModelRepository::FILE_GROUP        = "files";
const char * ModelRepository::REGION_GROUP      = "regions";
const char * ModelRepository::POSITION_GROUP    = "positions";

const char * ModelRepository::REGION_FILENAME   = "Gruppen.properties";
const char * ModelRepository::SIGNAL_FILENAME   = "Signale.properties";
const char * ModelRepository::RAILPART_FILENAME = "Gleisteile.properties";

ModelRepository::ModelRepository(
	const QString & input,
	const bool      position_using) :
	modelname(input),
	settings_model(modelname),
	home_dir(QDir::homePath()),
	use_positions(position_using)
{
	filename += modelname + ".modelrailway";
	filter << filename;

	if (prepareModel())
	{
		SettingsGroup group(&settings_host, MODEL_GROUP);

		model = new ModelRailway(model_filename);

		readMaps();
		settings_host.setValue("modelname", modelname);
	}
	prepareHost();

	if (use_positions)
	{
		prepareRegions();
		prepareRailParts();
	}
}

ModelRepository::~ModelRepository()
{
	if (use_positions)
	{
		save();
	}

	qInfo("Shutting down model.");
	delete model;
}

const QString & ModelRepository::modelName() const
{
	return modelname;
}

ModelRepository::operator ModelRailway * () const
{
	return model;
}

const QString & ModelRepository::plugin() const
{
	return can_plugin;
}

const QString & ModelRepository::interface() const
{
	return can_iface;
}

void ModelRepository::save()
{
	qInfo("Saving positions.");
	storePositions();

	qInfo("Saving regions.");
	storeRegions();

	settings_model.sync();
}

void ModelRepository::info()
{
	if (dump_result)
	{
		model->info();
	}
}

void ModelRepository::xml()
{
	if (dump_xml)
	{
		model->xml();
	}
}

QString ModelRepository::proposeModelName(const int index)
{
	QStringList   args = QCoreApplication::arguments();
	Settings      settings;
	SettingsGroup group(&settings, MODEL_GROUP);

	return args.size() > index ?
		args[index] :
		settings.value("modelname", "RailwayModel").toString();
}

bool ModelRepository::prepareModel()
{
	SettingsGroup group(&settings_model, FILE_GROUP);

	qDebug().noquote() << "Preparing model" << modelname;

	// First try: Read directly from config by default filename."
	model_filename = settings_model.value("filename", home_dir.absolutePath() + "/mrw/" + filename).toString();
	if (!QFile::exists(model_filename))
	{
		// lookup file.
		const QString found = lookup();

		return !found.isEmpty();
	}
	else
	{
		signal_filename   = settings_model.value("signals").toString();
		region_filename   = settings_model.value("regions").toString();
		railpart_filename = settings_model.value("railparts").toString();
	}
	return true;
}

void ModelRepository::readMaps()
{
	qDebug("Reading position maps...");

	region_map.read(region_filename);
	signal_map.read(signal_filename);
	railpart_map.read(railpart_filename);
}

ModelRepository::operator bool() const
{
	return model != nullptr;
}

QString ModelRepository::lookup()
{
	QDirIterator dir_it(QDir::homePath(), filter, QDir::Files, QDirIterator::Subdirectories);

	qDebug().noquote() << "Looking up model file " << model_filename;
	while (dir_it.hasNext())
	{
		QString file = dir_it.next();

		if (!file.isEmpty())
		{
			const QStringList & properties = lookupProperties(file);

			for (const QString & propfile : properties)
			{
				QFileInfo info(propfile);
				QString   name(info.fileName());

				if (name == REGION_FILENAME)
				{
					region_filename = info.absoluteFilePath();
				}
				else if (name == SIGNAL_FILENAME)
				{
					signal_filename = info.absoluteFilePath();
				}
				else if (name == RAILPART_FILENAME)
				{
					railpart_filename = info.absoluteFilePath();
				}
			}
			model_filename = file;
			setFilenames();

			return file;
		}
	}
	return "";
}

QStringList ModelRepository::lookupProperties(const QString & input)
{
	QDir        base = QDir::cleanPath(input);
	QStringList properties;

	base.cdUp();
	base.cdUp();
	base.cdUp();
	QDirIterator dir_it(base.absolutePath(), QStringList() << SIGNAL_FILENAME << REGION_FILENAME << RAILPART_FILENAME, QDir::Files, QDirIterator::Subdirectories);

	while (dir_it.hasNext())
	{
		const QString  & dir = dir_it.next();

		properties << dir;
	}
	return properties;
}

void ModelRepository::setFilenames()
{
	qDebug("Setting filenames...");
	settings_model.setValue("filename",  model_filename);
	settings_model.setValue("signals",   signal_filename);
	settings_model.setValue("regions",   region_filename);
	settings_model.setValue("railparts", railpart_filename);
	settings_model.sync();
}

void ModelRepository::prepareHost()
{
	QString       hostname = QHostInfo::localHostName();
	SettingsGroup group (&settings_host, hostname);

	can_plugin  = settings_host.value("plugin", "socketcan").toString();
	can_iface   = settings_host.value("interface", "can0").toString();
	dump_result = settings_host.value("dump", dump_result).toBool();
	dump_xml    = settings_host.value("xml",  dump_xml).toBool();

	qDebug().noquote().nospace() << "Using CAN: " << can_plugin << "/" << can_iface;
}

void ModelRepository::prepareRegions()
{
	for (size_t r = 0; r < model->regionCount(); r++)
	{
		Region * region = model->region(r);

		try
		{
			SettingsGroup       group(&settings_model, REGION_GROUP);
			QString             prep  = region->key();
			const std::string & key   = prepareKey(prep).toStdString();
			const std::string & value = region_map.lookup(key);

			// Values stored as invert direction flag.
			region->parse(settings_model, value != "true");
		}
		catch (std::out_of_range & e)
		{
			qWarning() << "No direction found for " << *region << e.what();
		}
		prepareSignals(region);
	}
}

void ModelRepository::prepareRailParts()
{
	SettingsGroup           group(&settings_model, POSITION_GROUP);
	std::vector<RailPart *> parts;

	model->parts<RailPart>(parts);
	for (RailPart * part : parts)
	{
		try
		{
			QString             prep  = part->key();
			const std::string & key   = prepareKey(prep).toStdString();
			const std::string & value = railpart_map.lookup(key);

			part->parse(settings_model, QString(value.c_str()));
		}
		catch (std::out_of_range & e)
		{
			qWarning() << "No coordinate found for " << *part;
		}
	}
}

void ModelRepository::prepareSignals(Region * region)
{
	SettingsGroup         group(&settings_model, POSITION_GROUP);
	QString               region_key = region->key();
	std::vector<Signal *> region_signals;

	region->parts<Signal>(region_signals);

	for (Signal * signal : region_signals)
	{
		try
		{
			QString             prep  = region_key + signal->partName();
			const std::string & key   = prepareKey(prep).toStdString();
			const std::string & value = signal_map.lookup(key);

			signal->parse(settings_model, QString(value.c_str()));
		}
		catch (std::out_of_range & e)
		{
			qWarning() << "No coordinate found for " << *signal;
		}
	}
}

void ModelRepository::storeRegions()
{
	SettingsGroup group(&settings_model, REGION_GROUP);

	for (size_t r = 0; r < model->regionCount(); r++)
	{
		Region * region = model->region(r);

		region->write(settings_model);
	}
}

void ModelRepository::storePositions()
{
	SettingsGroup           group(&settings_model, POSITION_GROUP);
	std::vector<Position *> positions;

	model->parts<Position>(positions);
	for (Position * pos : positions)
	{
		pos->write(settings_model);
	}
}

QString & ModelRepository::prepareKey(QString & input)
{
	return input.replace(" ", "");
}
