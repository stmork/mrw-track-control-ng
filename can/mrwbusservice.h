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
	/**
	 * This class connects to the CAN bus for input and output processing. If
	 * the given interface is not available the service looks for another
	 * available interface.
	 */
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

		/**
		 * This method returns true on successful connection to the CAN bus.
		 *
		 * @return True on succesful CAN bus connection.
		 */
		bool valid();

		/**
		 * This is a convenience method which shows all CAN plugins and their
		 * connected devices.
		 *
		 * @return True on success.
		 */
		bool list();

		/**
		 * This method writes a MrwMessage as a CAN frame onto the CAN bus.
		 *
		 * @param message The MrwMessage to write.
		 * @return True on successful writing.
		 */
		bool write(const MrwMessage & message);

		/**
		 * This method processes a single MrwMessage. It is intended to
		 * overload this method to do further processing. The default
		 * implementation simply dumps the MrwMessage.
		 *
		 * @note The given MrwMessage may be invalid.
		 *
		 * @param message The received MrwMessage.
		 *
		 * @see MrwMessage::valid()
		 */
		virtual void process(const MrwMessage & message);

	signals:
		void connected();
		void disconnected();

	private:
		QString select(const QString & interface, const QString & plugin);

	private slots:
		void stateChanged(QCanBusDevice::CanBusDeviceState state);
		void receive();
	};
}

#endif
