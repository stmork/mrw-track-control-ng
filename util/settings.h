//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_SETTINGS_H
#define MRW_UTIL_SETTINGS_H

#include <QSettings>

namespace mrw::util
{
	/**
	 * This class extends the QSettings class by some convenience methods. The
	 * organization is "mrw" and the app_name is given into the constructor.
	 */
	class Settings : public QSettings
	{
		Q_OBJECT

	public:
		explicit Settings(
			const QString & app_name = "model",
			QObject    *    parent   = nullptr);
	};

	/**
	 * This class surrounds some QSettings staff with the
	 * beginGroup()/endGroup() method calls automatically while instantiating
	 * and destroying this class.
	 */
	class SettingsGroup
	{
	public:
		/**
		 * The constructor calls QSettings::beginGroup() for grouped access to
		 * QSettings.
		 *
		 * @param settings The QSettings to use.
		 * @param prefix The group name.
		 */
		explicit SettingsGroup(QSettings * settings, const QString & prefix);

		/**
		  * @brief The destructor calls QSettings::endGroup() to close the
		  * grouped access.
		  */
		~SettingsGroup();

	private:
		QSettings * grouped_settings = nullptr;
	};
}

#endif
