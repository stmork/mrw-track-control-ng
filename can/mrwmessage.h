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
	 * have two message types:
	 * 1. As a command to be sent.
	 * 2. As a response from a previous command.
	 *
	 * A command may address all mrw::model::Controller instances using the
	 * broadcast ID CAN_BROADCAST_ID. In case a mrw::model::Device is addressed
	 * both is needed: the mrw::model::Controller ID and the mrw::model::Device
	 * unit number.
	 *
	 * @see mrw::model::Controller:id()
	 * @see mrw::model::Device::unitNo()
	 */
	class MrwMessage : public mrw::util::String
	{
		static const mrw::util::ConstantEnumerator<Command>    command_map;
		static const mrw::util::ConstantEnumerator<Response>   response_map;

		enum InfoIdx
		{
			IDX_COMMAND    = 0,
			IDX_COMMAND_SIZE,

			IDX_RESPONSE   = IDX_COMMAND_SIZE,
			IDX_UNITNO_L,
			IDX_UNITNO_H,
			IDX_RESPONSE_SIZE
		};

		mrw::model::ControllerId   src;
		mrw::model::ControllerId   dst;
		mrw::model::UnitNo         unit_no;

		Command                    msg_command;
		Response                   msg_response;
		size_t                     len;
		bool                       is_response;
		bool                       is_extended;
		uint8_t                    info[8];

	public:
		/**
		 * This constructor creates a command MrwMessage using a basic frame
		 * format to address all controllers by broadcast.
		 *
		 * @param command The Command to send.
		 */
		explicit MrwMessage(const Command command);

		/**
		 * This constructor creates a command MrwMessage using an extended
		 * frame format to address a specific mrw::model::Controller and
		 * mrw::model::Device.
		 *
		 * @param command The command to process.
		 * @param id The mrw::model::Controller to address by its ID.
		 * @param no The mrw::model::Device to address by its unit number.
		 */
		explicit MrwMessage(
			const Command                   command,
			const mrw::model::ControllerId  id,
			const mrw::model::UnitNo        no);

		/**
		 * This constructor creates a response MrwMessage using always an
		 * extended frame format.
		 *
		 * @param id The addressed mrw::model::Controller, mostly the CAN
		 * gateway ID.
		 * @param no The sending mrw::model::Device unit number.
		 * @param command The processed Command.
		 * @param code The Response code.
		 */
		explicit MrwMessage(
			const mrw::model::ControllerId  id,
			const mrw::model::UnitNo        no,
			const Command                   command,
			const Response        code);

		/**
		 * This constructor parses a receive CAN frame. Depending on the
		 * contents the resulting MrwMessage may be invalid.
		 *
		 * @param frame The CAN frame to parse
		 * @see valid()
		 */
		explicit MrwMessage(const QCanBusFrame & frame);

		uint16_t eid() const;
		uint16_t sid() const;
		quint32  id() const;

		/**
		 * This method returns the mrw::model::Device unit number. In case of a
		 * broadcast MrwMessage this unit number is \c 0.
		 *
		 * @return The used mrw::model::Device unit number.
		 */
		inline mrw::model::UnitNo unitNo() const
		{
			return unit_no;
		}

		/**
		 * This method returns the command inside the MrwMessage. If it is
		 * a response the response flag (@c Response::CMD_RESPONSE) is
		 * already cleared.
		 *
		 * @return The command inside this MrwMessage
		 * @see Command
		 */
		inline Command command() const
		{
			return msg_command;
		}

		/**
		 * This method returns the MrwMessage response code in case of a
		 * response. In case of a command message this method returns
		 * @c Response::MSG_NO_RESPONSE.
		 *
		 * @return The MrwMessage response code
		 * @see Response
		 */
		inline Response response() const
		{
			return msg_response;
		}

		/**
		 * This method returns true if the underlying CAN frame is a valid
		 * MrwMessage.
		 *
		 * @return True if this is a valid MrwMessage.
		 */
		bool valid() const;

		/**
		 * This cast operator converts this MrwMessage into a CAN bus frame
		 * ready for sending.
		 *
		 * @return The converted CAN bus frame.
		 */
		operator QCanBusFrame() const;

		QString toString() const override;

	private:
		size_t max() const;
	};
}

#endif
