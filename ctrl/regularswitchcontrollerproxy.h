//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H

#include <ctrl/regularswitchcontroller.h>
#include <model/regularswitch.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerProxy : public RegularSwitchController
	{
		Q_OBJECT

	private:
		mrw::model::RegularSwitch * part = nullptr;

	public:
		explicit RegularSwitchControllerProxy(
			mrw::model::RegularSwitch * new_part,
			QObject          *          parent = nullptr);

		virtual bool isLeft() const override;
		virtual bool isRight() const override;
		virtual bool isRightHanded() const override;
		virtual bool isDirection() const override;
		virtual bool isInclined() const override;
		virtual QString name() const override;

		virtual mrw::model::Position * position() const override
		{
			return part;
		}

		virtual mrw::model::SectionState state() const override
		{
			return part->section()->state();
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return part->lock();
		}

		virtual mrw::model::Position::Bending bending() const override
		{
			return part->bending();
		}
	};
}

#endif
