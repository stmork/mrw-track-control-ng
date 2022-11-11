//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "mrwmessage.h"

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::util;

const ConstantEnumerator<Command>       MrwMessage::command_map
{
	CONSTANT(SETLFT),
	CONSTANT(SETRGT),
	CONSTANT(GETDIR),

	CONSTANT(SETRON),
	CONSTANT(SETROF),
	CONSTANT(GETRBS),

	CONSTANT(SETSGN),

	CONSTANT(CFGSWN),
	CONSTANT(CFGSWO),
	CONSTANT(CFGRAI),
	CONSTANT(CFGPF2),
	CONSTANT(CFGPF3),
	CONSTANT(CFGMF2),
	CONSTANT(CFGMF3),
	CONSTANT(CFGPL2),
	CONSTANT(CFGPL3),
	CONSTANT(CFGSL2),
	CONSTANT(CFGML2),
	CONSTANT(CFGML3),
	CONSTANT(CFGML4),
	CONSTANT(CFGLGT),

	CONSTANT(CFGBGN),
	CONSTANT(CFGEND),
	CONSTANT(SET_ID),
	CONSTANT(PING),
	CONSTANT(RESET),
	CONSTANT(GETCFG),
	CONSTANT(GETDVC),
	CONSTANT(FLASH_REQ),
	CONSTANT(FLASH_DATA),
	CONSTANT(FLASH_CHECK),
	CONSTANT(QRYBUF),
	CONSTANT(QRYERR),
	CONSTANT(GETVER),
	CONSTANT(SENSOR)
};

const ConstantEnumerator<CommandResult> MrwMessage::result_map
{
	CONSTANT(MSG_OK),
	CONSTANT(MSG_QUEUE_FULL),
	CONSTANT(MSG_UNKNOWN_CMD),
	CONSTANT(MSG_PENDING),
	CONSTANT(MSG_IGNORED),
	CONSTANT(MSG_QUEUED),
	CONSTANT(MSG_NOT_CONFIGURED_YET),
	CONSTANT(MSG_NO_UNITNO_DEFINED),
	CONSTANT(MSG_UNITTYPE_WRONG),
	CONSTANT(MSG_RESET_PENDING),
	CONSTANT(MSG_UNITNO_MISSING),
	CONSTANT(MSG_UNIT_NOT_FOUND),
	CONSTANT(MSG_NOT_IN_CONFIG_MODE),
	CONSTANT(MSG_BOOTED),
	CONSTANT(MSG_ID_NOT_CHANGED),
	CONSTANT(MSG_CHECKSUM_ERROR),
	CONSTANT(MSG_INFO),
	CONSTANT(MSG_ID_CHANGE_DISABLED),
	CONSTANT(MSG_HARDWARE_MISMATCH),
	CONSTANT(MSG_SWITCH_FAILED),
	CONSTANT(MSG_CONFIG_BUFFER_FULL),

	CONSTANT(MSG_NO_RESULT)
};

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

	if (len >= IDX_COMMAND_SIZE)
	{
		mrw_command =  Command(payload[0] & CMD_MASK);
		is_result   = (payload[0] & CMD_RESPONSE) != 0;

		if ((is_result) && (len >= IDX_RESULT_SIZE))
		{
			dst         = is_extended ? id >> CAN_SID_SHIFT : id & CAN_SID_MASK;
			src         = is_extended ? id &  CAN_EID_UNITNO_MASK : 0;
			mrw_result  = (CommandResult)payload[1];
			unit_no     = payload[2] | (payload[3] << 8);

			std::copy(payload.begin() + IDX_RESULT_SIZE, payload.end(), info);
		}
		else
		{
			dst         = is_extended ? id >> CAN_SID_SHIFT : id & CAN_SID_MASK;
			src         = 0;
			mrw_result  = MSG_NO_RESULT;
			unit_no     = is_extended ? id & CAN_EID_UNITNO_MASK : 0;

			std::copy(payload.begin() + IDX_COMMAND_SIZE, payload.end(), info);
		}
	}
	else
	{
		src         = 0;
		dst         = 0;
		unit_no     = 0;
		is_result   = false;
		mrw_command = CMD_ILLEGAL;
		mrw_result  = MSG_NO_RESULT;
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
		return (dst << CAN_SID_SHIFT) | src;
	}
	else if (is_extended)
	{
		return (dst << CAN_SID_SHIFT) | unit_no;
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
		return is_extended && (len >= IDX_RESULT_SIZE);
	}
	else
	{
		return len >= IDX_COMMAND_SIZE;
	}
}

MrwMessage::operator QCanBusFrame() const
{
	QByteArray array;

	if (is_result)
	{
		array.append(mrw_command | CMD_RESPONSE);
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
	QString appendix = QByteArray((const char *)info, max()).toHex(' ');

	if (is_result)
	{
		return QString::asprintf("ID: %04x:%04x len=%zu # %04x > %-11.11s %-22.22s %s",
				sid(), eid(), len, unit_no,
				command_map.get(mrw_command).toStdString().c_str(),
				result_map.get(mrw_result).toStdString().c_str(),
				appendix.toStdString().c_str());
	}
	else
	{
		return QString::asprintf("ID: %04x:%04x len=%zu #      < %-11.11s %-22.22s %s",
				sid(), eid(), len,
				command_map.get(mrw_command).toStdString().c_str(), "",
				appendix.toStdString().c_str());
	}
}

size_t MrwMessage::max() const
{
	const size_t start = is_result ? IDX_RESULT_SIZE : IDX_COMMAND_SIZE;

	return len < start ? 0 : len - start;
}
