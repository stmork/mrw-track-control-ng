//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SECTIONCONTROLLER_H
#define MRW_CTRL_SECTIONCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	class SectionController : public BaseController
	{
		Q_OBJECT

	public:
		explicit SectionController(QObject * parent = nullptr);

		virtual bool isDirection() const = 0;
		virtual bool forwardEnds() const = 0;
		virtual bool backwardEnds() const = 0;
	};
}

#endif
