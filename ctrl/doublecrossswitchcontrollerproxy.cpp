//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;

DoubleCrossSwitchControllerProxy::DoubleCrossSwitchControllerProxy(QObject * parent) :
	DoubleCrossSwitchController(parent)
{
}

void DoubleCrossSwitchControllerProxy::setSwitch(mrw::model::DoubleCrossSwitch * new_part)
{
	Q_ASSERT(part == nullptr);

	part = new_part;
}

bool DoubleCrossSwitchControllerProxy::isDirection() const
{
	return !part->aIsDir();
}

QString DoubleCrossSwitchControllerProxy::name() const
{
	return part->name();
}
