//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MODELREPOSITORY_H
#define MODELREPOSITORY_H

#include <unordered_map>
#include <string>

#include <QDir>

#include <util/properties.h>
#include <util/settings.h>
#include <model/modelrailway.h>
#include <model/region.h>

class ModelRepository
{
	static const char      *     REGION_FILENAME;
	static const char      *     SIGNAL_FILENAME;
	static const char      *     RAILPART_FILENAME;

	mrw::util::Settings          settings_model;
	mrw::util::Settings          settings_host;

	QDir                         home_dir;

	QString                      filename;
	QString                      model_filename;
	QString                      signal_filename;
	QString                      railpart_filename;
	QString                      region_filename;
	QStringList                  filter;

	mrw::model::ModelRailway  *  model = nullptr;
	mrw::util::Properties        region_map;
	mrw::util::Properties        signal_map;
	mrw::util::Properties        railpart_map;

public:
	explicit ModelRepository(const QString & model_name);
	~ModelRepository();

	inline operator mrw::model::ModelRailway * () const
	{
		return model;
	}

	operator bool() const;

private:
	void        readMaps();
	QString     lookup();
	QString     lookupModel(const QString & base);
	QStringList lookupProperties(const QString & base);
	void        setFilenames();
	void        prepareRegions();
	void        prepareRailParts();
	void        prepareSignals(mrw::model::Region * region);

	static QString & prepareKey(QString & input);
};

#endif // MODELREPOSITORY_H
