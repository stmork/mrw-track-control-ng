//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_MODELRAILWAY_H
#define MRW_MODULE_MODELRAILWAY_H

#include <QDomDocument>
#include <QString>

#include <vector>
#include <unordered_map>

#include <util/stringutil.h>
#include <model/controller.h>
#include <model/region.h>
#include <model/section.h>

namespace mrw::model
{
	class AssemblyPart;

	/**
	 * This class contains the complete data structure referring to a
	 * model railway. It is parsed from a modelrailway EMF/XMI file.
	 */
	class ModelRailway : public mrw::util::String
	{
		friend class Controller;
		friend class Module;
		friend class MultiplexConnection;
		friend class Device;
		friend class SwitchModuleReference;
		friend class Section;
		friend class AssemblyPart;
		friend class RailPart;
		friend class Rail;
		friend class AbstractSwitch;
		friend class RegularSwitch;
		friend class DoubleCrossSwitch;
		friend class Light;
		friend class ProfileLight;
		friend class Signal;

		std::unordered_map<mrw::can::ControllerId, Controller *> controller_map;
		std::unordered_map<mrw::can::ControllerId, Device *>     device_map;

		QDomDocument               xml;
		QString                    name;
		std::vector<Controller *>  controllers;
		std::vector<Region *>      regions;

		size_t                     warnings = 0;
		size_t                     errors   = 0;

	public:
		explicit ModelRailway(const QString & filename);

		// Copy not allowed.
		ModelRailway(const ModelRailway & other) = delete;
		ModelRailway & operator=(const ModelRailway & other) = delete;

		virtual ~ModelRailway();

		/**
		 * This method dumps the parsed EMF/XMI nodes and attributes.
		 */
		void dump() const;

		/**
		 * This method dumps the parsed information about the model railway.
		 */
		void info();

		/**
		 * This method returns the nth Controller element. It is not ID-based
		 * but index based and is used for linking the Controller after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of the Controller.
		 * @return The found Controller instance.
		 */
		inline Controller * controller(const size_t index) const
		{
			return controllers.at(index);
		}

		/**
		 * This method returns a Controller instance based on its ID. This ID
		 * is used to address the Controller using a CAN ID.
		 *
		 * @param id The ID of the Controller.
		 * @return The Controller looked up by its ID.
		 */
		inline Controller * controllerById(const mrw::can::ControllerId id) const
		{
			return controller_map.find(id)->second;
		}

		/**
		 * This method returns a Device instance based on its unit number. This
		 * unit number is used to address the Device using a CAN ID in
		 * combination of the Controller ID.
		 *
		 * @param unit_no The unit number of the Device.
		 * @return The Device looked up by its unit number.
		 */
		inline Device * deviceById(const mrw::can::UnitNo unit_no) const
		{
			return device_map.find(unit_no)->second;
		}

		/**
		 * This method return the amount of containing Controller elements.
		 *
		 * @return The amount of containing Controller elements.
		 */
		inline size_t controllerCount() const
		{
			return controllers.size();
		}

		/**
		 * This method returns the nth Module element. It is not ID-based
		 * but index based and is used for linking the modules after the
		 * model has loaded into memory.
		 *
		 * @param controller_idx The zero based index of the Controller.
		 * @param module_idx The zero based index of connected Module boards.
		 * @return The found Module instance.
		 *
		 * @see controller()
		 */
		inline Module * module(
			const size_t   controller_idx,
			const size_t   module_idx) const
		{
			return controller(controller_idx)->module(module_idx);
		}

		/**
		 * This method returns the nth MultiplexConnection. It is not ID-based
		 * but index based and is used for linking the connections after the
		 * model is loaded into memory.
		 *
		 * @param controller_idx The zero based index of the Controller.
		 * @param connection_idx The zero based index of connected
		 * MultiplexConnection boards.
		 * @return The found MultiplexConnection instance.
		 *
		 * @see controller()
		 */
		inline MultiplexConnection * connection(
			const size_t   controller_idx,
			const size_t   connection_idx) const
		{
			return controller(controller_idx)->connection(connection_idx);
		}

		/**
		 * This method returns the nth Region element. It is not ID-based
		 * but index based and is used for linking the Region after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of the Region.
		 * @return The found Region instance.
		 */
		inline Region * region(const size_t index) const
		{
			return regions.at(index);
		}

		/**
		 * This method return the amount of containing Region elements.
		 *
		 * @return The amount of containing Region elements.
		 */
		inline size_t regionCount() const
		{
			return regions.size();
		}

		/**
		 * This method returns the nth Section element. It is not ID-based
		 * but index based and is used for linking the Section after the
		 * model has loaded into memory.
		 *
		 * @param region_idx The zero based index of the Region.
		 * @param section_idx The zero based index of the Section.
		 * @return The found Section instance.
		 *
		 * @see region()
		 */
		inline Section * section(
			const size_t region_idx,
			const size_t section_idx) const
		{
			return region(region_idx)->section(section_idx);
		}

		/**
		 * This method returns the nth AssemblyPart element. It is not ID-based
		 * but index based and is used for linking the AssemblyPart after the
		 * model has loaded into memory.
		 *
		 * @param region_idx The zero based index of the Region.
		 * @param section_idx The zero based index of the Section.
		 * @param part_idx The zero based index of the AssemblyPart.
		 * @return The found AssemblyPart instance.
		 *
		 * @see section()
		 */
		inline AssemblyPart * assemblyPart(
			const size_t region_idx,
			const size_t section_idx,
			const size_t part_idx) const
		{
			return section(region_idx, section_idx)->assemblyPart(part_idx);
		}

		/**
		 * This method displays a warning message and increases the internal
		 * warning counter.
		 *
		 * @param message The message to log.
		 */
		void warning(const QString & message);

		/**
		 * This method displays an error message and increases the internal
		 * error counter. It also makes this instance invalid.
		 *
		 * @param message The message to log.
		 * @see valid()
		 */
		void error(const QString & message);

		/**
		 * This method validates if all underlying controllers are valid and
		 * no error was reported.
		 *
		 * @return True if the ModelRailway configuration is fine.
		 * @see Controller::valid()
		 * @see error()
		 */
		bool valid() const;

		QString toString() const override;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 */
		template <class T> void parts(std::vector<T *> & result)
		{
			for (Region * sub : regions)
			{
				sub->parts<T>(result);
			}
		}

	private:
		static QString  type(const QDomElement & node);
		static bool     boolean(const QDomElement & node, const char * attr, const bool default_value = false);
		static unsigned value(const QDomElement & node, const char * attr, const unsigned default_value = 0);
		static QString  string(const QDomElement & node, const char * attr);

		void create();
		void add(Controller * controller);
		void add(Device * device);
		void link();

		void dump(const QDomNode & node, const QString & indent = "") const;
	};
}

#endif
