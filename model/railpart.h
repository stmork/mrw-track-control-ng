//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILPART_H
#define MRW_MODEL_RAILPART_H

#include <regex>
#include <set>

#include <QDomElement>

#include <model/assemblypart.h>

namespace mrw::model
{
	class ModelRailway;

	/**
	 * This base class represents any kind of rail elements which can be:
	 * - simple rail
	 * - switch
	 * - double cross switch.
	 *
	 * The rail parts are connected by connectors which in fact are doubly
	 * linked lists. So when a rail part is connected to another rail part
	 * it is also back linked. Use the valid() method to verify that the
	 * linkage is valid.
	 *
	 * All rail parts follow a counting direction which is normally read
	 * from left to right. You can invert the counting direction which is
	 * normally necessary for switches (@see RailSwitch). If the aIsDir()
	 * method returns true the counting direction is internally inverted.
	 *
	 * Depeding on the
	 * rail part there are multiple connectors in forward and backward
	 * counting direction. For creating a track route you can follow in
	 * counting direction an against the counting direction. To get a list
	 * of appropiate connectors use the advance() method.
	 */
	class RailPart : public AssemblyPart
	{
		static const std::regex path_regex;

	protected:
		/**
		 * The counting direction inversion flag.
		 *
		 * @note It does not invert the counting direction but it rotates the
		 * rail part.
		 */
		const bool              a_in_dir;

		/** All connectors in counting direction. */
		std::set<RailPart *>    rail_forward;

		/** All connectors against counting direction. */
		std::set<RailPart *>    rail_backward;

	public:
		explicit RailPart(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~RailPart() = default;

		/**
		 * This method returns true if the counting direction is inverted
		 * for the connectors.
		 *
		 * @return True if this RailPart uses inverted connectors.
		 */
		inline bool aIsDir() const
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
		virtual bool contains(const RailPart * rail, const bool dir) const;

		/**
		 * This method verifies if all connectors of this rail part are
		 * correctly connected which their counter parts.
		 *
		 * @return True if the connectors are correctly linked.
		 * @see contains()
		 */
		virtual bool valid() const = 0;

	protected:
		/**
		 * This method resolves a connected RailPart from the EMF model.
		 *
		 * @param attr The EMF/XMI attribute name which contains the link
		 * path.
		 * @return The connected RailPart.
		 */
		RailPart *                   resolve(const char * attr) const;

		/**
		 * This method returns all connectors in the given counting
		 * direction.
		 *
		 * @param dir The counting direction to follow.
		 * @return The collection of connected rail parts. The collection
		 * may be empty in case of an end rail.
		 */
		std::set<RailPart *> &       advance(const bool dir);

		/**
		 * This method returns all connectors in the given counting
		 * direction.
		 *
		 * @param dir The counting direction to follow.
		 * @return The collection of connected rail parts. The collection
		 * may be empty in case of an end rail.
		 */
		const std::set<RailPart *> & advance(const bool dir) const;
	};
}

#endif
