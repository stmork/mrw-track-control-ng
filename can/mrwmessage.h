//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_MRWMESSAGE_H
#define MRW_CAN_MRWMESSAGE_H

#include <QLoggingCategory>
#include <QCanBusFrame>

#include <can/commands.h>
#include <util/stringutil.h>
#include <util/constantenumerator.h>

namespace mrw::can
{
	Q_DECLARE_LOGGING_CATEGORY(log)

	static constexpr uint16_t CAN_SID_MASK         = 0x07ff;
	static constexpr uint16_t CAN_EID_UNITNO_MASK  = 0xffff;
	static constexpr size_t   CAN_SID_SHIFT        = 18;

	/**
	 * This class represents a CAN bus frame in model railway manner. It may
	 * have two message types:
	 * 1. As a command request to be sent. This may be a broadcast.
	 * 2. As a response from a previous command request.
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
		static const mrw::util::ConstantEnumerator<Command>      command_map;
		static const mrw::util::ConstantEnumerator<Response>     response_map;
		static const mrw::util::ConstantEnumerator<SignalAspect> signal_map;

		enum InfoIdx
		{
			IDX_COMMAND    = 0,
			IDX_COMMAND_SIZE,

			IDX_RESPONSE   = IDX_COMMAND_SIZE,
			IDX_UNITNO_L,
			IDX_UNITNO_H,
			IDX_RESPONSE_SIZE
		};

		ControllerId               src;
		ControllerId               dst;
		UnitNo                     unit_no;

		Command                    msg_command;
		Response                   msg_response;
		size_t                     len;
		bool                       is_response;
		bool                       is_extended;
		uint8_t                    info[8];

	public:
		/**
		 * This constructor creates a command MrwMessage using a basic frame
		 * format to address a specific mrw::model::Controller or using the
		 * broadcast id @c CAN_BROADCAST_ID to address all connected
		 * mrw::model::Controller.
		 *
		 * @param command The Command to send.
		 * @param id The mrw::model::Controller to address by its ID.
		 */
		explicit MrwMessage(
			const Command      command,
			const ControllerId id = CAN_BROADCAST_ID);

		virtual ~MrwMessage() = default;

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
			const Command       command,
			const ControllerId  id,
			const UnitNo        no);

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
			const ControllerId  id,
			const UnitNo        no,
			const Command       command,
			const Response      code);

		/**
		 * This constructor parses a receive CAN frame. Depending on the
		 * contents the resulting MrwMessage may be invalid.
		 *
		 * @param frame The CAN frame to parse
		 * @see valid()
		 */
		explicit MrwMessage(const QCanBusFrame & frame);

		uint16_t eid() const noexcept;
		uint16_t sid() const noexcept;
		quint32  id()  const noexcept;

		/**
		 * This method returns the mrw::model::Device unit number. In case of a
		 * broadcast MrwMessage this unit number is \c 0.
		 *
		 * @return The used mrw::model::Device unit number.
		 */
		inline UnitNo unitNo() const noexcept
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
		inline Command command() const noexcept
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
		inline Response response() const noexcept
		{
			return msg_response;
		}

		/**
		 * This method returns true if the underlying CAN frame is a valid
		 * MrwMessage.
		 *
		 * @return True if this is a valid MrwMessage.
		 */
		bool valid() const noexcept;

		/**
		 * This cast operator converts this MrwMessage into a CAN bus frame
		 * ready for sending.
		 *
		 * @return The converted CAN bus frame.
		 */
		operator QCanBusFrame() const noexcept;

		inline bool isResponse() const noexcept
		{
			return is_response;
		}

		/**
		 * This method returns a payload byte. The payload is related to the
		 * MRW payload which is seven bytes for a command and four bytes for
		 * a response.
		 *
		 * @param index The access index into the payload.
		 * @return The accessed payload byte.
		 * @exception std::out_of_range if the index exceeds the payload range.
		 */
		uint8_t operator[] (const size_t index) const;

		/**
		 * This method appends a payload byte. The payload is related to the
		 * MRW payload which may be seven bytes for a command and four bytes for
		 * a response.
		 *
		 * @param input The byte to append to the payload.
		 * @exception std::out:of:range If the payload exceeds available space.
		 */
		void append(const uint8_t input);

		/**
		 * This method returns the size of the payload.
		 *
		 * @note This is not the CAN frame length!
		 *
		 * @return The payload size in sense of this modelrailway.
		 */
		size_t size() const noexcept;

		/**
		 * This method returns the clear text QString of the SignalState.
		 *
		 * @param state The SignalState enumeration to translate.
		 * @return The QString clear text of the given SignalState enumeration.
		 */
		static QString get(const SignalAspect state) noexcept;

		QString toString() const noexcept override;

	private:
		size_t max() const noexcept;
		size_t start() const noexcept;
		void   copy(QByteArray & array) const noexcept;
	};
}

#endif
