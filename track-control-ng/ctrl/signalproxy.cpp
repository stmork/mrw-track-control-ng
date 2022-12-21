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

void SignalProxy::idle()
{
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
		state = curved_count < 2 ? SIGNAL_HP1 : SIGNAL_HP2;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);
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

void DistantProxy::send(sc::integer symbol)
{
	Q_UNUSED(symbol)
	__METHOD__;

	Q_ASSERT(signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(signal->device()->command(SETSGN));

	if (main_signal != nullptr)
	{
		Device * device = dynamic_cast<Device *>(main_signal);
		SignalControllerProxy * proxy = ControllerRegistry::instance().find<SignalControllerProxy>(device);

		switch (proxy->main())
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
	}
	else
	{
		state = SIGNAL_OFF;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);
}

void DistantProxy::setMainSignal(Signal * master_signal)
{
	main_signal = master_signal;
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
