//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALPROXY_H
#define MRW_CTRL_SIGNALPROXY_H

#include <util/self.h>
#include <model/signal.h>
#include <statecharts/SignalStatechart.h>

namespace mrw::ctrl
{
	class SignalControllerProxy;

	class SignalProxy :
		public mrw::statechart::SignalStatechart,
		public mrw::util::Self<mrw::statechart::SignalStatechart::OperationCallback>
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

	protected:
		bool setAspect(const mrw::can::SignalAspect aspect);
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for main signals                                  **
	**                                                                      **
	*************************************************************************/

	class MainProxy :
		public SignalProxy,
		public mrw::util::SelfPointer<MainProxy>
	{
		Q_OBJECT

		size_t curved = 0;

	public:
		MainProxy();

		/**
		 * This method translates a given SignalSymbol (STOP, GO or OFF) into
		 * the correct real life SignalAspect.
		 *
		 * @see mrw::model::Signal::Symbol
		 * @see mrw::can::SignalAspect
		 */
		bool prepare() override;

		/**
		 * This method sets the amount of branched rail parts embraced of the
		 * block starting with this MainSignal. Dependend on the amount of
		 * branched rail parts the SignbalAspect will be HP2 instead of HP1.
		 *
		 * @param count The real amount of branched rail parts.
		 * @see SLOW_BRANCHED_LIMIT
		 */
		void setCurveCount(const size_t count);
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for distant                                       **
	**                                                                      **
	*************************************************************************/

	class DistantProxy :
		public SignalProxy,
		public mrw::util::SelfPointer<DistantProxy>
	{
		Q_OBJECT

		mrw::model::Signal   *  combined_signal = nullptr;
		SignalControllerProxy * main_controller = nullptr;

	public:
		DistantProxy();

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
		bool prepare() override;

		mrw::model::Signal::Symbol getPreparedSymbol() const;
	};

	/*************************************************************************
	**                                                                      **
	**       Signal proxy for shunting                                      **
	**                                                                      **
	*************************************************************************/

	class ShuntProxy :
		public SignalProxy,
		public mrw::util::SelfPointer<ShuntProxy>
	{
		Q_OBJECT

		mrw::model::Signal * main_signal = nullptr;

	public:
		ShuntProxy();

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
		bool prepare() override;
	};
}

#endif
