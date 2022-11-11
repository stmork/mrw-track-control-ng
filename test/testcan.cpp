//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCanBusFrame>
#include <QTest>

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

void TestCan::testCanFrame()
{
	QByteArray   array;
	QCanBusFrame frame;

	array.append(GETRBS);
	array.append(MSG_QUEUED | MSG_RESULT);
	array.append(0x33);
	array.append(0x44);
	frame.setFrameId(0x12345678);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(), 0x5678);
	QCOMPARE(message.sid(), 0x048d);
	QCOMPARE(message.command(), GETRBS);
	QCOMPARE(message.result(), MSG_QUEUED);
	QCOMPARE(message.unitNo(), 0x4433);
	QVERIFY(message.toString().size() > 0);
}
