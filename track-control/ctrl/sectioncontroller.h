//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLER_H
#define MRW_CTRL_SECTIONCONTROLLER_H

#include <util/batchparticipant.h>
#include <util/self.h>
#include <model/section.h>
#include <ctrl/basecontroller.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/timerservice.h>
#include <statecharts/SectionStatechart.h>

namespace mrw::ctrl
{
	class SectionController :
		public BaseController,
		public ControllerRegistrand,
		public mrw::util::BatchParticipant,
		public mrw::util::Self<mrw::statechart::SectionStatechart::OperationCallback>
	{
		Q_OBJECT

	private:
		mrw::statechart::QtStatechart<mrw::statechart::SectionStatechart>  statechart;
		mrw::model::Section                        *                       ctrl_section;
		SectionController                         *                        next = nullptr;

	public:
		explicit SectionController(
			mrw::model::Section * input,
			QObject       *       parent = nullptr);
		virtual ~SectionController();

		mrw::model::Section * section() const;

		void setAutoOff(const bool auto_off);
		void setAutoUnlock(const bool auto_unlock);
		void nextController(SectionController * next);

	signals:
		void enable(bool turn_on = true);
		void disable();
		void unlock();
		void failed();
		void start();
		void entered();
		void leaving();
		void left();
		void tryUnblock();
		void unregister();
		void stop();

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

		virtual void off() override;
		virtual void on() override;
		virtual void request() override;
		virtual void passed() override;
		virtual void free() override;
		virtual void leftBefore() override;

		virtual void fail() override;
		virtual void pending() override;
		virtual void lock(const bool do_it) override;
	};
}

#endif
