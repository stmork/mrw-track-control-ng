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

	class Crossing : public AssemblyPart, public Device
	{
		Controller       *      signal_controller = nullptr;
		std::vector<Section *>  sections;

	public:
		explicit Crossing(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);
	};
}

#endif
