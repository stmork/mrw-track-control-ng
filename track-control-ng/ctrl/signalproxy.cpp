//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <util/method.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>

#include "ctrl/signalproxy.h"
#include "ctrl/signalcontrollerproxy.h"

using namespace mrw::statechart;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

/*************************************************************************
**                                                                      **
**       Signal proxy base class                                        **
**                                                                      **
*************************************************************************/

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
	if ((input != nullptr) && (input->device()->controller() == nullptr))
	{
		// Controller for signal not configured.
		qWarning().noquote() << "No controller for signal" << *input << "configured!";
		signal = nullptr;
	}
	else
	{
		signal = input;
	}
	enter();
}

bool SignalProxy::hasSignal()
{
	return signal != nullptr;
}

void SignalProxy::send()
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	MrwMessage  message(signal->device()->command(SETSGN));

	message.append(signal->state());
	ControllerRegistry::can()->write(message);
}

void SignalProxy::prepare(sc::integer symbol)
{
	prepare(static_cast<Signal::Symbol>(symbol));
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
			QMetaObject::invokeMethod(this, &SignalStatechart::queued, Qt::QueuedConnection);
			return true;

		case MSG_OK:
			switch (message.command())
			{
			case SETSGN:
				QMetaObject::invokeMethod(this, &SignalStatechart::ok, Qt::QueuedConnection);
				return true;

			default:
				// Intentionally do nothing.
				break;
			}
			break;

		default:
			qCritical().noquote() << "Error turning" << signal->toString();
			QMetaObject::invokeMethod(this, &SignalStatechart::failed, Qt::QueuedConnection);
			return true;
		}
	}
	return false;
}

/*************************************************************************
**                                                                      **
**       Signal proxy for main signals                                  **
**                                                                      **
*************************************************************************/

void MainProxy::prepare(Signal::Symbol symbol)
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalState state = SIGNAL_OFF;

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Signal::Symbol::STOP:
		state = SIGNAL_HP0;
		break;

	case Signal::Symbol::GO:
		state = curved_count < 2 ? SIGNAL_HP1 : SIGNAL_HP2;
		break;
	}
	signal->setState(state);
}

void MainProxy::setCurved(const size_t count)
{
	curved_count = count;
}

/*************************************************************************
**                                                                      **
**       Signal proxy for distant                                       **
**                                                                      **
*************************************************************************/

void DistantProxy::start(Signal * input, Signal * combined)
{
	combined_signal = combined;
	SignalProxy::start(input);
}

void DistantProxy::prepare(Signal::Symbol symbol)
{
	__METHOD__;

	Q_UNUSED(symbol)
	Q_ASSERT(signal != nullptr);

	SignalState state      = SIGNAL_OFF;
	uint8_t     main_state = SIGNAL_OFF;

	if (main_controller != nullptr)
	{
		switch (symbol)
		{
		case Signal::Symbol::GO:
			main_state = main_controller->mainSignal()->state();
			state      = static_cast<SignalState>(main_state + SIGNAL_MAIN_DISTANT_OFFSET);
			break;

		case Signal::Symbol::STOP:
			state = SIGNAL_VR0;
			break;

		case Signal::Symbol::OFF:
			state = SIGNAL_OFF;
			break;
		}

	}
	else
	{
		state = SIGNAL_OFF;
	}
	signal->setState(state);
}

SignalControllerProxy * DistantProxy::mainController() const
{
	return main_controller;
}

void DistantProxy::setMainController(SignalControllerProxy * proxy)
{
	main_controller = proxy;
}

/*************************************************************************
**                                                                      **
**       Signal proxy for shunting                                      **
**                                                                      **
*************************************************************************/

void ShuntProxy::start(Signal * input, Signal * combined)
{
	main_signal = combined;
	SignalProxy::start(input);
}

bool ShuntProxy::isCombined()
{
	return hasSignal() && (signal == main_signal);
}

void ShuntProxy::prepare(mrw::model::Signal::Symbol symbol)
{
	SignalState state = SIGNAL_OFF;

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
	}
	signal->setState(state);
}
