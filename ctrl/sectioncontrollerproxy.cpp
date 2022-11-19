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
	mrw::model::Section * parent_section,
	QObject       *       parent) :
	SectionController(parent),
	section(parent_section)
{
}

QPoint SectionControllerProxy::point() const
{
	return QPoint(0, 0);
}

QString SectionControllerProxy::name() const
{
	return section->name();
}
