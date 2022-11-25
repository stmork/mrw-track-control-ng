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

		// Implementations from BaseController
		virtual QString name() const override;
		virtual float   extensions() const override;
		virtual bool    isDirection() const override;
		virtual bool    isExpandable() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from SectionController
		virtual bool    aEnds() const override;
		virtual bool    bEnds() const override;
	};
}

#endif
