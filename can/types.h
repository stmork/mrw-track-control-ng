//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_TYPES_H
#define MRW_CAN_TYPES_H

#include <utility>
#include <cstdint>

namespace mrw::can
{
	/**
	 * This ID correspondends directly with the CAN ID of a basic frame format.
	 * The valid range is from 1-1999. The range from 2000-2046 are reserved
	 * for internal use. The ID 0 is the gateway ID (@c CAN_GATEWAY_ID),
	 * whereas the 2047 is the broadcast ID (@c CAN_BROADCAST_ID). This ID
	 * has to be unique for all CAN controllers.
	 */
	typedef uint16_t ControllerId;

	/**
	 * This is the high priority gateway ID.
	 */
	static constexpr ControllerId CAN_GATEWAY_ID   = 0x000;

	/**
	 * This is the low priority broadcast ID.
	 */
	static constexpr ControllerId CAN_BROADCAST_ID = 0x7ff;

	/**
	 * This is the unit number used in a mrw::model::Device. The valid range
	 * is from 1-65535.
	 */
	typedef uint16_t UnitNo;

	/**
	 * This constant marks an unconfigured unit number in case no unit number
	 * is used.
	 */
	static constexpr UnitNo NO_UNITNO = 0;

	/**
	 * This is the unique identification of a Device inside a complete
	 * ModelRailway.
	 *
	 * @note A UnitNo is not completely unique but its easier to identify a
	 * Device if the UnitNo is although unique.
	 */
	typedef std::pair<ControllerId, UnitNo> DeviceKey;
}

#endif // TYPES_H
