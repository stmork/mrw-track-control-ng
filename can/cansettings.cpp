//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QHostInfo>

#include <can/cansettings.h>

using namespace mrw::util;
using namespace mrw::can;

CanSettings::CanSettings() :
	hostname(QHostInfo::localHostName().split(".").at(0).toLower())
{
	SettingsGroup group (this, hostname);

	can_plugin  = value("plugin", "socketcan").toString();
	can_iface   = value("interface", "can0").toString();
}

const QString & CanSettings::plugin() const noexcept
{
	return can_plugin;
}

const QString & CanSettings::interface() const noexcept
{
	return can_iface;
}

CanSettings::operator SettingsGroup()
{
	return SettingsGroup(this, hostname);
}
