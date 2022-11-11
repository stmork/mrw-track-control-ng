//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_MRWMESSAGE_H
#define MRW_CAN_MRWMESSAGE_H

#include <QCanBusFrame>

#include <can/commands.h>
#include <model/controller.h>
#include <model/device.h>
#include <util/stringutil.h>
#include <util/constantenumerator.h>

namespace mrw::can
{
	static constexpr uint16_t CAN_SID_MASK         = 0x07ff;
	static constexpr uint16_t CAN_EID_UNITNO_MASK  = 0xffff;
	static constexpr size_t   CAN_SID_SHIFT        = 18;

	/**
	 * This class represents a CAN bus frame in model railway manner. It may
	 * have two states:
	 * 1. As a command to be sent.
	 * 2. As a result from a previous command.
	 */
	class MrwMessage : public mrw::util::String
	{
		static const mrw::util::ConstantEnumerator<Command>       command_map;
		static const mrw::util::ConstantEnumerator<CommandResult> result_map;

		mrw::model::ControllerId   src;
		mrw::model::ControllerId   dst;
		mrw::model::UnitNo         unit_no;

		Command             mrw_command;
		CommandResult       mrw_result;
		size_t              len;
		bool                is_result;
		bool                is_extended;
		uint8_t             info[8];

	public:
		explicit MrwMessage(const Command command);
		explicit MrwMessage(
			const Command                   command,
			const mrw::model::ControllerId  id,
			const mrw::model::UnitNo        no);
		explicit MrwMessage(
			const mrw::model::ControllerId  id,
			const mrw::model::UnitNo        no,
			const Command                   command,
			const CommandResult             code);
		explicit MrwMessage(const QCanBusFrame & frame);

		uint16_t eid() const;
		uint16_t sid() const;
		quint32 id() const;

		inline mrw::model::UnitNo unitNo() const
		{
			return unit_no;
		}

		inline Command command() const
		{
			return mrw_command;
		}

		inline CommandResult result() const
		{
			return mrw_result;
		}

		bool valid() const;

		operator QCanBusFrame() const;

		QString toString() const override;
	};
}

#endif
