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

		virtual bool isDirection() const = 0;
		virtual bool hasShunting() const = 0;
		virtual bool hasDistant() const = 0;
		virtual bool hasMain() const = 0;
		virtual QString name() const = 0;

	signals:
		void update();
	};
}

#endif
