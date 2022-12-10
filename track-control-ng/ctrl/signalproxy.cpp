//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <util/method.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>

#include "signalproxy.h"

using namespace mrw::statechart;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

SignalProxy::SignalProxy() : SignalStatechart(nullptr)
{
	setOperationCallback(this);
	setTimerService(&TimerService::instance());
}

SignalProxy::~SignalProxy()
{
	exit();
}

void SignalProxy::start(Signal * input)
{
	signal = input;
	enter();
}

bool SignalProxy::hasSignal()
{
	return signal != nullptr;
}

bool SignalProxy::process(Signal * device, const MrwMessage & message)
{
	Q_ASSERT(device != nullptr);

	if (device == signal)
	{
#if 0
		qDebug().noquote() << message << "  (signal)" << signal->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");
#else
		qDebug().noquote() << message << "  (signal)" << device->toString();
#endif

		switch (message.response())
		{
		case MSG_QUEUED:
			queued();
			return true;

		case MSG_OK:
			switch (message.command())
			{
			case SETSGN:
				ok();
				return true;

			default:
				// Intentionally do nothing.
				break;
			}
			break;

		default:
			qCritical().noquote() << "Error turning" << signal->toString();
			failed();
			return true;
		}
	}
	return false;
}

void MainProxy::send(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Signal::Symbol::STOP:
		state = SIGNAL_HP0;
		break;

	case Signal::Symbol::GO:
		state = SIGNAL_HP1;
		break;

	case Signal::Symbol::SLOW:
		state = SIGNAL_HP2;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);
}

void DistantProxy::send(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Signal::Symbol::STOP:
		state = SIGNAL_VR0;
		break;

	case Signal::Symbol::GO:
		state = SIGNAL_VR1;
		break;

	case Signal::Symbol::SLOW:
		state = SIGNAL_VR2;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);
}

void ShuntProxy::send(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Signal::Symbol::STOP:
		state = SIGNAL_SH0;
		break;

	case Signal::Symbol::GO:
		state = SIGNAL_SH1;
		break;

	case Signal::Symbol::SLOW:
		state = SIGNAL_SH1;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);
}
