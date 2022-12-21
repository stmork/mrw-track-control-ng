//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_STATECHART_SIGNALPROXY_H
#define MRW_STATECHART_SIGNALPROXY_H

#include <model/signal.h>
#include <statecharts/SignalStatechart.h>

namespace mrw::statechart
{
	class SignalProxy :
		public SignalStatechart,
		public SignalStatechart::OperationCallback
	{
		Q_OBJECT

	protected:

		mrw::model::Signal * signal = nullptr;

	public:
		explicit SignalProxy();
		virtual ~SignalProxy();

		void start(mrw::model::Signal * input);
		bool hasSignal() override;
		void idle() override;
		bool process(model::Signal * device, const can::MrwMessage & message);
	};

	class MainProxy: public SignalProxy
	{
		Q_OBJECT

		size_t curved_count = 0;

	public:
		void send(sc::integer symbol) override;

		void setCurved(const size_t count);
	};

	class DistantProxy: public SignalProxy
	{
		Q_OBJECT

		mrw::model::Signal * combined_signal = nullptr;
		mrw::model::Signal * main_signal     = nullptr;

	public:
		void start(
			mrw::model::Signal * input,
			mrw::model::Signal * combined);
		void send(sc::integer symbol) override;

		void setMainSignal(model::Signal * signal);
	};

	class ShuntProxy: public SignalProxy
	{
		Q_OBJECT

		mrw::model::Signal * main_signal = nullptr;

	public:
		void start(
			mrw::model::Signal * input,
			mrw::model::Signal * combined);
		bool isCombined();
		void send(sc::integer symbol) override;
	};
}

#endif
