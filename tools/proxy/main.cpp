//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QDebug>
#include <QTimer>

#include <util/duration.h>
#include <util/termhandler.h>
#include <can/mrwmessage.h>
#include <can/mrwbusservice.h>

using namespace mrw::util;
using namespace mrw::can;

class CanBusProxyService : public MrwBusService
{
	MrwBusService * proxy = nullptr;

public:
	explicit CanBusProxyService(
		const char * interface = "can0",
		const char * plugin    = "socketcan",
		QObject   *  parent    = nullptr) :
		MrwBusService(interface, plugin, parent)
	{
	}

	void setProxy(MrwBusService * other)
	{
		proxy = other;
	}

	virtual void process(const MrwMessage & message) override
	{
		Q_ASSERT(proxy != nullptr);

		proxy->write(message);
	}
};

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Duration::pattern();

	CanBusProxyService    tcp_service("can0", "virtualcan");
	CanBusProxyService    can_service("can0");
	TermHandler           term_handler;

	tcp_service.setProxy(&can_service);
	can_service.setProxy(&tcp_service);

	return app.exec();
}
