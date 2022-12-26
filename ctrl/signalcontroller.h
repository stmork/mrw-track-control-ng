//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLER_H
#define MRW_CTRL_SIGNALCONTROLLER_H

#include <ctrl/basecontroller.h>
#include <model/signal.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a group of signals inside a Section
	 * orientated in the same counting direction.
	 *
	 * @note The representation does not distinguish between LightSignal and
	 * FormSignal.
	 *
	 * @see BaseController
	 */
	class SignalController : public BaseController
	{
		Q_OBJECT

	public:
		explicit SignalController(QObject * parent = nullptr);

		/**
		 * This method returns true if this Signal group contains a
		 * shunting Signal.
		 *
		 * @return True if this Signal group contains a shunting signal.
		 */
		virtual bool      hasShunting() const = 0;

		/**
		 * This method returns true if this Signal group contains a
		 * distant Signal.
		 *
		 * @return True if this Signal group contains a distant signal.
		 */
		virtual bool      hasDistant()  const = 0;

		/**
		 * This method returns true if this Signal group contains a
		 * main Signal.
		 *
		 * @return True if this Signal group contains a main signal.
		 */
		virtual bool      hasMain()     const = 0;

		/**
		 * This method returns the Signal::Symbol of a distant signal. This
		 * may by identical to a "remote" main Signal.
		 *
		 * @return The Signal::Symbol representing this distant Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol distant()     const = 0;

		/**
		 * This method returns the Signal::Symbol of a shunting signal.
		 *
		 * @note In case of an outgoing main Signal this represented Signal
		 * may be the same instance.
		 *
		 * @return The Signal::Symbol representing this main Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol shunt()       const = 0;

		/**
		 * This method returns the Signal::Symbol of a main signal.
		 *
		 * @return The Signal::Symbol representing this main Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol main()        const = 0;
	};
}

#endif
