//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLER_H
#define MRW_CTRL_SIGNALCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	class SignalController : public BaseController
	{
		Q_OBJECT

	public:
		enum class TourState
		{
			STOP, GO
		};

		explicit SignalController(QObject * parent = nullptr);

		virtual bool      hasShunting() const = 0;
		virtual bool      hasDistant()  const = 0;
		virtual bool      hasMain()     const = 0;

		virtual TourState distant()     const = 0;
		virtual TourState shunt()       const = 0;
		virtual TourState main()        const = 0;
	};
}

#endif
