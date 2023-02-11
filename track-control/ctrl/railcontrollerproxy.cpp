//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QPoint>

#include <can/commands.h>
#include <ctrl/railcontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

RailControllerProxy::RailControllerProxy(
	Rail   *  referenced_rail,
	QObject * parent) :
	RailController(parent),
	rail(referenced_rail)
{
}

QString RailControllerProxy::name() const
{
	return rail->partName();
}

float RailControllerProxy::extensions() const
{
	return rail->extension();
}

float RailControllerProxy::lines() const
{
	return rail->lines();
}

bool RailControllerProxy::isDirection() const
{
	return !rail->aIsDir() == region()->direction();
}

bool RailControllerProxy::isExpandable() const
{
	return true;
}

Position * RailControllerProxy::position() const
{
	return rail;
}

SectionState RailControllerProxy::state() const
{
	if (rail->reserved())
	{
		return section()->occupation() ? SectionState::OCCUPIED : section()->state();
	}
	else
	{
		if (section()->anyReserved())
		{
			return SectionState::FREE;
		}
		else
		{
			return section()->occupation() ? SectionState::OCCUPIED : section()->state();
		}
	}
}

Device::LockState RailControllerProxy::lock() const
{
	return section()->lock();
}

Position::Bending RailControllerProxy::bending() const
{
	return rail->bending();
}

bool RailControllerProxy::aEnds() const
{
	return rail->advance(rail->aIsDir()).empty();
}

bool RailControllerProxy::bEnds() const
{
	return rail->advance(!rail->aIsDir()).empty();
}

RailPart * RailControllerProxy::railPart() const
{
	return rail;
}
