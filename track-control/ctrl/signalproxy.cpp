//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

using Symbol = Signal::Symbol;

/*************************************************************************
**                                                                      **
**       Signal proxy base class                                        **
**                                                                      **
*************************************************************************/

SignalProxy::SignalProxy() : SignalStatechart(nullptr)
{
	setTimerService(&TimerService::instance());
	setOperationCallback(this);

	Q_ASSERT(check());
}

SignalProxy::~SignalProxy()
{
	exit();
}

void SignalProxy::start(Signal * input)
{
	Device * device = dynamic_cast<Device *>(input);

	if ((device != nullptr) && (device->controller() == nullptr))
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

	Device * device = dynamic_cast<Device *>(signal);

	Q_ASSERT(device != nullptr);

	if (device->controller() != nullptr)
	{
		MrwMessage  message(device->command(SETSGN));

		message.append(signal->aspect());
		ControllerRegistry::can()->write(message);
	}
}

void SignalProxy::dump()
{
	__METHOD__;

	if (signal != nullptr)
	{
		qDebug().noquote() << *signal;
	}
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

bool SignalProxy::setAspect(const SignalAspect aspect)
{
	Q_ASSERT(signal != nullptr);

	modified = signal->aspect() != aspect;
	signal->setAspect(aspect);

	return modified;
}

/*************************************************************************
**                                                                      **
**       Signal proxy for main signals                                  **
**                                                                      **
*************************************************************************/

bool MainProxy::prepare()
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalAspect state = SIGNAL_OFF;

	switch (getSymbol())
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = SIGNAL_HP0;
		break;

	case Symbol::GO:
		state = curved < SLOW_CURVED_LIMIT ? SIGNAL_HP1 : SIGNAL_HP2;
		break;
	}

	return setAspect(state);
}

void MainProxy::setCurveCount(const size_t count)
{
	curved = count;
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

bool DistantProxy::prepare()
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalAspect state      = SIGNAL_OFF;

	switch (getSymbol())
	{
	case Symbol::GO:
		if (main_controller != nullptr)
		{
			const uint8_t main_state = main_controller->mainSignal()->aspect();

			state = static_cast<SignalAspect>(main_state + SIGNAL_MAIN_DISTANT_OFFSET);
		}
		break;

	case Symbol::STOP:
		if ((main_controller != nullptr) || (combined_signal == nullptr))
		{
			state = SIGNAL_VR0;
		}
		break;

	case Symbol::OFF:
		state = SIGNAL_OFF;
		break;
	}

	return setAspect(state);
}

Symbol DistantProxy::getPreparedSymbol() const
{
	Symbol result = (Symbol)getSymbol();

	if ((result == Symbol::GO) && (main_controller != nullptr))
	{
		result = main_controller->main();
	}
	return result;
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

bool ShuntProxy::prepare()
{
	__METHOD__;

	SignalAspect state = SIGNAL_OFF;

	switch (getSymbol())
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = isCombined() ? SIGNAL_HP0 : SIGNAL_SH0;
		break;

	case Symbol::GO:
		state = SIGNAL_SH1;
		break;
	}

	return setAspect(state);
}
