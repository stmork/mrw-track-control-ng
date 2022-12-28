//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <can/mrwmessage.h>

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

const ConstantEnumerator<Response> MrwMessage::response_map
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

	CONSTANT(MSG_NO_RESPONSE)
};

const ConstantEnumerator<SignalState> MrwMessage::signal_map
{
	CONSTANT(SIGNAL_OFF),
	CONSTANT(SIGNAL_HP0),
	CONSTANT(SIGNAL_HP1),
	CONSTANT(SIGNAL_HP2),
	CONSTANT(SIGNAL_VR0),
	CONSTANT(SIGNAL_VR1),
	CONSTANT(SIGNAL_VR2),
	CONSTANT(SIGNAL_SH0),
	CONSTANT(SIGNAL_SH1),
	CONSTANT(SIGNAL_TST)
};

MrwMessage::MrwMessage(const Command command) :
	src(0),
	dst(CAN_BROADCAST_ID),
	unit_no(0),
	msg_command(command),
	msg_response(MSG_NO_RESPONSE)
{
	is_extended = false;
	is_response = false;
	len         = 1;
	bzero(info, sizeof(info));
}

MrwMessage::MrwMessage(
	const Command        command,
	const ControllerId   id,
	const UnitNo         no) :
	src(0),
	dst(id),
	unit_no(no),
	msg_command(command),
	msg_response(MSG_NO_RESPONSE)
{
	len         = 1;
	is_extended = true;
	is_response = false;
	bzero(info, sizeof(info));
}

MrwMessage::MrwMessage(
	const ControllerId  id,
	const UnitNo        no,
	const Command       command,
	const Response code) :
	src(id),
	dst(CAN_GATEWAY_ID),
	unit_no(no),
	msg_command(command),
	msg_response(code)
{
	is_extended = true;
	is_response = true;
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
		msg_command =  Command(payload[0] & CMD_MASK);
		is_response = (payload[0] & CMD_RESPONSE) != 0;

		if (is_response)
		{
			dst         = is_extended ? id >> CAN_SID_SHIFT : id & CAN_SID_MASK;
			src         = is_extended ? id &  CAN_EID_UNITNO_MASK : 0;

			if (len >= IDX_RESPONSE_SIZE)
			{
				msg_response  = (Response)payload[1];
				unit_no       =
					(UnitNo(payload[2]) & 0xff) |
					(UnitNo(payload[3]) << 8);

				std::copy(payload.begin() + IDX_RESPONSE_SIZE, payload.end(), info);
			}
			else
			{
				// Invalid! message response needs at least four bytes.
				msg_response = MSG_NO_RESPONSE;
				unit_no      = NO_UNITNO;
			}
		}
		else
		{
			dst           = is_extended ? id >> CAN_SID_SHIFT : id & CAN_SID_MASK;
			src           = 0;
			msg_response  = MSG_NO_RESPONSE;
			unit_no       = is_extended ? id & CAN_EID_UNITNO_MASK : NO_UNITNO;

			std::copy(payload.begin() + IDX_COMMAND_SIZE, payload.end(), info);
		}
	}
	else
	{
		src          = 0;
		dst          = 0;
		unit_no      = NO_UNITNO;
		is_response  = false;
		msg_command  = CMD_ILLEGAL;
		msg_response = MSG_NO_RESPONSE;
	}
}

uint16_t MrwMessage::eid() const
{
	if (is_response)
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
	if (is_response)
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
	if (is_response)
	{
		return is_extended && (len >= IDX_RESPONSE_SIZE);
	}
	else
	{
		return len >= IDX_COMMAND_SIZE;
	}
}

uint8_t MrwMessage::operator[](const size_t index) const
{
	if (index >= start())
	{
		throw std::out_of_range("Index out of payload bounds!");
	}
	return info[index];
}

MrwMessage::operator QCanBusFrame() const
{
	QByteArray array;

	if (is_response)
	{
		array.append(msg_command | CMD_RESPONSE);
		array.append(msg_response);
		array.append(unit_no & 0xff);
		array.append(unit_no >> 8);
		copy(array);

		QCanBusFrame frame = QCanBusFrame(id(), array);
		frame.setExtendedFrameFormat(is_extended);

		return frame;
	}
	else
	{
		array.append(msg_command);
		copy(array);

		return QCanBusFrame(id(), array);
	}
}

QString MrwMessage::toString() const
{
	QString appendix = QByteArray((const char *)info, max()).toHex(' ');

	if (is_response)
	{
		return QString::asprintf("ID: %04x:%04x len=%zu %c%c # %04x > %-11.11s %-22.22s %s",
				sid(), eid(), len,
				valid() ? 'V' : '-',
				is_extended ? 'X' : 's',
				unit_no,
				command_map.get(msg_command).toStdString().c_str(),
				response_map.get(msg_response).toStdString().c_str(),
				appendix.toStdString().c_str());
	}
	else
	{
		if (command() == SETSGN)
		{
			appendix = signal_map.get((SignalState)info[0]);
		}

		return QString::asprintf("ID: %04x:%04x len=%zu %c%c #      < %-11.11s %-22.22s %s",
				sid(), eid(), len,
				valid() ? 'V' : '-',
				is_extended ? 'X' : 's',
				command_map.get(msg_command).toStdString().c_str(), "",
				appendix.toStdString().c_str());
	}
}

void MrwMessage::append(const uint8_t input)
{
	const size_t s = start();

	if (len < 8)
	{
		info[len++ - s] = input;
	}
	else
	{
		throw std::out_of_range("CAN payload exceeded!");
	}
}

QString MrwMessage::get(const SignalState & state)
{
	return signal_map.get(state);
}

void MrwMessage::copy(QByteArray & array) const
{
	size_t s = start();

	for (size_t i = s; i < len; i++)
	{
		array.append(info[i - s]);
	}
}

size_t MrwMessage::max() const
{
	const size_t s = start();

	return len < s ? 0 : len - s;
}

size_t MrwMessage::start() const
{
	return is_response ? IDX_RESPONSE_SIZE : IDX_COMMAND_SIZE;
}
