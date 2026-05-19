//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_CROSSINGCONTROLLER_H
#define MRW_CTRL_CROSSINGCONTROLLER_H

#include <util/batchparticipant.h>
#include <util/self.h>
#include <model/crossing.h>
#include <ctrl/basecontroller.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/timerservice.h>
#include <statecharts/CrossingStatechart.h>

namespace mrw::ctrl
{
	class CrossingController :
		public BaseController,
		public ControllerRegistrand,
		public mrw::util::BatchParticipant,
		public mrw::util::Self<mrw::statechart::CrossingStatechart::OperationCallback>
	{
		Q_OBJECT
	private:
		mrw::statechart::QtStatechart<mrw::statechart::CrossingStatechart>  statechart;
		mrw::model::Crossing                        *                       ctrl_crossing = nullptr;

	public:
		CrossingController() = delete;
		explicit CrossingController(
			mrw::model::Crossing * input,
			QObject        *       parent = nullptr);
		virtual ~CrossingController();

	signals:
		void start();
		void update();

	private:
		// Implementations from BaseController
		virtual const QString & name()         const noexcept override;
		virtual float           extensions()   const noexcept override;
		virtual bool            isDirection()  const noexcept override;
		virtual bool            isExpandable() const noexcept override;

		virtual mrw::model::Position     *    position() const noexcept override;
		virtual mrw::model::SectionState      state()    const noexcept override;
		virtual mrw::model::Device::LockState lock()     const noexcept override;
		virtual mrw::model::Position::Bending bending()  const noexcept override;

		// Implementations from ControllerRegistrand
		virtual bool    process(const can::MrwMessage & message) noexcept override;
		virtual void    restart() override;
		virtual QString toString() const override;

		// Implementations from OperationCallback
		virtual void inc() override;
		virtual void dec() override;
		virtual sc::integer used() override;
		virtual void close() override;
		virtual void open() override;
		virtual void fail() override;
		virtual void pending() override;
		virtual void lock(const bool do_it) override;

	};
}

#endif
