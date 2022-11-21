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
			QObject       *       parent);

		QPoint point() const;

		virtual QString name() const override;

		virtual mrw::model::SectionState state() const override
		{
			return section->state();
		}

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

		virtual mrw::model::Device::LockState lock() const override
		{
			return section->lock();
		}
	};
}

#endif
