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

namespace mrw::ctrl
{
	class SignalControllerProxy : public SignalController
	{
		Q_OBJECT

	private:
		mrw::model::Section * section   = nullptr;
		const bool            direction = true;
		QPoint                position;
		QString               signal_name;

		mrw::model::Signal  * main_signal    = nullptr;
		mrw::model::Signal  * distant_signal = nullptr;
		mrw::model::Signal  * shunt_signal   = nullptr;

	public:
		explicit SignalControllerProxy(
			mrw::model::Section * parent_section,
			const bool            direction,
			QObject       *       parent);

		QPoint point() const;
		virtual bool isDirection() const override;
		virtual bool hasShunting() const override;
		virtual bool hasDistant() const override;
		virtual bool hasMain() const override;
		virtual QString name() const override;

		TourState distant() const override;
		TourState shunt() const override;
		TourState main() const override;

		virtual mrw::model::SectionState state() const override
		{
			return section->state();
		}
	};
}

#endif