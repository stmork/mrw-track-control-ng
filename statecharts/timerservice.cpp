//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <statecharts/timerservice.h>
#include <util/method.h>

using namespace mrw::statechart;
using namespace mrw::util;
using namespace sc::timer;

using Qt::TimerType::PreciseTimer;
using Qt::TimerType::CoarseTimer;

SCTimer::SCTimer(QObject * parent, TimedInterface * statemachine, const sc::eventid new_id) :
	QTimer(parent),
	machine(statemachine),
	event_id(new_id)
{
	Q_ASSERT(statemachine != nullptr);

	connect(this, &QTimer::timeout, this, &SCTimer::trigger);
}

void SCTimer::trigger()
{
	machine->raiseTimeEvent(event_id);
}

TimerService::TimerService() :
	QObject(nullptr),
	self(this, &Method::noop<TimerService>)
{
}

void TimerService::setTimer(
	std::shared_ptr<TimedInterface> statemachine,
	sc::eventid                     event,
	sc::time                        time_ms,
	bool                            is_periodic)
{
	SCTimer  * timer          = getTimer(statemachine, event);
	const bool high_precision = (time_ms % 1000) != 0;

	// amor the timer
	timer->setTimerType(high_precision ? PreciseTimer : CoarseTimer);
	timer->setInterval(time_ms);
	timer->setSingleShot(!is_periodic);
	timer->start();
}

void TimerService::unsetTimer(
	std::shared_ptr<sc::timer::TimedInterface> statemachine,
	sc::eventid                                event)
{
	SCTimer * timer = this->getTimer(statemachine, event);

	if (timer != nullptr)
	{
		timer->stop();
	}
}

SCTimer * TimerService::getTimer(
	std::shared_ptr<sc::timer::TimedInterface> & statemachine,
	sc::eventid                                  event)
{
	TimerKey  key(statemachine.get(), event);
	SCTimer * timer;

	if (chart_map.contains(key))
	{
		timer = chart_map[key];
	}
	else
	{
		timer = new SCTimer(this, statemachine.get(), event);

		chart_map.insert(key, timer);
	}
	return timer;
}
