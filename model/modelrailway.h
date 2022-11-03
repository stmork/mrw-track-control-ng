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

#include "util/stringutil.h"
#include <model/controller.h>
#include <model/area.h>
#include <model/section.h>

namespace mrw::model
{
	class AssemblyPart;

	class ModelRailway : public mrw::util::String
	{
		QDomDocument               xml;
		QString                    name;
		std::vector<Controller *>  controllers;
		std::vector<Area *>        areas;
		size_t                     warnings = 0;
		size_t                     errors   = 0;

	public:
		explicit ModelRailway(const QString & filename);
		virtual ~ModelRailway();

		void dump() const;
		void create();
		void link();
		void traverse() const;

		void add(Controller * controller);
		void add(Area * area);

		inline Controller * controller(const ControllerId idx) const
		{
			return controllers.at(idx);
		}

		inline size_t controllerCount() const
		{
			return controllers.size();
		}

		inline Module * module(
			const ControllerId controller_idx,
			const ModuleId     module_idx) const
		{
			return controller(controller_idx)->module(module_idx);
		}

		inline MultiplexConnection * connection(
			const ControllerId controller_idx,
			const ModuleId     connection_idx) const
		{
			return controller(controller_idx)->connection(connection_idx);
		}

		inline Area * area(const size_t index) const
		{
			return areas.at(index);
		}

		inline size_t areaCount() const
		{
			return areas.size();
		}

		inline Section * section(
			const size_t area_idx,
			const UnitNo section_idx) const
		{
			return area(area_idx)->section(section_idx);
		}

		inline AssemblyPart * railPart(
			const size_t area_idx,
			const UnitNo section_idx,
			const UnitNo rail_idx) const
		{
			return section(area_idx, section_idx)->railPart(rail_idx);
		}

		static QString  type(const QDomElement & node);
		static bool     boolean(const QDomElement & node, const char * attr, const bool default_value = false);
		static unsigned value(const QDomElement & node, const char * attr, const unsigned default_value = 0);
		static QString  string(const QDomElement & node, const char * attr);

		void warning(const QString & message);
		void error(const QString & message);

		inline bool isValid() const
		{
			return errors == 0;
		}


		QString toString() const override;

	private:
		void dump(const QDomNode & node, const QString & indent = "") const;
		void traverse(const QDomNode & node, const size_t level = 0) const;
	};
}

#endif
