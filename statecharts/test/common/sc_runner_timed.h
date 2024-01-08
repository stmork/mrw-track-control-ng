/*
 * Timer Service for SCTUnit
 */

#ifndef SC_UNIT_TIMER_SERVICE_H_
#define SC_UNIT_TIMER_SERVICE_H_


#include "sc_types.h"
#include "sc_eventdriven.h"
#include "sc_timer.h"
#include "sc_timer_service.h"

class TimedSctUnitRunner : public sc::timer::TimerServiceInterface
{
public:
	TimedSctUnitRunner(size_t maximal_parallel_time_events_);
	virtual ~TimedSctUnitRunner()
	{
		delete [] tasks;
	};
	void proceed_time(sc::time time_ms);
	void proceed_cycles(sc::integer cycles);
	virtual void setTimer(sc::timer::TimedInterface * statemachine_, sc::eventid event, sc::time time_ms, bool isPeriodic) override;
	virtual void unsetTimer(sc::timer::TimedInterface * statemachine_, sc::eventid event) noexcept override;
	virtual void cancel() noexcept;
private:
	sc::timer::TimerTask * tasks;
	sc::timer::TimerService timerServiceImplementation;
};
#endif /* SC_UNIT_TIMER_SERVICE_H_ */

