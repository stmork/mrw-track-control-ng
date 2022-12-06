//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ROUTE_H
#define MRW_MODEL_ROUTE_H

#include <list>

#include <QObject>

#include <model/section.h>
#include <model/railpart.h>

namespace mrw::model
{
	class Route : public QObject
	{
		Q_OBJECT

	public:
		typedef std::list<RailPart *> Track;

		explicit Route(
			const bool           dir,
			const SectionState   wanted_state,
			RailPart      *      first,
			QObject       *      parent = nullptr);
		virtual ~Route();

		inline operator const Track & () const
		{
			return track;
		}

		bool extend(RailPart * target);
		bool extend(RailPart * rail, RailPart * target);
		void prepare();
		void clear();

	private:
		bool qualified(RailPart * rail) const;

		const bool              direction;
		const SectionState      state = SHUNTING;
		Track                   track;
		std::list<Section *>    sections;
		Section        *        first_section = nullptr;
	};
}

#endif // ROUTE_H
