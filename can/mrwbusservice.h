//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#ifndef MRW_CAN_MRWBUSSERVICE_H
#define MRW_CAN_MRWBUSSERVICE_H

#include <QCanBus>
#include <QCanBusDevice>

#include <can/mrwmessage.h>

namespace mrw::can
{
	class MrwBusService : public QObject
	{
		Q_OBJECT

		QCanBus     *    can_bus;
		QCanBusDevice  * device;

	public:
		explicit MrwBusService(
			const char * interface = "can0",
			const char * plugin    = "socketcan",
			QObject   *  parent    = nullptr);
		~MrwBusService();

		bool valid();
		void list();
		bool write(const MrwMessage & message);
		virtual void process(const MrwMessage & message);

	private:
		QString select(const QString & interface, const QString & plugin);

	private slots:
		void receive();
	};
}

#endif
