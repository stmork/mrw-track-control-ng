//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
	array.append(0x33);
	array.append(0x44);
	frame.setFrameId(TEST_ID);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_UNIT_NO);
	QCOMPARE(message.sid(),      TEST_CTRL_ID);
	QCOMPARE(message.command(),  GETRBS);
	QCOMPARE(message.response(), MSG_QUEUED);
	QCOMPARE(message.unitNo(),   0x4433);
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
	QCOMPARE(array[0], PING);
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
	QCOMPARE(array[0], SETLFT);
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
	QCOMPARE(array[0], SETLFT | CMD_RESPONSE);
	QCOMPARE(array[1], MSG_OK);
	QCOMPARE(array[2], 0x78);
	QCOMPARE(array[3], 0x56);
}
