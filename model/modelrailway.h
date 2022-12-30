//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_MODELRAILWAY_H
#define MRW_MODEL_MODELRAILWAY_H

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
	class Region;

	/**
	 * This functor class provides a hashing function for Device mapping
	 * usage. A unique identifier for a Device consists on a Controller id
	 * and a unit number from a Device.
	 */
	struct DeviceId : public mrw::can::DeviceKey
	{
		DeviceId() = default;
		virtual ~DeviceId() = default;

		/**
		 * This contructor is for conveniance to initialize the
		 * mrw::can::DeviceKey type. It extracts from the given Device the
		 * Controller id and the unit number.
		 *
		 * @param device The Device to extract the Controller id and unit
		 * number from.
		 */
		explicit DeviceId(const Device * device);

		/**
		 * This call operator overload computes the hash of the containing
		 * Device id.
		 *
		 * @param id The id from the Device to compute the hash from.
		 * @return The hashed id.
		 */
		std::size_t operator () (const mrw::can::DeviceKey & id) const;
	};

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
		friend class LightSignal;
		friend class ProfileLight;
		friend class Signal;

		std::unordered_map<mrw::can::ControllerId, Controller *>     controller_map;
		std::unordered_map<mrw::can::DeviceKey, Device *, DeviceId>  device_map;

		QDomDocument               xml_doc;
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
		void xml() const;

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
		Controller * controller(const size_t index) const;

		/**
		 * This method returns a Controller instance based on its ID. This ID
		 * is used to address the Controller using a CAN ID.
		 *
		 * @param id The ID of the Controller.
		 * @return The Controller looked up by its ID.
		 */
		Controller * controllerById(const mrw::can::ControllerId id) const;

		/**
		 * This method returns a Device instance based on its unit number. This
		 * unit number is used to address the Device using a CAN ID in
		 * combination of the Controller ID.
		 *
		 * @param id The Controller id of the Device.
		 * @param unit_no The unit number of the Device.
		 * @return The Device looked up by its unit number.
		 */
		Device * deviceById(
			const mrw::can::ControllerId id,
			const mrw::can::UnitNo       unit_no) const;

		/**
		 * This method returns a Device instance based on its unit number. This
		 * unit number is used to address the Device using a CAN ID in
		 * combination of the Controller ID.
		 *
		 * @param id The unit number of the Device.
		 * @return The Device looked up by its unit number.
		 */
		Device * deviceById(const DeviceId id) const;

		/**
		 * This method return the amount of containing Controller elements.
		 *
		 * @return The amount of containing Controller elements.
		 */
		size_t controllerCount() const;

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
		Module * module(
			const size_t   controller_idx,
			const size_t   module_idx) const;

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
		MultiplexConnection * connection(
			const size_t   controller_idx,
			const size_t   connection_idx) const;

		/**
		 * This method returns the nth Region element. It is not ID-based
		 * but index based and is used for linking the Region after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of the Region.
		 * @return The found Region instance.
		 */
		Region * region(const size_t index) const;

		/**
		 * This method return the amount of containing Region elements.
		 *
		 * @return The amount of containing Region elements.
		 */
		size_t regionCount() const;

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
		Section * section(
			const size_t region_idx,
			const size_t section_idx) const;

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
		AssemblyPart * assemblyPart(
			const size_t region_idx,
			const size_t section_idx,
			const size_t part_idx) const;

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
		template <class T> void parts(
			std::vector<T *>          &         result,
			std::function<bool(const T * part)> guard = [](const T * part)
		{
			(void)part;

			return true;
		})
		{
			for (Region * sub : regions)
			{
				sub->parts<T>(result, guard);
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

		void xml(const QDomNode & node, const QString & indent = "") const;
	};
}

#endif
