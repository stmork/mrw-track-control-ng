//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLER_H
#define MRW_CTRL_SIGNALCONTROLLER_H

#include <QObject>

namespace mrw::ctrl
{
	class SignalController : public QObject
	{
		Q_OBJECT
	public:
		explicit SignalController(QObject * parent = nullptr);

	signals:

	};
}

#endif
