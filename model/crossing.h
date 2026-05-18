//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CROSSING_H
#define MRW_MODEL_CROSSING_H

#include <model/device.h>
#include <model/section.h>

namespace mrw::model
{
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

		// Implementations from Device
		const QString    &   name()         const noexcept override;
		bool                 isUnlockable() const noexcept override;
		Controller     *     controller()   const noexcept override;
		virtual mrw::can::MrwMessage configMsg(const unsigned pin) const override;
	};
}

#endif
