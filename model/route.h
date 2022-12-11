//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ROUTE_H
#define MRW_MODEL_ROUTE_H

#include <list>
#include <unordered_set>

#include <QObject>

#include <model/section.h>
#include <model/railpart.h>

namespace mrw::model
{
	class Route : public QObject
	{
		Q_OBJECT

	public:
		typedef std::list<RailPart *> RailTrack;
		typedef std::list<Section *>  SectionTrack;

		explicit Route(
			const bool           dir,
			const SectionState   wanted_state,
			RailPart      *      first,
			QObject       *      parent = nullptr);
		virtual ~Route();

		inline operator const RailTrack & () const
		{
			return track;
		}

		bool extend(RailPart * target);
		bool extend(RailPart * rail, RailPart * target);
		void clear();
		void dump();
		bool isLastSectionEnded();

	protected:
		RailTrack               track;
		SectionTrack            sections;
		const bool              direction;

		virtual void prepare();

	private:
		bool qualified(RailPart * rail) const;
		bool isLastSectionEnded(
			std::unordered_set<mrw::model::Section *> & neighbours,
			mrw::model::Section  * section,
			mrw::model::RailPart * part);

		const SectionState      state = SHUNTING;
		Section        *        first_section = nullptr;
	};
}

#endif // ROUTE_H
