//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "mrwmessage.h"

using namespace mrw::can;

MrwMessage::MrwMessage(const QCanBusFrame & frame)
{
	const QByteArray & payload(frame.payload());
	const quint32      id = frame.frameId();

	is_extended = frame.hasExtendedFrameFormat();
	len         = payload.size();
	s_id        = is_extended ? id >> 18 : id & 0x7ff;
	e_id        = is_extended ? id & 0xffff : 0;

	if (len >= 4)
	{
		mrw_command =  Command(payload[0]);
		mrw_result  =  (CommandResult)(payload[1] & MSG_MASK);
		is_result   = (payload[1] & MSG_RESULT) != 0;
		unit_no     =  payload[2] | (payload[3] << 8);

		std::copy(payload.begin() + 4, payload.end(), info);
	}
}

bool MrwMessage::valid() const
{
	return is_extended && is_result && (len >= 4);
}

QString MrwMessage::toString() const
{
	return QString::asprintf("ID: %04x:%04x %02x %02x", eid(), sid(), command(), result());
}
