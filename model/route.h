//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	class RegularSwitch;

	/**
	 * This class represents a track of interconnected RailPart elements for
	 * driving a train. It may extended at the back end to prolong the track
	 * and shortened if the train as passed or left the RailPart elements.
	 * It also computes the Signal symbol elements. The SignalControllerProxy
	 * computes the definitive SignalState to show. The Route class ensures
	 * that the containing RailPart elements could not allocated by other
	 * Route instances using the RailPart::reserve() method. This class also
	 * maintains the SectionState states of each involved Section.
	 *
	 * @see RailPart::reserve()
	 * @see SectionState
	 */
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

		/**
		 * This cast operator returns the RailPart list for unit testing
		 * purposes.
		 */
		inline operator const RailTrack & () const
		{
			return track;
		}

		/**
		 * This method tries to find a prolonging track to the given RailPart.
		 *
		 * @param target The target RailPart to prolong this Route.
		 * @return True if a track was successfully prolonged.
		 */
		bool append(RailPart * target);

		/**
		 * This method tries to find a RailPart track from the given RailPart
		 * rail to the target parameter. If the search_region is not @c nullptr
		 * The path between both RailPart ends are not allowed to leave the
		 * given Region.
		 *
		 * @param rail The starting point.
		 * @param target The end point.
		 * @param search_region The Region where to search the track. May be
		 * @c nullptr.
		 * @return True if a track was successfully prolonged.
		 */
		bool append(
			RailPart * rail,
			RailPart * target,
			Region  *  search_region);

		/**
		 * This method deallocates all contained Section and RailPart from
		 * this instance.
		 */
		void clear();

		/**
		 * This method returns true if the last RailPart ends so that the
		 * Route cannot be prolonged. It also ensures that the last segment
		 * is turned.
		 *
		 * @return True if the last Rail is ended.
		 */
		bool isLastSectionEnded() const;

		/**
		 * This method tries to find neigbours. If there are no neighbours
		 * the given RailPart ends there.
		 *
		 * @param neighbours The neighbours to collect.
		 * @param part The RailPart to search from.
		 * @param direction The counting direction to follow.
		 */
		static void isLastSectionEnded(
			std::unordered_set<Section *> & neighbours,
			const RailPart         *        part,
			const bool                      direction);

	public slots:
		virtual void dump() const;

	protected:
		/** The RailPart track to follow. */
		RailTrack               track;

		/** The Section track to follow. */
		SectionTrack            sections;

		/** The front end of the Section track. */
		Section        *        first_section   = nullptr;

		/** The tail of the Section track. */
		Section        *        last_section    = nullptr;

		/** The tail of the Rail track. */
		RailPart        *       last_valid_part = nullptr;

		/** The allocation state. */
		const SectionState      state = FREE;

		/** The direction to drive. */
		const bool              direction;

		/**
		 * State for automatic unblocking. This is necessary for
		 * SectionState::TOUR mode.
		 */
		const bool              auto_unblock = false;

		/**
		 * The collection of flank switches along the route.
		 */
		std::vector<mrw::model::RegularSwitch *> flank_switches;

		/**
		 * This method prepares the RailPart track and initializes the
		 * Section track and Signal::Symbol of each Section. Override this
		 * method to additionally add QObject connections or update more
		 * logic.
		 *
		 * @note Do not forget to call this base method in case of overloading!
		 *
		 * @param last_valid_section The last initialized Section.
		 * @param last_valid_part The last initialized RailPart.
		 */
		virtual void prepare();

		/**
		 * This method collects all flank protection switches and changes their
		 * states appropriately.
		 *
		 * @see AbstractSwitch::flank()
		 */
		virtual void prepareFlank();

	private:
		Region   *   findSearchRegion(const RailPart * target) const;
		bool         qualified(
			const RailPart * rail,
			const Region  *  search_region) const;
	};
}

#endif // ROUTE_H
