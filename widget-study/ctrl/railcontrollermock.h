//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
		bool                           a_is_dir      = false;
		unsigned                       ext_count     = 0;
		unsigned                       line_count    = 0;
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

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setLock(const mrw::model::Device::LockState lock);
		void setEnds(const bool a, const bool b);
		void setBending(const mrw::model::Position::Bending curve);

	private:

		// Implementations from BaseController
		virtual QString name()        const override;
		virtual float   extensions()  const override;
		virtual float   lines()       const override;
		virtual bool    isDirection() const override;

		virtual mrw::model::SectionState      state()   const override;
		virtual mrw::model::Device::LockState lock()    const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from RailController
		virtual bool  aEnds() const override;
		virtual bool  bEnds() const override;

	signals:
		void extend();
		void computeConnectors();

	public slots:
		void setExtension(const int ext_count);
		void setLines(const int line_count);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
