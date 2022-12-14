//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QHostInfo>

#include <can/cansettings.h>

using namespace mrw::util;
using namespace mrw::can;

CanSettings::CanSettings() : hostname(QHostInfo::localHostName())
{
	SettingsGroup group (this, hostname);

	can_plugin  = value("plugin", "socketcan").toString();
	can_iface   = value("interface", "can0").toString();
}

const QString & CanSettings::plugin() const
{
	return can_plugin;
}

const QString & CanSettings::interface() const
{
	return can_iface;
}

mrw::can::CanSettings::operator mrw::util::SettingsGroup()
{
	return SettingsGroup(this, hostname);
}
