//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

#include <ctrl/controllerregistrand.h>

using namespace mrw::can;
using namespace mrw::ctrl;

ControllerRegistrand::ControllerRegistrand()
{
}

bool ControllerRegistrand::process(const MrwMessage & message)
{
	qInfo().noquote() << "Unprocessed message:" << message;

	return false;
}
