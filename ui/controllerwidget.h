//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CONTROLLERWIDGET_H
#define MRW_UI_CONTROLLERWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	class ControllerWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit ControllerWidget(QWidget * parent = nullptr);
	};
}

#endif
