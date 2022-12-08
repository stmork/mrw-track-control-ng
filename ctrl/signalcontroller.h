//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLER_H
#define MRW_CTRL_SIGNALCONTROLLER_H

#include <ctrl/basecontroller.h>
#include <model/signal.h>

namespace mrw::ctrl
{
	class SignalController : public BaseController
	{
		Q_OBJECT

	public:
		explicit SignalController(QObject * parent = nullptr);

		virtual bool      hasShunting() const = 0;
		virtual bool      hasDistant()  const = 0;
		virtual bool      hasMain()     const = 0;

		virtual mrw::model::Signal::Symbol distant()     const = 0;
		virtual mrw::model::Signal::Symbol shunt()       const = 0;
		virtual mrw::model::Signal::Symbol main()        const = 0;
	};
}

#endif
