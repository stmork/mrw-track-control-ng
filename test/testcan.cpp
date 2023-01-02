//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCanBusFrame>
#include <QTest>

#include "can/mrwbusservice.h"
#include "can/mrwmessage.h"

#include "testcan.h"

using namespace mrw::test;
using namespace mrw::can;

TestCan::TestCan(QObject * parent) : QObject(parent)
{
}

void TestCan::testEmptyCanFrame()
{
	QCanBusFrame empty;
	MrwMessage   message(empty);

	QVERIFY(!message.valid());
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testInvalidStandardCanFrame()
{
	QByteArray array;

	array.append(PING | CMD_RESPONSE);

	const QCanBusFrame frame(CAN_BROADCAST_ID, array);
	const MrwMessage   message(frame);

	QVERIFY(!message.valid());
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testInvalidExtendedCanFrame()
{
	QByteArray array;

	array.append(PING | CMD_RESPONSE);

	const QCanBusFrame frame(TEST_ID, array);
	const MrwMessage   message(frame);

	QVERIFY(!message.valid());
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testValidService()
{
	MrwBusService service("can0");
	MrwMessage    message(PING);

	QVERIFY(service.valid());
	QVERIFY(service.write(message));
	service.list();
}

void TestCan::testTryValidService()
{
	MrwBusService service("eth0");
	MrwMessage    message(PING);

	QVERIFY(service.valid());
	QVERIFY(service.write(message));
	service.list();
}

void TestCan::testInvalidService()
{
	MrwBusService service("no-interface", "no-plugin");
	MrwMessage    message(PING);

	QVERIFY(!service.valid());
	QVERIFY(!service.write(message));
	service.list();
}

void TestCan::testReceivedResult()
{
	QByteArray   array;
	QCanBusFrame frame;

	array.append(GETRBS | CMD_RESPONSE);
	array.append(MSG_QUEUED);
	array.append(TEST_LSB);
	array.append(TEST_MSB);
	frame.setFrameId(TEST_ID);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_UNIT_NO);
	QCOMPARE(message.sid(),      TEST_CTRL_ID);
	QCOMPARE(message.command(),  GETRBS);
	QCOMPARE(message.response(), MSG_QUEUED);
	QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testReceivedBroadcast()
{
	QByteArray   array;
	QCanBusFrame frame;

	array.append(SENSOR);
	frame.setFrameId(CAN_BROADCAST_ID);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),       0);
	QCOMPARE(message.sid(),       CAN_BROADCAST_ID);
	QCOMPARE(message.command(),   SENSOR);
	QCOMPARE(message.response(),  MSG_NO_RESPONSE);
	QCOMPARE(message.unitNo(),    0);
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testReceivedCommand()
{
	QByteArray   array;
	QCanBusFrame frame;

	array.append(GETRBS);
	frame.setFrameId(TEST_ID);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_UNIT_NO);
	QCOMPARE(message.sid(),      TEST_CTRL_ID);
	QCOMPARE(message.command(),  GETRBS);
	QCOMPARE(message.response(), MSG_NO_RESPONSE);
	QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testBroadcast()
{
	MrwMessage   message(PING);
	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      0);
	QCOMPARE(message.sid(),      CAN_BROADCAST_ID);
	QCOMPARE(message.command(),  PING);
	QCOMPARE(message.response(), MSG_NO_RESPONSE);
	QCOMPARE(message.unitNo(),   0);
	QVERIFY(message.toString().size() > 0);

	QVERIFY(!frame.hasExtendedFrameFormat());
	QCOMPARE(frame.frameId(), CAN_BROADCAST_ID);
	QCOMPARE(array.size(), 1);
	QCOMPARE(Command(array.at(0)), PING);
}

void TestCan::testCommand()
{
	MrwMessage   message(SETLFT, TEST_CTRL_ID, TEST_UNIT_NO);
	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_UNIT_NO);
	QCOMPARE(message.sid(),      TEST_CTRL_ID);
	QCOMPARE(message.id(),       TEST_ID);
	QCOMPARE(message.command(),  SETLFT);
	QCOMPARE(message.response(), MSG_NO_RESPONSE);
	QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
	QVERIFY(message.toString().size() > 0);

	QVERIFY(frame.hasExtendedFrameFormat());
	QCOMPARE(frame.frameId(), TEST_ID);
	QCOMPARE(array.size(), 1);
	QCOMPARE(Command(array.at(0)), SETLFT);
}

void TestCan::testSpecial()
{
	MrwMessage msg_signal(SETSGN);
	MrwMessage msg_getver(TEST_CTRL_ID, TEST_UNIT_NO, GETVER, MSG_OK);
	MrwMessage msg_qrybuf(TEST_CTRL_ID, TEST_UNIT_NO, QRYBUF, MSG_OK);
	MrwMessage msg_qryerr(TEST_CTRL_ID, TEST_UNIT_NO, QRYERR, MSG_OK);

	msg_signal.append(SIGNAL_HP0);
	QVERIFY(msg_signal.toString().contains("SIGNAL_HP0"));

	msg_getver.append(3);
	msg_getver.append(0x11);
	msg_getver.append(0x22);
	msg_getver.append(0x33);
	QVERIFY(msg_getver.toString().contains("V17.13090"));

	msg_qrybuf.append(2);
	msg_qrybuf.append(11);
	msg_qrybuf.append(22);
	QVERIFY(msg_qrybuf.toString().contains("rx:11 tx:22"));

	msg_qryerr.append(3);
	msg_qryerr.append(0xab);
	msg_qryerr.append(0xcd);
	msg_qryerr.append(0xef);
	QVERIFY(msg_qryerr.toString().contains("01:ab 02:cd 03:ef"));
}

void TestCan::testResult()
{
	MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, MSG_OK);
	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_CTRL_ID);
	QCOMPARE(message.sid(),      CAN_GATEWAY_ID);
	QCOMPARE(message.id(),       TEST_CTRL_ID);
	QCOMPARE(message.command(),  SETLFT);
	QCOMPARE(message.response(), MSG_OK);
	QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
	QVERIFY(message.toString().size() > 0);

	QVERIFY(frame.hasExtendedFrameFormat());
	QCOMPARE(frame.frameId(), TEST_CTRL_ID);
	QCOMPARE(array.size(), 4);
	QCOMPARE(Command( array.at(0)), SETLFT | CMD_RESPONSE);
	QCOMPARE(Response(array.at(1)), MSG_OK);
	QCOMPARE(UnitNo(  array.at(2) & 0xff), TEST_LSB);
	QCOMPARE(UnitNo(  array.at(3) & 0xff), TEST_MSB);
}

void TestCan::testRequestPayload()
{
	MrwMessage   message(PING);
	uint8_t      nibble = 1;
	uint8_t      bytes[7];

	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		uint8_t byte  = nibble++ << 4;

		byte |= nibble++;
		bytes[t] = byte;

		QCOMPARE(message.size(), t);
		QVERIFY_EXCEPTION_THROWN(message[t], std::out_of_range);
		message.append(byte);
		for (size_t b = 0; b < t; b++)
		{
			QCOMPARE(message[b], bytes[b]);
		}
	}

	QVERIFY_EXCEPTION_THROWN(message.append(0xff), std::out_of_range);

	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());
	QCOMPARE(array.size(), 8);
	QCOMPARE(array.at(0), PING);
	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		uint8_t byte = (unsigned)array.at(1 + t) & 0xff;

		QCOMPARE(byte, bytes[t]);
	}
}

void TestCan::testResponsePayload()
{
	MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, MSG_OK);
	uint8_t      nibble = 1;
	uint8_t      bytes[4];

	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		uint8_t byte  = nibble++ << 4;

		byte |= nibble++;
		bytes[t] = byte;

		QCOMPARE(message.size(), t);
		QVERIFY_EXCEPTION_THROWN(message[t], std::out_of_range);
		message.append(byte);
		for (size_t b = 0; b < t; b++)
		{
			QCOMPARE(message[b], bytes[b]);
		}
	}

	QVERIFY_EXCEPTION_THROWN(message.append(0xff), std::out_of_range);

	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());
	uint8_t      command = (unsigned)array.at(0) & 0xff;

	QCOMPARE(array.size(), 8);
	QCOMPARE(command, SETLFT | CMD_RESPONSE);
	QCOMPARE(array.at(1), MSG_OK);
	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		uint8_t byte = (unsigned)array.at(4 + t) & 0xff;

		QCOMPARE(byte, bytes[t]);
	}
}

void TestCan::testCopyRequest()
{
	const size_t start = 1;

	for (size_t i = start; i < 8; i++)
	{
		MrwMessage   message(PING);

		for (unsigned v = start; v <= i; v++)
		{
			message.append(0x11 * v);
		}

		QCanBusFrame frame(message);
		QByteArray   array(frame.payload());

		QVERIFY(message.valid());
		QCOMPARE(message.eid(),      0);
		QCOMPARE(message.sid(),      CAN_BROADCAST_ID);
		QCOMPARE(message.command(),  PING);
		QCOMPARE(message.response(), MSG_NO_RESPONSE);
		QCOMPARE(message.unitNo(),   0);
		QVERIFY(message.toString().size() > 0);

		QVERIFY(!frame.hasExtendedFrameFormat());
		QCOMPARE(frame.frameId(), CAN_BROADCAST_ID);
		QCOMPARE((size_t)array.size(), i + 1);
		QCOMPARE(Command(array.at(0)), PING);

		for (size_t v = start; v <= i; v++)
		{
			QCOMPARE((size_t)array.at(v), 0x11 * v);
		}
	}
}

void TestCan::testCopyResponse()
{
	const size_t start = 4;

	for (size_t i = start; i < 8; i++)
	{
		MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, MSG_QUEUED);

		for (unsigned v = start; v <= i; v++)
		{
			message.append(0x11 * v);
		}

		QCanBusFrame frame(message);
		QByteArray   array(frame.payload());

		QVERIFY(message.valid());
		QCOMPARE(message.eid(),      TEST_CTRL_ID);
		QCOMPARE(message.sid(),      CAN_GATEWAY_ID);
		QCOMPARE(message.command(),  SETLFT);
		QCOMPARE(message.response(), MSG_QUEUED);
		QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
		QVERIFY(message.toString().size() > 0);

		QVERIFY(frame.hasExtendedFrameFormat());
		QCOMPARE(frame.frameId(), TEST_CTRL_ID);
		QCOMPARE((size_t)array.size(), i + 1);
		QCOMPARE(Command( array.at(0)), SETLFT | CMD_RESPONSE);
		QCOMPARE(Response(array.at(1)), MSG_QUEUED);

		for (size_t v = start; v <= i; v++)
		{
			QCOMPARE((size_t)array.at(v), 0x11 * v);
		}
	}
}
