//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLERMOCK_H
#define MRW_CTRL_SIGNALCONTROLLERMOCK_H

#include <ctrl/signalcontroller.h>

namespace mrw::ctrl
{
	class SignalControllerMock : public SignalController
	{
		Q_OBJECT

	private:
		bool direction      = true;
		bool shunting       = false;
		bool distant_signal = false;
		bool main_signal    = true;

		mrw::model::SectionState section_state = mrw::model::SectionState::FREE;

	public:
		explicit SignalControllerMock(QObject * parent = nullptr);

		virtual bool isDirection() const override;
		virtual bool hasShunting() const override;
		virtual bool hasDistant() const override;
		virtual bool hasMain() const override;
		virtual QString name() const override;
		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);

	public slots:
		void setShunting(const bool shunt = true);
		void setDistant(const bool distant = true);
		void setMain(const bool main = true);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
