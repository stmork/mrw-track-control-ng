//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLER_H
#define MRW_CTRL_REGULARSWITCHCONTROLLER_H

#include <QObject>

#include <model/section.h>

namespace mrw::ctrl
{
	class RegularSwitchController : public QObject
	{
		Q_OBJECT

	public:
		explicit RegularSwitchController(QObject * parent = nullptr);

		virtual bool isLeft() const = 0;
		virtual bool isRight() const = 0;
		virtual bool isRightHanded() const = 0;
		virtual bool isDirection() const = 0;
		virtual QString name() const = 0;
		virtual mrw::model::SectionState state() const = 0;

	signals:
		void update();
	};
}

#endif
