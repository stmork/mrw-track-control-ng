//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ui/routelistwidget.h>

using namespace mrw::ui;

RouteListWidget::RouteListWidget(QWidget * parent) : QListWidget(parent)
{
}

void RouteListWidget::collect(std::vector<ControlledRoute *> & routes) const
{
	// Collect active routes.
	for (int index = 0; index < count(); index++)
	{
		QListWidgetItem * element = item(index);
		ControlledRoute   *   route   = element->data(ControlledRoute::USER_ROLE).value<ControlledRoute *>();

		routes.push_back(route);
	}
}

ControlledRoute * RouteListWidget::selected() const
{
	for (QListWidgetItem * item : selectedItems())
	{
		return item->data(ControlledRoute::USER_ROLE).value<ControlledRoute *>();
	}
	return nullptr;
}
