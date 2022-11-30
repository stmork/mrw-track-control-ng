//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QHostInfo>
#include <QDirIterator>
#include <QDebug>

#include <model/signal.h>
#include <model/railpart.h>
#include <ui/basewidget.h>

#include "modelrepository.h"

using namespace mrw::util;
using namespace mrw::model;
using namespace mrw::ui;

const char * ModelRepository::REGION_FILENAME   = "Gruppen.properties";
const char * ModelRepository::SIGNAL_FILENAME   = "Signale.properties";
const char * ModelRepository::RAILPART_FILENAME = "Gleisteile.properties";

ModelRepository::ModelRepository(const QString & model_name) :
	settings_model(model_name),
	home_dir(QDir::homePath())
{
	filename += model_name + ".modelrailway";
	filter << model_name;

	readMaps();
	prepareHost();
	prepareRegions();
	prepareRailParts();
}

ModelRepository::~ModelRepository()
{
	SettingsGroup           group(&settings_model, "positions");
	std::vector<Position *> positions;

	model->parts<Position>(positions);
	for (Position * pos : positions)
	{
		pos->write(settings_model);
	}

	delete model;
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

void ModelRepository::readMaps()
{
	SettingsGroup group(&settings_model, "files");

	model_filename = settings_model.value("filename", home_dir.absolutePath() + "/mrw/" + filename).toString();
	if (!QFile::exists(model_filename))
	{
		lookup();
	}
	else
	{
		signal_filename   = settings_model.value("signals").toString();
		region_filename   = settings_model.value("regions").toString();
		railpart_filename = settings_model.value("railparts").toString();
	}

	// cppcheck-suppress noCopyConstructor
	// cppcheck-suppress noOperatorEq
	model = new ModelRailway(model_filename);

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
	QDirIterator dir_it(QDir::homePath(), filter, QDir::Dirs, QDirIterator::Subdirectories);

	while (dir_it.hasNext())
	{
		QString dir  = dir_it.next();
		QString file = lookupModel(dir);

		if (!file.isEmpty())
		{
			const QStringList & properties = lookupProperties(dir);

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

QString ModelRepository::lookupModel(const QString & base)
{
	QDirIterator dir_it(base, QStringList() << filename, QDir::Files, QDirIterator::Subdirectories);

	while (dir_it.hasNext())
	{
		return dir_it.next();
	}
	return "";
}

QStringList ModelRepository::lookupProperties(const QString & base)
{
	QStringList properties;

	QDirIterator dir_it(base, QStringList() << SIGNAL_FILENAME << REGION_FILENAME << RAILPART_FILENAME, QDir::Files, QDirIterator::Subdirectories);

	while (dir_it.hasNext())
	{
		const QString  & dir = dir_it.next();

		properties << dir;
	}
	return properties;
}

void ModelRepository::setFilenames()
{
	settings_model.setValue("filename", model_filename);
	settings_model.setValue("signals", signal_filename);
	settings_model.setValue("regions", region_filename);
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
	qDebug() << can_plugin << can_iface;
}

void ModelRepository::prepareRegions()
{
	for (size_t r = 0; r < model->regionCount(); r++)
	{
		Region * region = model->region(r);

		try
		{
			QString             prep  = region->key();
			const std::string & key   = prepareKey(prep).toStdString();
			const std::string & value = region_map.lookup(key);

			// Values stored as invert direction flag.
			region->setDirection(value != "true");
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
	SettingsGroup           group(&settings_model, "positions");
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
	SettingsGroup         group(&settings_model, "positions");
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

QString & ModelRepository::prepareKey(QString & input)
{
	return input.replace(" ", "");
}
