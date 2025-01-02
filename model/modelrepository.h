//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_MODELREPOSITORY_H
#define MRW_MODEL_MODELREPOSITORY_H

#include <unordered_map>
#include <string>

#include <QDir>

#include <util/properties.h>
#include <util/settings.h>
#include <can/cansettings.h>
#include <model/modelrailway.h>
#include <model/region.h>

namespace mrw::model
{
	/**
	 * The model repository class maintain the several files surrounding the
	 * modelrailway file which contain information about positioning widgets
	 * representing some active parts of the model railway such as signals,
	 * switches and rail sections.
	 *
	 * From the given model name (not filename) the constructor tries to find
	 * the real modelfile. From there the constructor searches for the three
	 * properties files which contain positioning hints ("Gleisteile.property"
	 * and "Signale.property") or view orientation of regions
	 * ("Gruppen.property"). The found model filename is persisted for later
	 * use so further constructions don't need the real modelfile search.
	 *
	 * These properties values are default values which are stored in a
	 * separate QSettings file named by the given modelname. There edited
	 * vales are stored so they have precedence over the properties values.
	 *
	 * If no properties values were found (in fact there exists world wide
	 * only two known triples) the positions were counted up so they are
	 * initially visible and editable.
	 *
	 * @see mrw::model::Position for more information.
	 *
	 * This class also maintain per hostname CAN bus configuration. So it is
	 * possible to make different CAN configuration available on specific hosts.
	 */
	class ModelRepository
	{
		static const char      *     MODEL_GROUP;
		static const char      *     FILE_GROUP;
		static const char      *     REGION_GROUP;
		static const char      *     POSITION_GROUP;

		static const char      *     REGION_FILENAME;
		static const char      *     SIGNAL_FILENAME;
		static const char      *     RAILPART_FILENAME;

		QString                      modelname;
		QString                      filename;
		QString                      model_filename;
		QString                      signal_filename;
		QString                      railpart_filename;
		QString                      region_filename;
		QStringList                  filter;

		mrw::util::Settings          settings_model;
		mrw::can::CanSettings        settings_host;

		QDir                         home_dir;

		// Per host configuration
		bool                         dump_result   = false;
		bool                         dump_xml      = false;
		bool                         use_positions = false;

		ModelRailway        *        model         = nullptr;
		mrw::util::Properties        region_map;
		mrw::util::Properties        signal_map;
		mrw::util::Properties        railpart_map;

	public:
		/**
		 * This method loads filenames and position information from a named
		 * QSettings configuration which is given by the specified modelname.
		 * If the filename of the model railway is not yet found a search in
		 * the home directory is started.
		 *
		 * @param modelname The name of the model configuration to be loaded.
		 * @param use_positions True if the possibly edited positions should
		 * used. Setting this to @c true is only useful when using a GUI.
		 */
		explicit ModelRepository(
			const QString & modelname,
			const bool      use_positions = false);
		ModelRepository() = delete;

		/**
		 * This method frees all loaded resources. In case the auto_save flag
		 * in construction was true the positions were stored using QSettings.
		 *
		 * @see Position
		 * @see QSetting
		 */
		virtual ~ModelRepository();

		/**
		 * This cast operator returns the pointer to the loaded ModelRailway
		 * instance.
		 *
		 * @note The returned pointer may be @c nullptr if the model couldn't
		 * be loaded for any reason.
		 */
		operator ModelRailway * () const;

		/**
		 * This cast operator returns true if the ModelRailway instance has been
		 * successfully loaded.
		 */
		operator bool() const;

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
		 * This method saves the Position data into the model named QSettings.
		 */
		void save();

		/**
		 * This method dumps the parsed model configuration data and states if
		 * this is enabled in the model named QSettings. Inside the
		 * &lt;modelname&gt;.conf file the value &lt;hostname&gt;/dump has to
		 * be set to @c true.
		 *
		 * @see ModelRailway::info();
		 */
		void info();

		/**
		 * This method dumps the XML/XMI data if this is enabled in the model
		 * named QSettings. Inside the &lt;modelname&gt;.conf file the value
		 * &lt;hostname&gt;/xml has to be set to @c true.
		 *
		 * @see ModelRailway::xml();
		 */
		void xml();

		/**
		 * This method returns the configured modelname.
		 *
		 * @return The configured modelname.
		 * @see modelName()
		 */
		const QString & modelName() const;

		/**
		 * This method proposes a modelname from the applications argument list
		 * or from a stored modelname from the QSettings.
		 *
		 * @param index The argument index to use or if not available an entry
		 * from the QSettings.
		 *
		 * @return The proposed modelname.
		 */
		static QString proposeModelName(const int index = 1);

	private:
		bool        prepareModel();
		void        readMaps();
		QString     lookup();
		QStringList lookupProperties(const QString & base);
		void        setFilenames();

		void        prepareHost();
		void        prepareRegions();
		void        prepareRailParts();
		void        prepareSignals(Region * region);

		void        storeRegions();
		void        storePositions();

		static QString & prepareKey(QString & input);
	};
}

#endif
