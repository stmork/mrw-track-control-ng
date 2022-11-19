//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLERPROXY_H
#define MRW_CTRL_SECTIONCONTROLLERPROXY_H

#include <model/section.h>
#include <ctrl/sectioncontroller.h>

namespace mrw::ctrl
{
	class SectionControllerProxy : public SectionController
	{
		Q_OBJECT

	private:
		mrw::model::Section  *  section   = nullptr;

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
	};
}

#endif
