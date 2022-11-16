//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLER_H
#define MRW_CTRL_REGULARSWITCHCONTROLLER_H

#include <QObject>

namespace mrw
{
	namespace ctrl
	{

		class RegularSwitchController : public QObject
		{
			Q_OBJECT
		public:
			explicit RegularSwitchController(QObject * parent = nullptr);

		signals:

		};

	} // namespace ctrl
} // namespace mrw

#endif // MRW_CTRL_REGULARSWITCHCONTROLLER_H
