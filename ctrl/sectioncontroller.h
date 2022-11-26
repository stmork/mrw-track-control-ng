//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLER_H
#define MRW_CTRL_SECTIONCONTROLLER_H

#include <ctrl/basecontroller.h>
#include <model/section.h>

namespace mrw::ctrl
{
	class SectionController : public mrw::ctrl::BaseController
	{
		Q_OBJECT

	private:
		mrw::model::Section * ctrl_section;

	public:
		explicit SectionController(
			mrw::model::Section * input,
			QObject       *       parent = nullptr);

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
	};
}

#endif
