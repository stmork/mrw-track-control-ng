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
	Rail   *  referenced_rail,
	QObject * parent) :
	SectionController(parent),
	section(parent_section),
	rail(referenced_rail)
{
}

QString SectionControllerProxy::name() const
{
	return section->name();
}
