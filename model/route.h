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
	 * To build a route several conditions have to meet. First you need the
	 * direction to drive and whether you want to shunt or region overlapping
	 * tour. So the first condition is that shunting don't leave the starting
	 * region.
	 *
	 * The next RailPart is selected by advancing in drive direction. It is
	 * possible that more than one RailPart is possible at switches. The list
	 * is sorted by priority. So you have to use the RailPart::advance()
	 * method to iterate through the possibilities. The selected RailPart has
	 * to be unreserved. The is the next condition.
	 *
	 * All RailPart elements belong to a Section. The third condition is that
	 * only the first section - the starting section - is allowed to be
	 * occupied. All other have to be in state SectionState::FREE.
	 *
	 * Now recurse through the selected RailPart and redo again. The last
	 * condition is the recursion depth specified by the Route::MAX_DEPTH
	 * constant.
	 *
	 * When returning successfully from recursion a further condition tests
	 * the flank protection. This is only required when using a tour route.
	 *
	 * @see RailPart::reserve()
	 * @see RailPart::advance()
	 * @see SectionState
	 */
	class Route : public QObject
	{
		Q_OBJECT

	public:
		typedef std::list<RailPart *> RailTrack;
		typedef std::list<Section *>  SectionTrack;

		static const size_t MAX_DEPTH = 100;

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

		/**
		 * The allocation state. SectionState::SHUNT and SectionState::TOUR
		 * are the only supported allocation states.
		 */
		const SectionState      state = FREE;

		/** The direction to drive. */
		const bool              direction;

		/**
		 * State for automatic unblocking. In SectionState::TOUR mode this flag
		 * must be false for blocks between main signals. In this case the
		 * Section is marked SectionState::PASSED.
		 */
		const bool              auto_unblock = false;

		/**
		 * The collection of flank switches along the route. The content
		 * changes while a train drives through the route.
		 */
		std::vector<mrw::model::RegularSwitch *> flank_switches;

		/**
		 * This method prepares the RailPart track and initializes the
		 * Section track and Signal::Symbol of each Section. Override this
		 * method to additionally add QObject connections or update more
		 * logic.
		 *
		 * @note Do not forget to call this base method in case of overloading!
		 */
		[[nodiscard]]
		virtual bool prepare();

		/**
		 * This method collects all flank protection switches and changes their
		 * states appropriately.
		 *
		 * @see AbstractSwitch::flank()
		 */
		virtual void prepareFlank();

	private:
		Region   *   findSearchRegion(const RailPart * target) const;

		/**
		 * Checks if the flank protection ensured for the actual RailPart. If
		 * no candidates are available everything is fine. Otherwise the flank
		 * protection switches have to be unlocked. Otherwise the flank switches
		 * have to be in correct state.
		 *
		 * @note The actual RailPart may not be any switch. In this case
		 * everything is fine.
		 * @note If the route state is not SectionState::TOUR everything is
		 * also fine.
		 *
		 * @param prev The previous RailPart of the actual RailPart.
		 * @param actual The actual RailPart to be checked.
		 * @return True if flank protection is available.
		 */
		bool         hasFlankProtection(
			const RailPart * prev,
			const RailPart * actual) const;
		bool         qualified(
			const RailPart * rail,
			const Region  *  search_region) const;

		/**
		 * Unreserve and remove all RailPart elements from the track behind the
		 * given actual RailPart.
		 *
		 * @param actual The RailPart which should be the last RailPart of the
		 * routing track.
		 */
		void         unreserveTail(const RailPart * actual);

		template<class T> static bool contains(
			const std::list<T *> & list,
			const T        *       ptr)
		{
			return std::find(list.begin(), list.end(), ptr) != list.end();
		}
	};
}

#endif
