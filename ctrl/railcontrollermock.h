//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_RAILCONTROLLERMOCK_H
#define MRW_CTRL_RAILCONTROLLERMOCK_H

#include <ctrl/railcontroller.h>

namespace mrw::ctrl
{
	class RailControllerMock : public RailController
	{
		Q_OBJECT

	private:
		unsigned                       extension     = 0;
		bool                           a_is_dir      = false;
		bool                           a_ends        = false;
		bool                           b_ends        = false;

		mrw::model::Device::LockState  lock_state    =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState       section_state =
			mrw::model::SectionState::FREE;
		mrw::model::Position::Bending  bending_state =
			mrw::model::Position::Bending::STRAIGHT;

	public:
		explicit RailControllerMock(QObject * parent = nullptr);

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
			return !a_is_dir;
		}

		virtual bool aEnds() const override
		{
			return a_ends;
		}

		virtual bool bEnds() const override
		{
			return b_ends;
		}

		virtual float extensions() const override
		{
			return extension;
		}

		virtual mrw::model::Position::Bending bending() const override
		{
			return bending_state;
		}

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setLock(const mrw::model::Device::LockState lock);
		void setEnds(const bool a, const bool b);
		void setBending(const mrw::model::Position::Bending curve);

	signals:
		void extend();
		void computeConnectors();

	public slots:
		void setExtension(const int extension);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
