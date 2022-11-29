//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <ctrl/controllerregistrand.h>

using namespace mrw::ctrl;

ControllerRegistrand::ControllerRegistrand()
{
}

bool ControllerRegistrand::process(const mrw::can::MrwMessage & message)
{
	qInfo().noquote() << "Unprocessed message:" << message;

	return false;
}
