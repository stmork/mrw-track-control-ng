//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_RAILCONTROLLER_H
#define MRW_CTRL_RAILCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	class RailController: public BaseController
	{
		Q_OBJECT

	public:
		explicit RailController(QObject * parent = nullptr);

		virtual bool  aEnds()  const = 0;
		virtual bool  bEnds() const = 0;
	};
}

#endif
