//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLERPROXY_H
#define MRW_CTRL_SIGNALCONTROLLERPROXY_H

#include <model/section.h>
#include <model/rail.h>
#include <model/signal.h>
#include <ctrl/signalcontroller.h>
#include <ctrl/controllerregistrand.h>
#include <ctrl/railpartinfo.h>
#include <ctrl/signalproxy.h>
#include <statecharts/SignalControllerStatechart.h>
#include <statecharts/SignalStatechart.h>

namespace mrw::ctrl
{
	class SignalControllerProxy :
		public SignalController,
		public RailPartInfo,
		public ControllerRegistrand,
		public mrw::statechart::SignalControllerStatechart::OperationCallback
	{
		Q_OBJECT

	private:
		mrw::statechart::SignalControllerStatechart  statechart;
		mrw::statechart::MainProxy                   statechart_main;
		mrw::statechart::DistantProxy                statechart_distant;
		mrw::statechart::ShuntProxy                  statechart_shunt;
		const bool                                   direction  = true;
		mrw::model::Device::LockState                lock_state =
			mrw::model::Device::LockState::UNLOCKED;

		mrw::model::Section  * signal_section = nullptr;
		mrw::model::Signal  *  base_signal    = nullptr;
		mrw::model::Signal  *  main_signal    = nullptr;
		mrw::model::Signal  *  distant_signal = nullptr;
		mrw::model::Signal  *  shunt_signal   = nullptr;
		mrw::model::RailPart * signal_rail    = nullptr;

		std::unordered_map<mrw::can::UnitNo, mrw::model::Signal *> signal_map;
		QString                                                    grouped_name;

	public:
		explicit SignalControllerProxy(
			mrw::model::Section * parent_section,
			const bool            direction,
			QObject       *       parent);
		virtual ~SignalControllerProxy();

		bool isUnlocked() const;

	signals:
		void enable();
		void disable();
		void failed();
		void start();

	private:
		void connectMain();
		void connectDistant();
		void connectShunt();
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

		mrw::model::Signal::Symbol       distant() const override;
		mrw::model::Signal::Symbol       shunt() const override;
		mrw::model::Signal::Symbol       main() const override;

		// Implementation from RailPartInfo
		virtual mrw::model::RailPart * railPart() const override;
		virtual mrw::model::Section  * section() const override;

		// Implementations from ControllerRegistrand
		virtual bool    process(const can::MrwMessage & message) override;
		virtual QString toString() const override;

		// Implementations from OperationCallback
		virtual void inc() override;
		virtual void dec() override;

		virtual bool hasMain() override;
		virtual bool isMain() override;
		virtual void prepare() override;

		virtual void fail() override;
		virtual void pending() override;
		virtual void lock(const bool do_it) override;
	};
}

#endif
