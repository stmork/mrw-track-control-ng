//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <util/method.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>

#include "ctrl/signalproxy.h"
#include "ctrl/signalcontrollerproxy.h"

using namespace mrw::util;
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
	setTimerService(TimerService::instance());
	setOperationCallback(*this);

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
		qCWarning(log).noquote() << "No controller for signal" << *input << "configured!";
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

		message.append(std::underlying_type_t<SignalAspect>(signal->aspect()));
		ControllerRegistry::can()->write(message);
	}
}

void SignalProxy::dump()
{
	__METHOD__;

	if (signal != nullptr)
	{
		qCDebug(log).noquote() << *signal;
	}
}

bool SignalProxy::process(Signal * device, const MrwMessage & message)
{
	Q_ASSERT(device != nullptr);

	if (device == signal)
	{
#if 0
		qCDebug(log).noquote() << message << "  (signal)" << signal->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");
#else
		qCDebug(log).noquote() << message << "  (signal)" << device->toString();
#endif

		switch (message.response())
		{
		case Response::MSG_QUEUED:
			QMetaObject::invokeMethod(this, &SignalStatechart::queued, Qt::QueuedConnection);
			return true;

		case Response::MSG_OK:
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
			qCCritical(log).noquote() << "Error turning" << signal->toString();
			QMetaObject::invokeMethod(this, &SignalStatechart::failed, Qt::QueuedConnection);
			return true;
		}
	}
	return false;
}

bool SignalProxy::setAspect(const SignalAspect aspect)
{
	Q_ASSERT(signal != nullptr);

	return signal->setAspect(aspect);
}

/*************************************************************************
**                                                                      **
**       Signal proxy for main signals                                  **
**                                                                      **
*************************************************************************/

MainProxy::MainProxy() : SelfPointer<MainProxy>(this)
{
}

bool MainProxy::prepare()
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalAspect state = SignalAspect::SIGNAL_OFF;

	switch (Symbol(getSymbol()))
	{
	default:
		state = SignalAspect::SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = SignalAspect::SIGNAL_HP0;
		break;

	case Symbol::GO:
		state = curved < SLOW_CURVED_LIMIT ? SignalAspect::SIGNAL_HP1 : SignalAspect::SIGNAL_HP2;
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

DistantProxy::DistantProxy() : SelfPointer<DistantProxy>(this)
{
}

void DistantProxy::start(Signal * input, Signal * combined)
{
	combined_signal = combined;
	SignalProxy::start(input);
}

bool DistantProxy::prepare()
{
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalAspect state = SignalAspect::SIGNAL_OFF;

	switch (Symbol(getSymbol()))
	{
	case Symbol::GO:
		if (main_controller != nullptr)
		{
			const auto distant_aspect =
				std::underlying_type_t<SignalAspect>(main_controller->mainSignal()->aspect()) +
				std::underlying_type_t<SignalAspect>(SignalAspect::SIGNAL_MAIN_DISTANT_OFFSET);

			state = SignalAspect(distant_aspect);
		}
		break;

	case Symbol::STOP:
		if ((main_controller != nullptr) || (combined_signal == nullptr))
		{
			state = SignalAspect::SIGNAL_VR0;
		}
		break;

	case Symbol::OFF:
		state = SignalAspect::SIGNAL_OFF;
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

ShuntProxy::ShuntProxy() : SelfPointer<ShuntProxy>(this)
{
}

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

	SignalAspect state = SignalAspect::SIGNAL_OFF;

	switch (Symbol(getSymbol()))
	{
	default:
		state = SignalAspect::SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = isCombined() ? SignalAspect::SIGNAL_HP0 : SignalAspect::SIGNAL_SH0;
		break;

	case Symbol::GO:
		state = SignalAspect::SIGNAL_SH1;
		break;
	}

	return setAspect(state);
}
