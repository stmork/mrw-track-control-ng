//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILPART_H
#define MRW_MODEL_RAILPART_H

#include <regex>
#include <set>

#include <QDomElement>
#include <QPoint>

#include <model/assemblypart.h>
#include <model/position.h>

namespace mrw::model
{
	class ModelRailway;
	class RailPart;

	/**
	 * This class contains information about a classfified connection to
	 * another RailPart. It is used to select a best fit RailPart when
	 * creating a new track.
	 */
	class RailInfo
	{
		enum class PreferCode : unsigned
		{
			INFERIOR_FLAG    = 2,
			CURVED_FLAG      = 1,

			PREFERRED        = 0,
			PREFERRED_CURVED = CURVED_FLAG,
			STRAIGHT         = INFERIOR_FLAG,
			CURVED           = INFERIOR_FLAG | CURVED_FLAG
		};

		RailPart   *   rail = nullptr;
		PreferCode     code = PreferCode::PREFERRED;

	public:
		explicit RailInfo(
			RailPart  *  rail_part,
			const bool   preferred = true,
			const bool   curved    = false);

		/**
		 * The less operator is used to sort this information instance by
		 * preferred priority.
		 *
		 * @param other The other RailInfo to compare to.
		 * @return True if this instance has a higher priority when selecting
		 * a new track.
		 */
		bool operator<(const RailInfo & other) const;

		/**
		 * This cast operator returns the RailPart of this information
		 * instance.
		 */
		inline operator RailPart * () const
		{
			return rail;
		}
	};

	/**
	 * This base class represents any kind of rail elements which can be:
	 * - simple rail (Rail)
	 * - regular switch (RegularSwitch)
	 * - double cross switch. (DoubleCrossSwitch)
	 *
	 * The rail parts are connected by connectors which in fact are doubly
	 * linked lists. So when a rail part is connected to another rail part
	 * it is also back linked. Use the valid() method to verify that the
	 * linkage is valid.
	 *
	 * All rail parts follow a counting direction which is normally read
	 * from left to right. You can invert the counting direction which is
	 * normally necessary for switches (@see RegularSwitch). If the aIsDir()
	 * method returns true the counting direction is internally inverted.
	 *
	 * Depeding on the rail part there are multiple connectors in forward and
	 * backward counting direction. For creating a track route you can follow
	 * in counting direction an against the counting direction. To get a list
	 * of appropiate connectors use the advance() method.
	 */
	class RailPart : public AssemblyPart, public Position
	{
		static const std::regex path_regex;

		bool                    is_reserved = false;

	protected:
		/**
		 * The counting direction inversion flag.
		 *
		 * @note It does not invert the counting direction but it rotates the
		 * rail part.
		 */
		const bool              a_in_dir;

		/** All connectors in counting direction. */
		std::set<RailInfo>      rail_forward;

		/** All connectors against counting direction. */
		std::set<RailInfo>      rail_backward;

	public:
		explicit RailPart(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);
		virtual ~RailPart() = default;

		/**
		 * This method returns true if the counting direction is inverted
		 * for the connectors.
		 *
		 * @return True if this RailPart uses inverted connectors.
		 */
		[[nodiscard]]
		inline bool aIsDir() const noexcept
		{
			return a_in_dir;
		}

		/**
		 * This method verifies if the given rail part is inside the
		 * corresponding connector set of the given counting direction.
		 *
		 * @param rail The RailPart to verify.
		 * @param dir The counting direction to follow.
		 * @return True if the other RailPart is correctly linked.
		 * @see advance()
		 */
		[[nodiscard]]
		virtual bool contains(const RailPart * rail, const bool dir) const noexcept;

		/**
		 * This method returns all connectors in the given counting
		 * direction.
		 *
		 * @param dir The counting direction to follow.
		 * @return The collection of connected rail parts. The collection
		 * may be empty in case of an end rail.
		 */
		[[nodiscard]]
		std::set<RailInfo> & advance(const bool dir) noexcept;

		/**
		 * This method returns all connectors in the given counting
		 * direction.
		 *
		 * @param dir The counting direction to follow.
		 * @return The collection of connected rail parts. The collection
		 * may be empty in case of an end rail.
		 */
		[[nodiscard]]
		const std::set<RailInfo> & advance(const bool dir) const noexcept;

		/**
		 * This method sets the internal switch state according to the given
		 * connected neighbours. This is useful while computing the track route.
		 *
		 * @note Both pointers need to be non nullptr.
		 *
		 * @param prev The previous RailPart in Route order.
		 * @param succ The successive RailPart in Route order.
		 * @return True if the state may be changed depending on a set
		 * LockState::LOCKED.
		 * @exception std::invalid_argument one of the RailPart pointer is not
		 * a neighbour.
		 */
		[[nodiscard]]
		virtual bool setState(
			const RailPart * prev,
			const RailPart * succ) = 0;

		/**
		 * This method reserves this RailPart for routing information so that
		 * no other router can allocate this RailPart.
		 *
		 * @param input True if this RailPart instance should be reserved,
		 * false if it should be freed.
		 */
		void reserve(const bool input = true) noexcept;

		/**
		 * This method returns the reservation state of this RailPart instance.
		 *
		 * @return The reserveration state of this instance.
		 */
		[[nodiscard]]
		bool reserved() const noexcept;

		/**
		 * This method returns true if the represented RailPart is curved or a
		 * turn out branch of a switch. In case of a Rail the result is static
		 * whereas the result of switches depends on its switching state.
		 *
		 * @return True if the RailPart is a curved or branched.
		 */
		[[nodiscard]]
		virtual bool isCurved() const noexcept = 0;

	protected:
		/**
		 * This method resolves a connected RailPart from the EMF model.
		 *
		 * @param attr The EMF/XMI attribute name which contains the link
		 * path.
		 *
		 * @return The connected RailPart.
		 */
		[[nodiscard]]
		RailPart * resolve(const char * attr) const noexcept;
	};
}

#endif
