//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QDebug>

#include <can/mrwmessage.h>
#include <can/mrwbusservice.h>
#include <util/termhandler.h>

using namespace mrw::can;
using namespace mrw::util;


class CanBusProxyService : public MrwBusService
{
	MrwBusService * proxy = nullptr;

public:
	CanBusProxyService(
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
	CanBusProxyService    tcp_service("can0", "virtualcan");
	CanBusProxyService    can_service("can0");
	TermHandler           term_handler( { SIGTERM, SIGINT } );

	tcp_service.setProxy(&can_service);
	can_service.setProxy(&tcp_service);

	return app.exec();
}
