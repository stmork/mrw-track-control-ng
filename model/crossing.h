//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CROSSING_H
#define MRW_MODEL_CROSSING_H

#include <vector>

#include <model/device.h>
#include <model/section.h>

namespace mrw::model
{
	typedef uint16_t CrossingId;

	class MultiplexConnection;

	class Crossing : public Device
	{
		Controller       *      crx_controller = nullptr;
		std::vector<Section *>  sections;

	protected:
		const QString           crx_name;  ///< The light name.

	public:
		explicit Crossing(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
		virtual ~Crossing() = default;

		void add(Section * section);

		/**
		 * This method verifies if the amount of connected sections are
		 * correctly.
		 *
		 * @return True if the configuration of this Crossing is correct.
		 */
		bool valid() const;

		/**
		 * This method return true if at least one of the connected sections is
		 * not in free state. This may include occupation in case no route is
		 * related to this section.
		 *
		 * @return True if any of the connected sections is not in
		 * SectionState::FREE.
		 */
		bool isUsed() const;

		// Implementations from Device
		const QString    &   name()         const noexcept override;
		bool                 isUnlockable() const noexcept override;
		Controller     *     controller()   const noexcept override;
		virtual mrw::can::MrwMessage configMsg(const unsigned pin) const override;
	};
}

#endif
