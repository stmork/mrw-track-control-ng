//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_ROUTELISTWIDGET_H
#define MRW_UI_ROUTELISTWIDGET_H

#include <QListWidget>

#include "controlledroute.h"

namespace mrw::ui
{
	class RouteListWidget : public QListWidget
	{
		Q_OBJECT
	public:
		explicit RouteListWidget(QWidget * parent = nullptr);

		void collect(std::vector<ControlledRoute *> & routes) const;
		ControlledRoute * selected() const;
	};
}

#endif
