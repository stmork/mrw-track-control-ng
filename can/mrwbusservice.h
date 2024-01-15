//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CAN_MRWBUSSERVICE_H
#define MRW_CAN_MRWBUSSERVICE_H

#include <QCanBus>
#include <QCanBusDevice>

#include <can/mrwmessage.h>

/**
 * The mrw::can namespace provides the CAN bus infrastructure using the Qt
 * serialbus API. The classes are prepared for using with the model railway
 * infrastructure. So all model railway constants for Command and Result codes
 * are defined in this namespace. To use the CAN bus simply deriv a class from
 * the MrwBus Service class and overload the MrwBusService::process() method.
 * This method gets the preparsed CAN message as a MrwMessage instance.
 */
namespace mrw::can
{
	/**
	 * This class connects to the CAN bus for input and output processing. If
	 * the given interface is not available the service looks for another
	 * available interface.
	 *
	 * For the CAN bus communication you can configure the interface and plugin
	 * in the QCanBusDevice class manner. To use a unique behaviour on a per
	 * host basis use the CanSettings class.
	 *
	 * The MrwBusService allows to connect to the CAN bus manually. So you have
	 * to set the @c auto_connect parameter to false and call the
	 * QCanBusService::connectDevice() method manually.
	 */
	class MrwBusService : public QObject
	{
		Q_OBJECT

	protected:
		QCanBus     *    can_bus    = nullptr;
		QCanBusDevice  * can_device = nullptr;

	public:
		explicit MrwBusService(
			const QString & interface    = "can0",
			const QString & plugin       = "socketcan",
			QObject    *    parent       = nullptr,
			const bool      auto_connect = true);
		MrwBusService() = delete;
		~MrwBusService();

		/**
		 * This method returns true on successful connection to the CAN bus.
		 *
		 * @return True on succesful CAN bus connection.
		 */
		bool valid() noexcept;

		/**
		 * This is a convenience method which shows all CAN plugins and their
		 * connected devices.
		 *
		 * @return True on success.
		 */
		bool list() noexcept;

		/**
		 * This method writes a MrwMessage as a CAN frame onto the CAN bus.
		 *
		 * @param message The MrwMessage to write.
		 * @return True on successful writing.
		 */
		bool write(const MrwMessage & message) noexcept;

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
		QString select(const QString & interface, const QString & plugin) noexcept;

	private slots:
		void stateChanged(QCanBusDevice::CanBusDeviceState state) noexcept;
		void receive() noexcept;
	};
}

#endif
