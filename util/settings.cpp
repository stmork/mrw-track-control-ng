//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "settings.h"

using namespace mrw::util;

Settings::Settings(const char * name, QObject * parent) :
	QSettings("mrw", name, parent)
{
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
