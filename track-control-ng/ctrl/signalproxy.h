//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

		mrw::model::Signal * signal = nullptr;

	public:
		explicit SignalProxy();
		virtual ~SignalProxy();

		void start(mrw::model::Signal * input);

		bool         hasSignal() override;
		virtual void prepare(sc::integer symbol) override;
		virtual void send() override;

		virtual void prepare(mrw::model::Signal::Symbol symbol) = 0;

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
		void prepare(mrw::model::Signal::Symbol symbol) override;

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

		void prepare(mrw::model::Signal::Symbol symbol) override;
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

		void prepare(mrw::model::Signal::Symbol symbol) override;
	};
}

#endif
