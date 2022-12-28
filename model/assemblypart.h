//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ASSEMBLYPART_H
#define MRW_MODEL_ASSEMBLYPART_H

#include <regex>

#include <QDomElement>

#include <util/stringutil.h>

namespace mrw::model
{
	class ModelRailway;
	class Section;

	/**
	 * This abstract base class represents all elements contained in a
	 * Section. It may be RailParts and Signals.
	 */
	class AssemblyPart : public mrw::util::String
	{
		friend class Section;

		static const std::regex path_regex;

	protected:
		const QString      part_name;
		ModelRailway   *   part_model   = nullptr;
		Section      *     part_section = nullptr;
		const QDomElement  reference;

	public:
		explicit AssemblyPart(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);
		virtual ~AssemblyPart() = default;

		/**
		 * This method verifies if all connectors of this rail part are
		 * correctly connected which their counter parts.
		 *
		 * @return True if the connectors are correctly linked.
		 * @see contains()
		 */
		virtual bool valid() const = 0;

		/**
		 * This method returns the name of this assembly part.
		 *
		 * @return The assembly part name.
		 */
		inline const QString & partName() const
		{
			return part_name;
		}

		/**
		 * This method returns the Section this AssemblyPart belongs to.
		 *
		 * @return The Section this AssemblyPart belongs to.
		 */
		inline Section * section() const
		{
			return part_section;
		}

	protected:
		/**
		 * This method links all elements needed for the implementation.
		 */
		virtual void link() = 0;

	private:
		static AssemblyPart * resolve(
			const ModelRailway * part_model,
			const QString    &   reference);

		friend class SwitchModule;
		friend class MultiplexConnection;
		friend class RailPart;
	};
}

#endif
