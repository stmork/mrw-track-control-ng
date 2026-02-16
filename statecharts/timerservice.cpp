//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <statecharts/timerservice.h>
#include <util/method.h>

using namespace mrw::statechart;
using namespace mrw::util;
using namespace sc::timer;

using Qt::TimerType::PreciseTimer;
using Qt::TimerType::CoarseTimer;

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
	QTimer  *  timer          = getTimer(statemachine, event);
	const bool high_precision = (time_ms % 1000) != 0;

	// armor the timer
	timer->setTimerType(high_precision ? PreciseTimer : CoarseTimer);
	timer->setInterval(time_ms);
	timer->setSingleShot(!is_periodic);
	timer->start();
}

void TimerService::unsetTimer(
	std::shared_ptr<TimedInterface> statemachine,
	sc::eventid                     event)
{
	QTimer * timer = this->getTimer(statemachine, event);

	timer->stop();
}

void TimerService::unsetTimerRaw(
	TimedInterface * statemachine,
	sc::eventid      event)
{
	TimerKey key{ statemachine, event };

	if (chart_map.contains(key))
	{
		QTimer * timer = chart_map[key];

		Q_ASSERT(timer != nullptr);
		timer->stop();
		chart_map.remove(key);
		delete timer;
	}
}

QTimer * TimerService::getTimer(
	std::shared_ptr<TimedInterface> & statemachine,
	sc::eventid                       event)
{
	Q_ASSERT(statemachine);

	TimerKey   key{ statemachine.get(), event };
	QTimer  *  timer = nullptr;

	if (chart_map.contains(key))
	{
		timer = chart_map[key];
	}
	else
	{
		timer = new QTimer(this);

		chart_map.insert(key, timer);
		connect(timer, &QTimer::timeout, [statemachine, event]()
		{
			statemachine->raiseTimeEvent(event);
		});
	}
	Q_ASSERT(timer != nullptr);
	return timer;
}
