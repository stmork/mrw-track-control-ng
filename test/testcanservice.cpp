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
#include "util/appsupport.h"
#include "util/settings.h"

#include "testcanservice.h"

using namespace mrw::test;
using namespace mrw::can;
using namespace mrw::util;

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
		const bool      auto_connect,
		const QString & iface,
		const QString & plugin) :
		MrwBusService(iface, plugin, nullptr, auto_connect)
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

TestCanService::TestCanService(QObject * parent) : QObject(parent)
{
	Settings      settings("test");
	SettingsGroup group (&settings, AppSupport::instance().hostname());

	can_iface  = settings.value("interface", "vcan0").toString();
	can_plugin = settings.value("plugin",    "socketcan").toString();
}

void TestCanService::testValidService()
{
	MrwBusService service(can_iface, can_plugin);
	MrwMessage    message(PING);

	QTest::qWait(50);
	QVERIFY(service.valid());
	QVERIFY(service.write(message));
	service.list();
}

void TestCanService::testTryValidService()
{
#ifdef __linux__

	MrwBusService service("eth0", "socketcan");
	MrwMessage    message(PING);

	QTest::qWait(50);
	QVERIFY(service.valid());
	QVERIFY(service.write(message));
	service.list();

#endif
}

void TestCanService::testInvalidService()
{
	MrwBusService service("no-interface", "no-plugin");
	MrwMessage    message(PING);

	QVERIFY(!service.valid());
	QVERIFY(!service.write(message));
	service.list();
}

void TestCanService::testManualConnectService()
{
	ManualCanService service(false, can_iface, can_plugin);
	QSignalSpy       spy(&service, &ManualCanService::connected);

	QVERIFY(!service.valid());

	service.connect();
	QVERIFY(spy.wait(2000));
	QCOMPARE(spy.count(), 1);
	QVERIFY(service.valid());

	service.disconnect();
	QVERIFY(!service.valid());
}

void TestCanService::testReadWrite()
{
	ManualCanService service(true, can_iface, can_plugin);

	QTest::qWait(50);
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
