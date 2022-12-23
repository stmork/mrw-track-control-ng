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

		bool append(RailPart * target);
		bool append(
			RailPart * rail,
			RailPart * target,
			Region  *  search_region);
		void clear();
		bool isLastSectionEnded() const;

		static void isLastSectionEnded(
			std::unordered_set<Section *> & neighbours,
			const RailPart         *        part,
			const bool                      direction);

	public slots:
		virtual void dump() const;

	protected:
		RailTrack               track;
		SectionTrack            sections;
		Section        *        last_section  = nullptr;
		Section        *        first_section = nullptr;
		const SectionState      state = FREE;
		const bool              direction;
		const bool              auto_unblock = false;

		virtual void prepare(
			Section  * last_valid_section,
			RailPart * last_valid_part);

	private:
		Region   *   findSearchRegion(
			const RailPart * last_valid_part,
			const RailPart * target) const;
		bool         qualified(
			const RailPart * rail,
			const Region  *  search_region) const;
	};
}

#endif // ROUTE_H
