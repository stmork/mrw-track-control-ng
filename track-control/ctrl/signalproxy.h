//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALPROXY_H
#define MRW_CTRL_SIGNALPROXY_H

#include <model/signal.h>
#include <statecharts/SignalStatechart.h>

namespace mrw::ctrl
{
	class SignalControllerProxy;

	class SignalProxy :
		public mrw::statechart::SignalStatechart,
		public mrw::statechart::SignalStatechart::OperationCallback
	{
		Q_OBJECT

	protected:
		/**
		 * This limit defines how many curved parts in a block (the rail
		 * between two main signals) causes to drive slowly. This limit
		 * selects Hp2 instead of Hp1 for speed limit.
		 */
		static const size_t SLOW_CURVED_LIMIT = 2;

		/** The Signal instance controlled by this controller. */
		mrw::model::Signal * signal = nullptr;

	public:
		explicit SignalProxy();
		virtual ~SignalProxy();

		void start(mrw::model::Signal * input);

		bool         hasSignal() override;
		virtual void send() override;
		virtual void dump() override;

		bool process(model::Signal * device, const can::MrwMessage & message);
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for main signals                                  **
	**                                                                      **
	*************************************************************************/

	class MainProxy: public SignalProxy
	{
		Q_OBJECT

		size_t curved_count = 0;

	public:
		/**
		 * This method translates a given SignalSymbol (STOP, GO or OFF) into
		 * the correct real life SignalAspect.
		 *
		 * @see mrw::model::Signal::Symbol
		 * @see mrw::can::SignalAspect
		 */
		void prepare() override;

		/**
		 * This method sets the amount of curved or branched rail parts
		 * embraced of the block starting with this MainSignal. Dependend
		 * on the amount of curved rail parts the SignbalAspect will be HP2
		 * instead of HP1.
		 *
		 * @param count The real amount of curved or branched rail parts.
		 * @see SLOW_CURVED_LIMIT
		 */
		void setCurved(const size_t count);
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for distant                                       **
	**                                                                      **
	*************************************************************************/

	class DistantProxy: public SignalProxy
	{
		Q_OBJECT

		mrw::model::Signal   *  combined_signal = nullptr;
		SignalControllerProxy * main_controller = nullptr;

	public:
		void start(
			mrw::model::Signal * input,
			mrw::model::Signal * combined);
		SignalControllerProxy * mainController() const;
		void setMainController(SignalControllerProxy * signal);

		/**
		 * This method translates a given SignalSymbol (STOP, GO or OFF) into
		 * the correct real life SignalAspect.
		 *
		 * @see mrw::model::Signal::Symbol
		 * @see mrw::can::SignalAspect
		 */
		void prepare() override;

		mrw::model::Signal::Symbol getPreparedSymbol() const;
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for shunting                                      **
	**                                                                      **
	*************************************************************************/

	class ShuntProxy: public SignalProxy
	{
		Q_OBJECT

		mrw::model::Signal * main_signal = nullptr;

	public:
		void start(
			mrw::model::Signal * input,
			mrw::model::Signal * combined);
		bool isCombined();

		/**
		 * This method translates a given SignalSymbol (STOP, GO or OFF) into
		 * the correct real life SignalAspect.
		 *
		 * @see mrw::model::Signal::Symbol
		 * @see mrw::can::SignalAspect
		 */
		void prepare() override;
	};
}

#endif
