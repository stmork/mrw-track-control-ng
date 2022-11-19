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
		mrw::model::SectionState section_state = mrw::model::SectionState::FREE;

	public:
		explicit SectionControllerMock(QObject * parent = nullptr);

		virtual QString name() const override;
		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		void setSectionState(const mrw::model::SectionState state);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
