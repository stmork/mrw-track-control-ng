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
		SETLFT  = CAT_SWITCH | 0x01, ///< Turn switch left.
		SETRGT  = CAT_SWITCH | 0x02, ///< Turn switch right.
		GETDIR  = CAT_SWITCH | 0x03, ///< Request turning state of a switch.

		SETRON  = CAT_RAIL | 0x01,   ///< Enable power to section.
		SETROF  = CAT_RAIL | 0x02,   ///< Disable power to section.
		GETRBS  = CAT_RAIL | 0x03,   ///< Request occupation state of a section.

		SETSGN  = CAT_SIGNAL | 0x01, ///< Set signal to SignalAspect.

		CFGSWN  = CAT_CONFIG | 0x01, ///< Configure switch with automatic cutoff.
		CFGSWO  = CAT_CONFIG | 0x02, ///< Configure switch without automatic cutoff.
		CFGRAI  = CAT_CONFIG | 0x03, ///< Configure section
		CFGPF2  = CAT_CONFIG | 0x04, ///< Configure distant FormSignal with two actuators.
		CFGPF3  = CAT_CONFIG | 0x05, ///< Configure distant FormSignal three two actuators.
		CFGMF2  = CAT_CONFIG | 0x06, ///< Configure main FormSignal with two actuators.
		CFGMF3  = CAT_CONFIG | 0x07, ///< Configure main FormSignal with three actuators.
		CFGPL2  = CAT_CONFIG | 0x08, ///< Configure distant LightSignal with two SignalAspect variants.
		CFGPL3  = CAT_CONFIG | 0x09, ///< Configure distant LightSignal with three SignalAspect variants.
		CFGSL2  = CAT_CONFIG | 0x0a, ///< Configure shunting LightSignal.
		CFGML2  = CAT_CONFIG | 0x0b, ///< Configure main block LightSignal.
		CFGML3  = CAT_CONFIG | 0x0c, ///< Configure main incoming LightSignal.
		CFGML4  = CAT_CONFIG | 0x0d, ///< Configure main outgoing LightSignal.
		CFGLGT  = CAT_CONFIG | 0x0e, ///< Configure ProfileLight.

		CFGBGN      = CAT_EXT | 0x01, ///< Start configuration mode.
		CFGEND      = CAT_EXT | 0x02, ///< Finish configuration mode.
		SET_ID      = CAT_EXT | 0x03, ///< Set ID and clear configuration space.
		PING        = CAT_EXT | 0x04, ///< Request response.
		RESET       = CAT_EXT | 0x05, ///< Request resetting of the controller.
		GETCFG      = CAT_EXT | 0x06, ///< Get amount of configured devices.
		GETDVC      = CAT_EXT | 0x07, ///< Get device configuration.
		FLASH_REQ   = CAT_EXT | 0x08, ///< Request firmware update in CAN bootloader.
		FLASH_DATA  = CAT_EXT | 0x09, ///< Send firmware data.
		FLASH_CHECK = CAT_EXT | 0x0a, ///< Verify firmware update checksum.
		QRYBUF      = CAT_EXT | 0x0b, ///< Request buffer state.
		QRYERR      = CAT_EXT | 0x0c, ///< Request error state.
		GETVER      = CAT_EXT | 0x0d, ///< Request firmware version.
		SENSOR      = CAT_EXT | 0x0e, ///< Transmit sensor data.

		CMD_MASK      = 0x7f,                  ///< The command mask.
		CMD_RESPONSE  = UINT8_MAX - CMD_MASK,  ///< The command response flag.
		CMD_ILLEGAL   =    0                   ///< This is not a command.
	};

	enum Response : uint8_t
	{
		/** The command succeeded successfully.*/
		MSG_OK                  =  0,

		/** The message queue is full. */
		MSG_QUEUE_FULL          =  1,

		/** The controller does not know the command. */
		MSG_UNKNOWN_CMD         =  2,

		/** The processing of a command needs some time. */
		MSG_PENDING             =  3,

		/** The command was ignored. */
		MSG_IGNORED             =  4,

		/** The command processing is queued. */
		MSG_QUEUED              =  5,

		/** The controller has no configuration. */
		MSG_NOT_CONFIGURED_YET  =  6,

		/** The command has no correct unit number. */
		MSG_NO_UNITNO_DEFINED   =  7,

		/** The addressed unit number is unknown. */
		MSG_UNITTYPE_WRONG      =  8,

		/** The RESET command was issued and the reset is ongoing. */
		MSG_RESET_PENDING       =  9,

		/**
		 * The command addressed a controller but no device because of a used
		 * basic frame format.
		 */
		MSG_UNITNO_MISSING      = 10,

		/** The given unit number was not configured inside this controller. */
		MSG_UNIT_NOT_FOUND      = 11,

		/** A config command was received outside configuration mode. */
		MSG_NOT_IN_CONFIG_MODE  = 12,

		/** The controller has booted successfully. */
		MSG_BOOTED              = 13,

		/** Requested ID change leads to same ID. */
		MSG_ID_NOT_CHANGED      = 14,

		/** The firmware update failed with a wrong checksum. */
		MSG_CHECKSUM_ERROR      = 15,

		/** The result contains only some information. */
		MSG_INFO                = 16,

		/** The controller ID was not changed after a SET_ID command. */
		MSG_ID_CHANGE_DISABLED  = 17,

		/** The firmware update has a wrong hardware ID. */
		MSG_HARDWARE_MISMATCH   = 18,

		/** The switch turning failed. */
		MSG_SWITCH_FAILED       = 19,

		/** No space left in configuration memory. */
		MSG_CONFIG_BUFFER_FULL  = 20,

		/** Illegal response code. */
		MSG_NO_RESPONSE           = UINT8_MAX // not really sent
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

	/**
	 * This is the high priority gateway ID.
	 */
	static constexpr uint16_t CAN_GATEWAY_ID   = 0x000;

	/**
	 * This is the low priority broadcast ID.
	 */
	static constexpr uint16_t CAN_BROADCAST_ID = 0x7ff;
}

#endif
