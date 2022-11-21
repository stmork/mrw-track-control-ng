//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BASECONTROLLER_H
#define MRW_CTRL_BASECONTROLLER_H

#include <QObject>

#include <model/device.h>
#include <model/section.h>

namespace mrw::ctrl
{
	class BaseController : public QObject
	{
		Q_OBJECT

	public:
		explicit BaseController(QObject * parent = nullptr);

		virtual QString name() const = 0;
		virtual mrw::model::SectionState state() const = 0;
		virtual mrw::model::Device::LockState lock() const = 0;

	signals:
		void update();

	};
}

#endif
