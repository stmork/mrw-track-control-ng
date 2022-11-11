//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_COMMANDS_H
#define MRW_CAN_COMMANDS_H

#include <cinttypes>

namespace mrw::can
{
	enum CommandCategory : uint8_t
	{
		CAT_SWITCH      = 0x00,
		CAT_RAIL        = 0x10,
		CAT_SIGNAL      = 0x20,
		CAT_CONFIG      = 0x30,
		CAT_EXT         = 0x40,
		CAT_MASK        = 0x70,

		CMD_NEED_UNITNO = 0x40
	};

	enum Command : uint8_t
	{
		SETLFT  = CAT_SWITCH | 0x01,
		SETRGT  = CAT_SWITCH | 0x02,
		GETDIR  = CAT_SWITCH | 0x03,

		SETRON  = CAT_RAIL | 0x01,
		SETROF  = CAT_RAIL | 0x02,
		GETRBS  = CAT_RAIL | 0x03,

		SETSGN  = CAT_SIGNAL | 0x01,

		CFGSWN  = CAT_CONFIG | 0x01,
		CFGSWO  = CAT_CONFIG | 0x02,
		CFGRAI  = CAT_CONFIG | 0x03,
		CFGPF2  = CAT_CONFIG | 0x04,
		CFGPF3  = CAT_CONFIG | 0x05,
		CFGMF2  = CAT_CONFIG | 0x06,
		CFGMF3  = CAT_CONFIG | 0x07,
		CFGPL2  = CAT_CONFIG | 0x08,
		CFGPL3  = CAT_CONFIG | 0x09,
		CFGSL2  = CAT_CONFIG | 0x0a,
		CFGML2  = CAT_CONFIG | 0x0b,
		CFGML3  = CAT_CONFIG | 0x0c,
		CFGML4  = CAT_CONFIG | 0x0d,
		CFGLGT  = CAT_CONFIG | 0x0e,

		CFGBGN      = CAT_EXT | 0x01,
		CFGEND      = CAT_EXT | 0x02,
		SET_ID      = CAT_EXT | 0x03,
		PING        = CAT_EXT | 0x04,
		RESET       = CAT_EXT | 0x05,
		GETCFG      = CAT_EXT | 0x06,
		GETDVC      = CAT_EXT | 0x07,
		FLASH_REQ   = CAT_EXT | 0x08,
		FLASH_DATA  = CAT_EXT | 0x09,
		FLASH_CHECK = CAT_EXT | 0x0a,
		QRYBUF      = CAT_EXT | 0x0b,
		QRYERR      = CAT_EXT | 0x0c,
		GETVER      = CAT_EXT | 0x0d,
		SENSOR      = CAT_EXT | 0x0e
	};

	enum CommandResult : uint8_t
	{
		MSG_OK                  =  0,
		MSG_QUEUE_FULL          =  1,
		MSG_UNKNOWN_CMD         =  2,
		MSG_PENDING             =  3,
		MSG_IGNORED             =  4,
		MSG_QUEUED              =  5,
		MSG_NOT_CONFIGURED_YET  =  6,
		MSG_NO_UNITNO_DEFINED   =  7,
		MSG_UNITTYPE_WRONG      =  8,
		MSG_RESET_PENDING       =  9,
		MSG_UNITNO_MISSING      = 10,
		MSG_UNIT_NOT_FOUND      = 11,
		MSG_NOT_IN_CONFIG_MODE  = 12,
		MSG_BOOTED              = 13,
		MSG_ID_NOT_CHANGED      = 14,
		MSG_CHECKSUM_ERROR      = 15,
		MSG_INFO                = 16,
		MSG_ID_CHANGE_DISABLED  = 17,
		MSG_HARDWARE_MISMATCH   = 18,
		MSG_SWITCH_FAILED       = 19,
		MSG_CONFIG_BUFFER_FULL  = 20,

		MSG_MASK        = 0x7f,
		MSG_RESULT      = 0x80,
		NO_RESULT       = 0xff // not really sent
	};

	enum SignalState : uint8_t
	{
		SIGNAL_OFF = 0,
		SIGNAL_HP0,
		SIGNAL_HP1,
		SIGNAL_HP2,
		SIGNAL_VR0,
		SIGNAL_VR1,
		SIGNAL_VR2,
		SIGNAL_SH0,
		SIGNAL_SH1,
		SIGNAL_TST
	};

	enum SwitchState : uint8_t
	{
		SWITCH_STATE_LEFT  = 1,
		SWITCH_STATE_RIGHT = 2
	};

	enum SensorType : uint8_t
	{
		SENSOR_LIGHT = 1,
		SENSOR_TEMP  = 2
	};

	static constexpr uint8_t CMD_MASK      = 0x7f;
	static constexpr uint8_t CMD_ILLEGAL   =    0;
}

#endif
