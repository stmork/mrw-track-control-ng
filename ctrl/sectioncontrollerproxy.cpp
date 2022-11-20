//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPoint>

#include <can/commands.h>
#include <ctrl/sectioncontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

SectionControllerProxy::SectionControllerProxy(
	Section * parent_section,
	QObject * parent) :
	SectionController(parent),
	section(parent_section)
{
	std::vector<Rail *> rails;

	section->parts<Rail>(rails);

	Q_ASSERT(rails.size() > 0);
	rail = rails[0];
}

QPoint SectionControllerProxy::point() const
{
	return QPoint(0, 0);
}

QString SectionControllerProxy::name() const
{
	return section->name();
}
