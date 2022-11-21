//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLERMOCK_H
#define MRW_CTRL_SECTIONCONTROLLERMOCK_H

#include <ctrl/sectioncontroller.h>

namespace mrw::ctrl
{
	class SectionControllerMock : public SectionController
	{
		Q_OBJECT

	private:
		bool                           direction     = true;
		bool                           a_ends        = false;
		bool                           b_ends        = false;

		mrw::model::Device::LockState  lock_state    =
				mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState       section_state =
				mrw::model::SectionState::FREE;

	public:
		explicit SectionControllerMock(QObject * parent = nullptr);

		virtual QString name() const override;

		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return lock_state;
		}

		virtual bool isDirection() const override
		{
			return direction;
		}

		virtual bool forwardEnds() const override
		{
			return a_ends;
		}

		virtual bool backwardEnds() const override
		{
			return b_ends;
		}

		void setSectionState(const mrw::model::SectionState state);
		void setDirection(const bool dir = true);
		void setEnds(const bool a, const bool b);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
