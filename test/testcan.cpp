//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QCanBusFrame>
#include <QTest>
#include <QSignalSpy>
#include <QList>

#include "can/mrwbusservice.h"
#include "can/mrwmessage.h"

#include "testcan.h"

using namespace mrw::test;
using namespace mrw::can;

#if (QT_VERSION < QT_VERSION_CHECK(6, 3, 0))
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_EXCEPTION_THROWN(condition, exception);
#else
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_THROWS_EXCEPTION(exception, condition);
#endif

/*************************************************************************
**                                                                      **
**       Test implementation of MrwBusService                           **
**                                                                      **
*************************************************************************/

class ManualCanService : public MrwBusService
{
	QList<MrwMessage> messages;

public:
	explicit ManualCanService(
		const bool   auto_connect = false,
		const char * iface        = "vcan0") :
		MrwBusService(iface, "socketcan", nullptr, auto_connect)
	{
		can_device->setConfigurationParameter(QCanBusDevice::ReceiveOwnKey, true);
	}

	void connect()
	{
		can_device->connectDevice();
	}

	void disconnect()
	{
		can_device->disconnectDevice();
	}

	void process(const MrwMessage & message) override
	{
		MrwBusService::process(message);
		messages.append(message);
	}

	int counted() const
	{
		return messages.size();
	}

	const QList<MrwMessage> & list() const
	{
		return messages;
	}
};

/*************************************************************************
**                                                                      **
**       Test class                                                     **
**                                                                      **
*************************************************************************/

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

	array.append(PING_RESPONSE);

	const QCanBusFrame frame(CAN_BROADCAST_ID, array);
	const MrwMessage   message(frame);

	QVERIFY(!message.valid());
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testInvalidExtendedCanFrame()
{
	QByteArray array;

	array.append(PING_RESPONSE);

	const QCanBusFrame frame(TEST_ID, array);
	const MrwMessage   message(frame);

	QVERIFY(!message.valid());
	QVERIFY(message.toString().size() > 0);
}

void TestCan::testValidService()
{
	MrwBusService service("vcan0");
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

void TestCan::testManualConnectService()
{
	ManualCanService service;
	QSignalSpy       spy(&service, &ManualCanService::connected);

	QVERIFY(!service.valid());

	service.connect();
	QVERIFY(spy.wait(2000));
	QCOMPARE(spy.count(), 1);
	QVERIFY(service.valid());

	service.disconnect();
	QVERIFY(!service.valid());
}

void TestCan::testReadWrite()
{
	ManualCanService service(true);

	QVERIFY(service.valid());
	QVERIFY(service.write(MrwMessage(PING)));

	QTest::qWait(50);
	QCOMPARE(service.counted(), 1);

	const MrwMessage & message = service.list().at(0);
	QVERIFY(message.valid());
	QVERIFY(!message.isResponse());
	QCOMPARE(message.command(),  PING);
	QCOMPARE(message.size(),     0u);
	QCOMPARE(message.unitNo(),   NO_UNITNO);
	QCOMPARE(message.response(), Response::MSG_NO_RESPONSE);
	QCOMPARE(message.id(),       CAN_BROADCAST_ID);
	QCOMPARE(message.sid(),      CAN_BROADCAST_ID);
	QCOMPARE(message.eid(),      NO_UNITNO);
}

void TestCan::testReceivedResult()
{
	QByteArray   array;
	QCanBusFrame frame;

	array.append(GETRBS_RESPONSE);
	array.append(std::underlying_type_t<Response>(Response::MSG_QUEUED));
	array.append((uint8_t)TEST_LSB);
	array.append((uint8_t)TEST_MSB);
	frame.setFrameId(TEST_ID);
	frame.setPayload(array);
	MrwMessage message(frame);

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_UNIT_NO);
	QCOMPARE(message.sid(),      TEST_CTRL_ID);
	QCOMPARE(message.command(),  GETRBS);
	QCOMPARE(message.response(), Response::MSG_QUEUED);
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
	QCOMPARE(message.response(),  Response::MSG_NO_RESPONSE);
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
	QCOMPARE(message.response(), Response::MSG_NO_RESPONSE);
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
	QCOMPARE(message.response(), Response::MSG_NO_RESPONSE);
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
	QCOMPARE(message.response(), Response::MSG_NO_RESPONSE);
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
	MrwMessage msg_getver(TEST_CTRL_ID, TEST_UNIT_NO, GETVER, Response::MSG_OK);
	MrwMessage msg_qrybuf(TEST_CTRL_ID, TEST_UNIT_NO, QRYBUF, Response::MSG_OK);
	MrwMessage msg_qryerr(TEST_CTRL_ID, TEST_UNIT_NO, QRYERR, Response::MSG_OK);

	msg_signal.append(std::underlying_type_t<SignalAspect>(SignalAspect::SIGNAL_HP0));
	QVERIFY(msg_signal.toString().contains("Hp0"));

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
	MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, Response::MSG_OK);
	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());

	QVERIFY(message.valid());
	QCOMPARE(message.eid(),      TEST_CTRL_ID);
	QCOMPARE(message.sid(),      CAN_GATEWAY_ID);
	QCOMPARE(message.id(),       TEST_CTRL_ID);
	QCOMPARE(message.command(),  SETLFT);
	QCOMPARE(message.response(), Response::MSG_OK);
	QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
	QVERIFY(message.toString().size() > 0);

	QVERIFY(frame.hasExtendedFrameFormat());
	QCOMPARE(frame.frameId(), TEST_CTRL_ID);
	QCOMPARE(array.size(), 4);
	QCOMPARE(Command( array.at(0)), SETLFT | CMD_RESPONSE);
	QCOMPARE(Response(array.at(1)), Response::MSG_OK);
	QCOMPARE(UnitNo(  array.at(2) & 0xff), TEST_LSB);
	QCOMPARE(UnitNo(  array.at(3) & 0xff), TEST_MSB);
}

void TestCan::testRequestPayload()
{
	MrwMessage   message(PING);
	uint8_t      bytes[7];

	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		bytes[t] = 0x12 + t + 0x22;

		QCOMPARE(message.size(), t);
		MRW_THROWS_EXCEPTION(message[t], std::out_of_range);
		message.append(bytes[t]);
		for (size_t b = 0; b < t; b++)
		{
			QCOMPARE(message[b], bytes[b]);
		}
	}

	MRW_THROWS_EXCEPTION(message.append(0xff), std::out_of_range);

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
	MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, Response::MSG_OK);
	uint8_t      bytes[4];

	for (size_t t = 0; t < sizeof(bytes); t++)
	{
		bytes[t] = 0x12 + t + 0x22;

		QCOMPARE(message.size(), t);
		MRW_THROWS_EXCEPTION(message[t], std::out_of_range);
		message.append(bytes[t]);
		for (size_t b = 0; b < t; b++)
		{
			QCOMPARE(message[b], bytes[b]);
		}
	}

	MRW_THROWS_EXCEPTION(message.append(0xff), std::out_of_range);

	QCanBusFrame frame(message);
	QByteArray   array(frame.payload());
	uint8_t      command = (unsigned)array.at(0) & 0xff;

	QCOMPARE(array.size(), 8);
	QCOMPARE(command, SETLFT | CMD_RESPONSE);
	QCOMPARE(Response(array.at(1)), Response::MSG_OK);
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
		QCOMPARE(message.response(), Response::MSG_NO_RESPONSE);
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
		MrwMessage   message(TEST_CTRL_ID, TEST_UNIT_NO, SETLFT, Response::MSG_QUEUED);

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
		QCOMPARE(message.response(), Response::MSG_QUEUED);
		QCOMPARE(message.unitNo(),   TEST_UNIT_NO);
		QVERIFY(message.toString().size() > 0);

		QVERIFY(frame.hasExtendedFrameFormat());
		QCOMPARE(frame.frameId(), TEST_CTRL_ID);
		QCOMPARE((size_t)array.size(), i + 1);
		QCOMPARE(Command( array.at(0)), SETLFT | CMD_RESPONSE);
		QCOMPARE(Response(array.at(1)), Response::MSG_QUEUED);

		for (size_t v = start; v <= i; v++)
		{
			QCOMPARE((size_t)array.at(v), 0x11 * v);
		}
	}
}
