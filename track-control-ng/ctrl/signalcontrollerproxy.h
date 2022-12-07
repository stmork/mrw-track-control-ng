//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLERPROXY_H
#define MRW_CTRL_SIGNALCONTROLLERPROXY_H

#include <model/section.h>
#include <model/signal.h>
#include <ctrl/signalcontroller.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/SignalStatechart.h>

namespace mrw::ctrl
{
	class SignalControllerProxy :
		public SignalController,
		public ControllerRegistrand,
		public mrw::statechart::SignalStatechart::OperationCallback
	{
		Q_OBJECT

	private:
		mrw::statechart::SignalStatechart  statechart;
		const bool                         direction = true;

		mrw::model::Section * section        = nullptr;
		mrw::model::Signal  * base_signal    = nullptr;
		mrw::model::Signal  * main_signal    = nullptr;
		mrw::model::Signal  * distant_signal = nullptr;
		mrw::model::Signal  * shunt_signal   = nullptr;

		std::unordered_map<mrw::can::UnitNo, mrw::model::Signal *> signal_map;

	public:
		explicit SignalControllerProxy(
			mrw::model::Section * parent_section,
			const bool            direction,
			QObject       *       parent);
		virtual ~SignalControllerProxy();

	private:
		void add(mrw::model::Signal * signal);


		// Implementations from BaseController
		virtual QString name() const override;
		virtual float   extensions() const override;
		virtual bool    isDirection() const override;
		virtual bool    isExpandable() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from SignalController
		virtual bool    hasShunting() const override;
		virtual bool    hasDistant() const override;
		virtual bool    hasMain() const override;

		TourState       distant() const override;
		TourState       shunt() const override;
		TourState       main() const override;

		// Implementations from ControllerRegistrand
		virtual bool    process(const can::MrwMessage & message) override;
		virtual QString toString() const override;

		// Implementations from OperationCallback
		virtual void inc() override;
		virtual void dec() override;

		virtual bool hasMain() override;
		virtual bool hasDistant() override;
		virtual bool hasShunt() override;

		virtual void turnMainSignal(sc::integer symbol) override;
		virtual void turnDistantSignal(sc::integer symbol) override;
		virtual void turnShuntSignal(sc::integer symbol) override;
	};
}

#endif
