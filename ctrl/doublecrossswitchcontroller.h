//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H

#include <ctrl/baseswitchcontroller.h>
#include <model/doublecrossswitch.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a double cross switch.
	 *
	 * @see BaseController
	 * @see mrw::model::DoubleCrossSwitch
	 */
	class DoubleCrossSwitchController : public BaseSwitchController
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchController(QObject * parent = nullptr);

		virtual float extensions() const override;

		/**
		 * This returns actual the mrw::model::DoubleCrossSwitch::State.
		 *
		 * @return The actual mrw::model::DoubleCrossSwitch::State.
		 */
		virtual mrw::model::DoubleCrossSwitch::State  switchState() const = 0;
	};
}

#endif
