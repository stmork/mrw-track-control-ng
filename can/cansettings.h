//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_CANSETTINGS_H
#define MRW_CAN_CANSETTINGS_H

#include <util/settings.h>

namespace mrw::can
{
	/**
	 * This class represents the modelrailway settings for a per hostname
	 * basis. So it is possible to distinguish CAN settings depending on the
	 * host being used. So it is possible to configure each host with different
	 * CAN settings.
	 *
	 * @note The default is to use plugin "socketcan" with interface "can0".
	 *
	 * @see QCanBusDevice
	 */
	class CanSettings : public mrw::util::Settings
	{
		Q_OBJECT

		QString  hostname;
		QString  can_plugin;
		QString  can_iface;

	public:
		CanSettings();

		/**
		 * This method returns the CAN bus plugin name used by the
		 * QCanBusDevice driver. The interface name is configured inside the
		 * &lt;modelname&gt;.conf file under the value
		 * &lt;hostname&gt;/interface. The default is "socketcan".
		 *
		 * @return The CAN plugin name of the QCanBusDevice.
		 * @see QCanBusDevice
		 */
		const QString & plugin() const;

		/**
		 * This method returns the interface of a specific CAN bus plugin. The
		 * interface name is configured inside the &lt;modelname&gt;.conf file
		 * under the value &lt;hostname&gt;/interface. The default is "can0".
		 *
		 * @return The interface name of the CAN plugin.
		 * @see QCanBusDevice
		 */
		const QString & interface() const;

		/**
		 * This method returns a mrw::util::SettingsGroup for a per host
		 * usage.
		 */
		operator mrw::util::SettingsGroup();
	};
}

#endif
