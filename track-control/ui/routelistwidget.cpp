//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ui/routelistwidget.h>

using namespace mrw::ui;

RouteListWidget::RouteListWidget(QWidget * parent) : QListWidget(parent)
{
}

void mrw::ui::RouteListWidget::collect(std::vector<WidgetRoute *> & routes) const
{
	// Collect active routes.
	for (int index = 0; index < count(); index++)
	{
		QListWidgetItem * element = item(index);
		WidgetRoute   *   route   = element->data(WidgetRoute::USER_ROLE).value<WidgetRoute *>();

		routes.push_back(route);
	}
}

WidgetRoute * mrw::ui::RouteListWidget::selected() const
{
	for (QListWidgetItem * item : selectedItems())
	{
		return item->data(WidgetRoute::USER_ROLE).value<WidgetRoute *>();
	}
	return nullptr;
}
