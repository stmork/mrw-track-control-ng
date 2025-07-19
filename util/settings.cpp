//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "settings.h"
#include "log.h"

using namespace mrw::util;

Settings::Settings(const QString & app_name, QObject * parent) :
	QSettings(QSettings::IniFormat, QSettings::UserScope, "mrw", app_name, parent)
{
	qCDebug(log) << fileName() << defaultFormat() << format();
}

SettingsGroup::SettingsGroup(QSettings * settings, const QString & prefix) :
	grouped_settings(settings)
{
	grouped_settings->beginGroup(prefix);
}

SettingsGroup::~SettingsGroup()
{
	grouped_settings->endGroup();
}
