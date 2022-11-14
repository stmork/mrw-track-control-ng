//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_DEVICE_H
#define MRW_MODEL_DEVICE_H

#include <can/types.h>
#include <model/module.h>

namespace mrw::model
{
	class Controller;

	/**
	 * This class represents device components for which a corresponding
	 * CAN command is existing. The CAN ID is built from the controller ID
	 * and the unit number of this Device.
	 *
	 * @see Controller::id()
	 */
	class Device
	{
		const mrw::can::UnitNo   unit_no;

	public:
		explicit Device(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		/**
		 * This method returns the unit number which is part of the CAN
		 * identifier sent to the controller with a CAN command. It is
		 * directly used inside the CAN extended frame format ID.
		 *
		 * @note Even the extended frame format ID is 18 bits wide only
		 * 16 bits are used from this unit number.
		 *
		 * @return The unit number of this Device.
		 * @see Controller::id()
		 */
		inline mrw::can::UnitNo unitNo() const
		{
			return unit_no;
		}

		/**
		 * This method returns the pointer to the managing controller.
		 *
		 * @return
		 */
		virtual Controller * controller() const = 0;

		/**
		 * This method returns the name modelled inside the EMF/XMI model
		 * file.
		 *
		 * @return The Device name modelled inside the EMF/XMI model.
		 */
		virtual const QString & name() const = 0;
	};
}

#endif
