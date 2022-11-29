//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLER_H
#define MRW_CTRL_SECTIONCONTROLLER_H

#include <model/section.h>
#include <ctrl/basecontroller.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/SectionStatechart.h>

namespace mrw::ctrl
{
	class SectionController :
		public BaseController,
		public ControllerRegistrand,
		public mrw::statechart::SectionStatechart::OperationCallback
	{
		Q_OBJECT

	private:
		mrw::statechart::SectionStatechart  statechart;
		mrw::model::Section        *        ctrl_section;

	public:
		explicit SectionController(
			mrw::model::Section * input,
			QObject       *       parent = nullptr);
		virtual ~SectionController();

		operator mrw::model::Section * () const;

		model::Section * section() const;

		// Implementations from BaseController
		virtual QString name() const override;
		virtual float   extensions() const override;
		virtual bool    isDirection() const override;
		virtual bool    isExpandable() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from ControllerRegistrand
		virtual bool process(const can::MrwMessage & message) override;

		// Implementations from OperationCallback
		virtual void off() override;
		virtual void on() override;
		virtual void request() override;
	};
}

#endif
