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

QString DoubleCrossSwitchControllerProxy::name() const
{
	return part->name();
}

bool DoubleCrossSwitchControllerProxy::isDirection() const
{
	return part->aIsDir() != part->section()->region()->direction();
}

Position * DoubleCrossSwitchControllerProxy::position() const
{
	return part;
}

SectionState DoubleCrossSwitchControllerProxy::state() const
{
	return part->section()->state();
}

Device::LockState DoubleCrossSwitchControllerProxy::lock() const
{
	return part->lock();
}

Position::Bending DoubleCrossSwitchControllerProxy::bending() const
{
	return part->bending();
}

DoubleCrossSwitch::State DoubleCrossSwitchControllerProxy::switchState() const
{
	return part->state();
}
