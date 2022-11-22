//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <model/region.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

DoubleCrossSwitchControllerProxy::DoubleCrossSwitchControllerProxy(
	DoubleCrossSwitch * new_part,
	QObject      *      parent) :
	DoubleCrossSwitchController(parent),
	part(new_part)
{
}

bool DoubleCrossSwitchControllerProxy::isDirection() const
{
	return part->aIsDir() == part->section()->region()->direction();
}

QString DoubleCrossSwitchControllerProxy::name() const
{
	return part->name();
}
