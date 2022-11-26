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

namespace mrw::ctrl
{
	class SignalControllerProxy :
		public SignalController,
		public ControllerRegistrand
	{
		Q_OBJECT

	private:
		const bool            direction = true;

		mrw::model::Section * section        = nullptr;
		mrw::model::Signal  * base_signal    = nullptr;
		mrw::model::Signal  * main_signal    = nullptr;
		mrw::model::Signal  * distant_signal = nullptr;
		mrw::model::Signal  * shunt_signal   = nullptr;

	public:
		explicit SignalControllerProxy(
			mrw::model::Section * parent_section,
			const bool            direction,
			QObject       *       parent);
		virtual ~SignalControllerProxy();

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
	};
}

#endif
