//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_MRWMESSAGE_H
#define MRW_CAN_MRWMESSAGE_H

#include <QCanBusFrame>

#include <can/commands.h>
#include <model/device.h>
#include <util/stringutil.h>

namespace mrw::can
{
	/**
	 * This class represents a CAN bus frame in model railway manner. It may
	 * have two states:
	 * 1. As a command to be sent.
	 * 2. As a result from a previous command.
	 */
	class MrwMessage : public mrw::util::String
	{
		Command             mrw_command;
		CommandResult       mrw_result;
		mrw::model::UnitNo  unit_no;
		uint16_t            s_id;
		uint16_t            e_id;
		size_t              len;
		bool                is_result;
		bool                is_extended;
		uint8_t             info[4];

	public:
		explicit MrwMessage(const QCanBusFrame & frame);

		inline uint16_t eid() const
		{
			return e_id;
		}

		inline uint16_t sid() const
		{
			return s_id;
		}

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

		QString toString() const override;
	};
}

#endif
