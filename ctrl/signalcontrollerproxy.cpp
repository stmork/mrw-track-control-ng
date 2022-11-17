//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "signalcontrollerproxy.h"

using namespace mrw::ctrl;
using namespace mrw::model;

SignalControllerProxy::SignalControllerProxy(QObject * parent) :
	SignalController(parent)
{
}

void SignalControllerProxy::setSignal(Signal * new_signal)
{
	Q_ASSERT(signal == nullptr);

	signal = new_signal;
}

bool SignalControllerProxy::isDirection() const
{
	return signal->direction();
}

bool SignalControllerProxy::hasShunting() const
{
	// TODO: Implement in mrw::model::Signal and mrw::model::Section.
	return false;
}

bool SignalControllerProxy::hasDistant() const
{
	return signal->pair() != nullptr;
}

bool SignalControllerProxy::hasMain() const
{
	return signal->type() == Signal::MAIN_SIGNAL;
}

QString SignalControllerProxy::name() const
{
	return signal->partName();
}
