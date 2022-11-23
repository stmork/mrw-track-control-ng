//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLERPROXY_H
#define MRW_CTRL_SECTIONCONTROLLERPROXY_H

#include <ctrl/sectioncontroller.h>
#include <model/region.h>
#include <model/section.h>
#include <model/rail.h>

namespace mrw::ctrl
{
	class SectionControllerProxy : public SectionController
	{
		Q_OBJECT

	private:
		mrw::model::Section  *  section   = nullptr;
		mrw::model::Rail    *   rail      = nullptr;

	public:
		explicit SectionControllerProxy(
			mrw::model::Section * parent_section,
			mrw::model::Rail   *  referenced_rail,
			QObject       *       parent);

		virtual QString name() const override;

		virtual bool isDirection() const override
		{
			return rail->aIsDir() == section->region()->direction();
		}

		virtual bool forwardEnds() const override
		{
			return rail->advance(true).empty();
		}

		virtual bool backwardEnds() const override
		{
			return rail->advance(false).empty();
		}

		virtual float extensions() const override
		{
			return rail->extend();
		}

		virtual mrw::model::Position * position() const override
		{
			return rail;
		}

		virtual mrw::model::SectionState state() const override
		{
			return section->state();
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return section->lock();
		}
	};
}

#endif
