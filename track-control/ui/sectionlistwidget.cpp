//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <unordered_set>
#include <vector>

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

void SectionListWidget::traverse(ControllerCallback callback) const
{
	for (int i = 0; i < count(); i++)
	{
		BaseController * base_controller = controller(i);

		callback(base_controller);
	}
}

void SectionListWidget::traverse(RailPartInfoCallback callback) const
{
	for (int i = 0; i < count(); i++)
	{
		BaseController * ctrl = controller(i);
		RailPartInfo  *  info = dynamic_cast<RailPartInfo *>(ctrl);

		if (info != nullptr)
		{
			callback(info);
		}
	}
}

void SectionListWidget::traverse(PositionCallback callback) const
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

void SectionListWidget::collect(std::vector<RailPartInfo *> & rails) const
{
	traverse([&](RailPartInfo * info)
	{
		rails.emplace_back(info);
	});
}

bool SectionListWidget::isSameRegion() const
{
	std::unordered_set<Region *> regions;
	std::vector<RailPartInfo *>  infos;

	collect(infos);
	for (RailPartInfo * info : infos)
	{
		regions.emplace(info->region());
	}

	return regions.size() == 1;
}

BaseController * SectionListWidget::controller(const int index) const
{
	QListWidgetItem * element = item(index);

	return element->data(ControllerWidget::USER_ROLE).value<BaseController *>();
}
