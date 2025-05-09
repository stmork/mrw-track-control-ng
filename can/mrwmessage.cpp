//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <can/mrwmessage.h>

using namespace mrw::can;
using namespace mrw::util;

Q_LOGGING_CATEGORY(mrw::can::log, "mrw.can")

const ConstantEnumerator<Command>  MrwMessage::command_map
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
	CONSTANT(Response::MSG_OK),
	CONSTANT(Response::MSG_QUEUE_FULL),
	CONSTANT(Response::MSG_UNKNOWN_CMD),
	CONSTANT(Response::MSG_PENDING),
	CONSTANT(Response::MSG_IGNORED),
	CONSTANT(Response::MSG_QUEUED),
	CONSTANT(Response::MSG_NOT_CONFIGURED_YET),
	CONSTANT(Response::MSG_NO_UNITNO_DEFINED),
	CONSTANT(Response::MSG_UNITTYPE_WRONG),
	CONSTANT(Response::MSG_RESET_PENDING),
	CONSTANT(Response::MSG_UNITNO_MISSING),
	CONSTANT(Response::MSG_UNIT_NOT_FOUND),
	CONSTANT(Response::MSG_NOT_IN_CONFIG_MODE),
	CONSTANT(Response::MSG_BOOTED),
	CONSTANT(Response::MSG_ID_NOT_CHANGED),
	CONSTANT(Response::MSG_CHECKSUM_ERROR),
	CONSTANT(Response::MSG_INFO),
	CONSTANT(Response::MSG_ID_CHANGE_DISABLED),
	CONSTANT(Response::MSG_HARDWARE_MISMATCH),
	CONSTANT(Response::MSG_SWITCH_FAILED),
	CONSTANT(Response::MSG_CONFIG_BUFFER_FULL),

	CONSTANT(Response::MSG_NO_RESPONSE)
};

const ConstantEnumerator<SignalAspect> MrwMessage::signal_map
{
	{ SignalAspect::SIGNAL_OFF, "Off" },
	{ SignalAspect::SIGNAL_HP0, "Hp0" },
	{ SignalAspect::SIGNAL_HP1, "Hp1" },
	{ SignalAspect::SIGNAL_HP2, "Hp2" },
	{ SignalAspect::SIGNAL_VR0, "Vr0" },
	{ SignalAspect::SIGNAL_VR1, "Vr1" },
	{ SignalAspect::SIGNAL_VR2, "Vr2" },
	{ SignalAspect::SIGNAL_SH0, "Sh0" },
	{ SignalAspect::SIGNAL_SH1, "Sh1" },
	CONSTANT(SignalAspect::SIGNAL_TST)
};

MrwMessage::MrwMessage(const Command command, const ControllerId id) :
	src(0),
	dst(id),
	unit_no(0),
	msg_command(command),
	msg_response(Response::MSG_NO_RESPONSE),
	info{}
{
	is_extended = false;
	is_response = false;
	len         = 1;
}

MrwMessage::MrwMessage(
	const Command        command,
	const ControllerId   id,
	const UnitNo         no) :
	src(0),
	dst(id),
	unit_no(no),
	msg_command(command),
	msg_response(Response::MSG_NO_RESPONSE),
	info{}
{
	len         = 1;
	is_extended = true;
	is_response = false;
}

MrwMessage::MrwMessage(
	const ControllerId  id,
	const UnitNo        no,
	const Command       command,
	const Response      code) :
	src(id),
	dst(CAN_GATEWAY_ID),
	unit_no(no),
	msg_command(command),
	msg_response(code),
	info{}
{
	is_extended = true;
	is_response = true;
	len         = 4;
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
				msg_response  = Response(payload[1]);
				unit_no       =
					(UnitNo(payload[2]) & 0xff) |
					(UnitNo(payload[3]) << 8);

				std::copy(payload.begin() + IDX_RESPONSE_SIZE, payload.end(), info);
			}
			else
			{
				// Invalid! message response needs at least four bytes.
				msg_response = Response::MSG_NO_RESPONSE;
				unit_no      = NO_UNITNO;
			}
		}
		else
		{
			dst           = is_extended ? id >> CAN_SID_SHIFT : id & CAN_SID_MASK;
			src           = 0;
			msg_response  = Response::MSG_NO_RESPONSE;
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
		msg_response = Response::MSG_NO_RESPONSE;
	}
}

uint16_t MrwMessage::eid() const noexcept
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
		return NO_UNITNO;
	}
}

uint16_t MrwMessage::sid() const noexcept
{
	return dst;
}

quint32 MrwMessage::id() const noexcept
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

bool MrwMessage::valid() const noexcept
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
	if (index >= max())
	{
		throw std::out_of_range("Index out of payload bounds!");
	}
	return info[index];
}

MrwMessage::operator QCanBusFrame() const noexcept
{
	QByteArray array;

	if (is_response)
	{
		array.append(msg_command | CMD_RESPONSE);
		array.append(std::underlying_type_t<Response>(msg_response));
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

QString MrwMessage::toString() const noexcept
{
	QString appendix = QByteArray(reinterpret_cast<const char *>(info), max()).toHex(' ');

	if (is_response)
	{
		switch (command() & CMD_MASK)
		{
		case GETVER:
			if (len == 8)
			{
				const unsigned major = info[1];
				const unsigned minor = info[2] | (info[3] << 8);

				appendix = QString::asprintf("V%u.%u", major, minor);
			}
			break;

		case QRYBUF:
			if (len >= 7)
			{
				appendix = QString::asprintf("rx:%u tx:%u", info[1], info[2]);
			}
			break;

		case QRYERR:
			if (len >= 8)
			{
				appendix = QString::asprintf("01:%02x 02:%02x 03:%02x", info[1], info[2], info[3]);
			}
			break;
		}

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
			appendix = signal_map.get((SignalAspect)info[0]);
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

size_t MrwMessage::size() const noexcept
{
	return len - start();
}

QString MrwMessage::get(const SignalAspect state) noexcept
{
	return signal_map.get(state);
}

void MrwMessage::copy(QByteArray & array) const noexcept
{
	const size_t s = start();

	for (size_t i = s; i < len; i++)
	{
		array.append(info[i - s]);
	}
}

size_t MrwMessage::max() const noexcept
{
	const size_t s = start();

	return len < s ? 0 : len - s;
}

size_t MrwMessage::start() const noexcept
{
	return is_response ? IDX_RESPONSE_SIZE : IDX_COMMAND_SIZE;
}
