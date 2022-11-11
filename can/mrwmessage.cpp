//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "mrwmessage.h"

using namespace mrw::can;
using namespace mrw::model;

MrwMessage::MrwMessage(const Command command) :
	src(0),
	dst(CAN_BROADCAST_ID),
	unit_no(0),
	mrw_command(command),
	mrw_result(MSG_NO_RESULT)
{
	is_extended = false;
	is_result   = false;
	len         = 1;
	bzero(info, sizeof(info));
}

MrwMessage::MrwMessage(
	const Command        command,
	const ControllerId   id,
	const UnitNo         no)
{
	dst         = id;
	src         = 0;
	is_extended = true;
	is_result   = false;
	mrw_command = command;
	mrw_result  = MSG_NO_RESULT;
	unit_no     = no;
	len         = 1;
	bzero(info, sizeof(info));
}

MrwMessage::MrwMessage(
	const ControllerId  id,
	const UnitNo        no,
	const Command       command,
	const CommandResult code)
{
	dst         = CAN_GATEWAY_ID;
	src         = id;
	is_extended = true;
	is_result   = true;
	mrw_command = command;
	mrw_result  = code;
	unit_no     = no;
	len         = 4;
	bzero(info, sizeof(info));
}

MrwMessage::MrwMessage(const QCanBusFrame & frame)
{
	const QByteArray & payload(frame.payload());
	const quint32      id = frame.frameId();

	is_extended = frame.hasExtendedFrameFormat();
	len         = payload.size();
	dst         = is_extended ? id >> 18 : id & 0x7ff;
	src         = is_extended ? id & 0xffff : 0;

	if (len >= 4)
	{
		mrw_command =  Command(payload[0] & CMD_MASK);
		mrw_result  =  (CommandResult)payload[1];
		is_result   = (payload[0] & CMD_RESULT) != 0;
		unit_no     =  payload[2] | (payload[3] << 8);

		std::copy(payload.begin() + 4, payload.end(), info);
	}
}

uint16_t MrwMessage::eid() const
{
	if (is_result)
	{
		return src;
	}
	else if (is_extended)
	{
		return unit_no;
	}
	else
	{
		return 0;
	}
}

uint16_t MrwMessage::sid() const
{
	return dst;
}

quint32 MrwMessage::id() const
{
	if (is_result)
	{
		return (dst << 18) | src;
	}
	else if (is_extended)
	{
		return (dst << 18) | unit_no;
	}
	else
	{
		return dst;
	}
}

bool MrwMessage::valid() const
{
	if (is_result)
	{
		return is_extended && (len >= 4);
	}
	else
	{
		return len >= 1;
	}
}

MrwMessage::operator QCanBusFrame() const
{
	QByteArray array;

	if (is_result)
	{
		array.append(mrw_command | CMD_RESULT);
		array.append(mrw_result);
		array.append(unit_no & 0xff);
		array.append(unit_no >> 8);

		QCanBusFrame frame = QCanBusFrame(id(), array);
		frame.setExtendedFrameFormat(is_extended);

		return frame;
	}
	else
	{
		array.append(mrw_command);

		return QCanBusFrame(id(), array);
	}
}

QString MrwMessage::toString() const
{
	return QString::asprintf("ID: %04x:%04x %02x %02x", eid(), sid(), command(), result());
}
