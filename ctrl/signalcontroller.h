//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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
	 * @note The representation does not distinguish between
	 * mrw::model::LightSignal and mrw::model::FormSignal.
	 *
	 * @see BaseController
	 * @see mrw::model::Signal
	 */
	class SignalController : public BaseController
	{
		Q_OBJECT

	public:
		/**
		 * This struct contains the complete Status of this SignalController
		 * instance including the BaseController::Status.
		 */
		struct Status : public BaseController::Status
		{
			/**
			 * The mrw::model::Signal::Symbol representing this
			 * main mrw::model::Signal.
			 *
			 * @see SignalController::main()
			 */
			mrw::model::Signal::Symbol main_state    = mrw::model::Signal::Symbol::OFF;

			/**
			 * The mrw::model::Signal::Symbol representing this distant
			 * mrw::model::Signal.
			 *
			 * @see SignalController::distant()
			 */
			mrw::model::Signal::Symbol distant_state = mrw::model::Signal::Symbol::OFF;

			/**
			 * The mrw::model::Signal::Symbol representing this shunting
			 * mrw::model::Signal.
			 *
			 * @see SignalController::shunt()
			 */
			mrw::model::Signal::Symbol shunt_state   = mrw::model::Signal::Symbol::OFF;

			/**
			 * True if this mrw::model::Signal group contains a main
			 * mrw::model::Signal.
			 *
			 * @see SignalController::hasMain()
			 */
			bool                       has_main      = false;

			/**
			 * True if this mrw::model::Signal group contains a distant
			 * mrw::model::Signal.
			 *
			 * @see SignalController::hasDistant()
			 */
			bool                       has_distant   = false;

			/**
			 * True if this mrw::model::Signal group contains a shunting
			 * mrw::model::Signal.
			 *
			 * @see SignalController::hasShunting()
			 */
			bool                       has_shunting  = false;
		};

		explicit SignalController(QObject * parent = nullptr);

		/**
		 * This method returns the complete Status of this SignalController
		 * instance.
		 *
		 * @note This method calls the base class method
		 * BaseController::status() method.
		 *
		 * @param status The Status struct to fill.
		 */
		void status(SignalController::Status & status) const;

	private:
		virtual bool            isExpandable() const override;

		/**
		 * This method returns true if this mrw::model::Signal group contains a
		 * shunting mrw::model::Signal.
		 *
		 * @return True if this mrw::model::Signal group contains a shunting
		 * mrw::model::Signal.
		 */
		virtual bool      hasShunting() const = 0;

		/**
		 * This method returns true if this mrw::model::Signal group contains a
		 * distant mrw::model::Signal.
		 *
		 * @return True if this mrw::model::Signal group contains a distant
		 * mrw::model::Signal.
		 */
		virtual bool      hasDistant()  const = 0;

		/**
		 * This method returns true if this mrw::model::Signal group contains a
		 * main mrw::model::Signal.
		 *
		 * @return True if this mrw::model::Signal group contains a main
		 * mrw::model::Signal.
		 */
		virtual bool      hasMain()     const = 0;

		/**
		 * This method returns the mrw::model::Signal::Symbol of a distant
		 * mrw::model::Signal. This may by identical to a "remote" main
		 * mrw::model::Signal.
		 *
		 * @return The mrw::model::Signal::Symbol representing this distant
		 * mrw::model::Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol distant()     const = 0;

		/**
		 * This method returns the mrw::model::Signal::Symbol of a shunting
		 * mrw::model::Signal.
		 *
		 * @note In case of an outgoing main Signal this represented
		 * mrw::model::Signal may be the same instance.
		 *
		 * @return The mrw::model::Signal::Symbol representing this main
		 * mrw::model::Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol shunt()       const = 0;

		/**
		 * This method returns the mrw::model::Signal::Symbol of a main
		 * mrw::model::Signal.
		 *
		 * @return The Signal::Symbol representing this main mrw::model::Signal.
		 * @see mrw::model::Signal::Symbol
		 */
		virtual mrw::model::Signal::Symbol main()        const = 0;
	};
}

#endif
