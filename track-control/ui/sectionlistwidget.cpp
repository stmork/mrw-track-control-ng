//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QListWidgetItem>

#include <ui/sectionlistwidget.h>
#include <ui/controllerwidget.h>
#include <ctrl/basecontroller.h>
#include <model/position.h>

using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

SectionListWidget::SectionListWidget(QWidget * parent) : QListWidget(parent)
{
}

void SectionListWidget::traverse(ControllerCallback callback)
{
	for (int i = 0; i < count(); i++)
	{
		QListWidgetItem * element    = item(i);
		BaseController  * controller = element->data(ControllerWidget::USER_ROLE).value<BaseController *>();

		callback(controller);
	}
}

void SectionListWidget::traverse(PositionCallback callback)
{
	traverse([&](BaseController * controller)
	{
		Position * position = controller->position();

		if (position != nullptr)
		{
			callback(controller, position);
		}
	});
}

void mrw::ui::SectionListWidget::collect(std::vector<RailPartInfo *> & rails)
{
	traverse([&](BaseController * controller)
	{
		RailPartInfo * info = dynamic_cast<RailPartInfo *>(controller);

		if (info != nullptr)
		{
			rails.push_back(info);
		}
	});
}
