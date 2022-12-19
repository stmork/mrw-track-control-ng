/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#include "SignalControllerStatechart.h"

/*! \file
Implementation of the state machine 'SignalControllerStatechart'
*/

namespace mrw {
namespace statechart {

const sc::integer SignalControllerStatechart::OFF = -1;
const sc::integer SignalControllerStatechart::STOP = 0;
const sc::integer SignalControllerStatechart::GO = 1;
const sc::integer SignalControllerStatechart::SLOW = 2;



SignalControllerStatechart::SignalControllerStatechart(QObject *parent) :
	QObject(parent),
	timeout(5000),
	timerService(nullptr),
	ifaceOperationCallback(nullptr),
	isExecuting(false),
	stateConfVectorPosition(0),
	stateConfVectorChanged(false),
	start_raised(false),
	clear_raised(false),
	started_raised(false),
	failed_raised(false),
	enable_raised(false),
	disable_raised(false),
	completedMain_raised(false),
	completedDistant_raised(false),
	completedShunt_raised(false),
	turnMain_value(0),
	turnDistant_value(0),
	turnShunt_value(0)
{
	for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
		stateConfVector[state_vec_pos] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	
	clearInEvents();
}

SignalControllerStatechart::~SignalControllerStatechart()
{
}



mrw::statechart::SignalControllerStatechart::EventInstance* SignalControllerStatechart::getNextEvent()
{
	mrw::statechart::SignalControllerStatechart::EventInstance* nextEvent = 0;

	if(!incomingEventQueue.empty()) {
		nextEvent = incomingEventQueue.front();
		incomingEventQueue.pop_front();
	}
	
	return nextEvent;
	
}					


void SignalControllerStatechart::dispatchEvent(mrw::statechart::SignalControllerStatechart::EventInstance * event)
{
	if(event == nullptr) {
		return;
	}
								
	switch(event->eventId)
	{
		case mrw::statechart::SignalControllerStatechart::Event::start:
		{
			start_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::clear:
		{
			clear_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::started:
		{
			started_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::failed:
		{
			failed_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::enable:
		{
			enable_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::disable:
		{
			disable_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::completedMain:
		{
			completedMain_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::completedDistant:
		{
			completedDistant_raised = true;
			break;
		}
		case mrw::statechart::SignalControllerStatechart::Event::completedShunt:
		{
			completedShunt_raised = true;
			break;
		}
		
		
		case mrw::statechart::SignalControllerStatechart::Event::_te0_main_region_Init_:
		case mrw::statechart::SignalControllerStatechart::Event::_te1_main_region_Operating_Processing_Locked_Processing_Go_:
		{
			timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SignalControllerStatechart::Event::_te0_main_region_Init_)] = true;
			break;
		}
		default:
			/* do nothing */
			break;
	}
	delete event;
}


void mrw::statechart::SignalControllerStatechart::start() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::start));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::clear() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::clear));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::started() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::started));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::failed() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::failed));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::enable() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::enable));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::disable() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::disable));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::completedMain() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedMain));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::completedDistant() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedDistant));
	runCycle();
}


void mrw::statechart::SignalControllerStatechart::completedShunt() {
	incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedShunt));
	runCycle();
}



bool SignalControllerStatechart::isActive() const
{
	return stateConfVector[0] != mrw::statechart::SignalControllerStatechart::State::NO_STATE||stateConfVector[1] != mrw::statechart::SignalControllerStatechart::State::NO_STATE||stateConfVector[2] != mrw::statechart::SignalControllerStatechart::State::NO_STATE;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
bool SignalControllerStatechart::isFinal() const
{
   return false;}

bool SignalControllerStatechart::check() const {
	if(timerService == nullptr) {
		return false;
	}
	if (this->ifaceOperationCallback == nullptr) {
		return false;
	}
	return true;
}


void SignalControllerStatechart::setTimerService(sc::timer::TimerServiceInterface* timerService_)
{
	this->timerService = timerService_;
}

sc::timer::TimerServiceInterface* SignalControllerStatechart::getTimerService()
{
	return timerService;
}

sc::integer SignalControllerStatechart::getNumberOfParallelTimeEvents() {
	return parallelTimeEventsCount;
}

void SignalControllerStatechart::raiseTimeEvent(sc::eventid evid)
{
	if (evid < timeEventsCount)
	{
		incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SignalControllerStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SignalControllerStatechart::Event::_te0_main_region_Init_))));
		runCycle();
	}
}


bool SignalControllerStatechart::isStateActive(State state) const
{
	switch (state)
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start :
		{
			return  (stateConfVector[scvi_main_region_Wait_for_Start] == mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init :
		{
			return  (stateConfVector[scvi_main_region_Init] >= mrw::statechart::SignalControllerStatechart::State::main_region_Init && stateConfVector[scvi_main_region_Init] <= mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning] >= mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning && stateConfVector[scvi_main_region_Init_Init_process_Turning] <= mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_main_Turn] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_main_Completed] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_distant_Turn] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_distant_Completed] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_shunt_Turn] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed :
		{
			return  (stateConfVector[scvi_main_region_Init_Init_process_Turning_shunt_Completed] == mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating :
		{
			return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
		{
			return  (stateConfVector[scvi_main_region_Operating_Processing_Unlocked] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked :
		{
			return  (stateConfVector[scvi_main_region_Operating_Processing_Locked] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked && stateConfVector[scvi_main_region_Operating_Processing_Locked] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go :
		{
			return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Processing_Go] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle :
		{
			return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Processing_Idle] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop :
		{
			return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Processing_Stop] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Failed :
		{
			return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::SignalControllerStatechart::State::main_region_Failed);
			break;
		}
		default:
		{
			/* State is not active*/
			return false;
			break;
		}
	}
}

sc::integer SignalControllerStatechart::getTimeout() const
{
	return timeout;
}

void SignalControllerStatechart::setTimeout(sc::integer timeout_)
{
	this->timeout = timeout_;
}

sc::integer SignalControllerStatechart::getOFF() 
{
	return OFF;
}

sc::integer SignalControllerStatechart::getSTOP() 
{
	return STOP;
}

sc::integer SignalControllerStatechart::getGO() 
{
	return GO;
}

sc::integer SignalControllerStatechart::getSLOW() 
{
	return SLOW;
}

void SignalControllerStatechart::setOperationCallback(OperationCallback* operationCallback)
{
	ifaceOperationCallback = operationCallback;
}

// implementations of all internal functions
/* Entry action for state 'Init'. */
void SignalControllerStatechart::enact_main_region_Init()
{
	/* Entry action for state 'Init'. */
	timerService->setTimer(this, 0, timeout, false);
	ifaceOperationCallback->inc();
}

/* Entry action for state 'Turn'. */
void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_main_Turn()
{
	/* Entry action for state 'Turn'. */
	turnMain_value = SignalControllerStatechart::STOP;
	emit turnMain(turnMain_value);
}

/* Entry action for state 'Turn'. */
void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_distant_Turn()
{
	/* Entry action for state 'Turn'. */
	if (!ifaceOperationCallback->hasMain())
	{ 
		turnDistant_value = SignalControllerStatechart::STOP;
		emit turnDistant(turnDistant_value);
	} 
	if (ifaceOperationCallback->hasMain())
	{ 
		turnDistant_value = SignalControllerStatechart::OFF;
		emit turnDistant(turnDistant_value);
	} 
}

/* Entry action for state 'Turn'. */
void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_shunt_Turn()
{
	/* Entry action for state 'Turn'. */
	if (!ifaceOperationCallback->isMain())
	{ 
		turnShunt_value = SignalControllerStatechart::STOP;
		emit turnShunt(turnShunt_value);
	} 
	if (ifaceOperationCallback->isMain())
	{ 
		completedShunt_raised = true;
	} 
}

/* Entry action for state 'Operating'. */
void SignalControllerStatechart::enact_main_region_Operating()
{
	/* Entry action for state 'Operating'. */
	started_raised = true;
}

/* Entry action for state 'Unlocked'. */
void SignalControllerStatechart::enact_main_region_Operating_Processing_Unlocked()
{
	/* Entry action for state 'Unlocked'. */
	ifaceOperationCallback->lock(false);
}

/* Entry action for state 'Locked'. */
void SignalControllerStatechart::enact_main_region_Operating_Processing_Locked()
{
	/* Entry action for state 'Locked'. */
	ifaceOperationCallback->lock(true);
}

/* Entry action for state 'Go'. */
void SignalControllerStatechart::enact_main_region_Operating_Processing_Locked_Processing_Go()
{
	/* Entry action for state 'Go'. */
	timerService->setTimer(this, 1, timeout, false);
	ifaceOperationCallback->inc();
	turnMain_value = SignalControllerStatechart::GO;
	emit turnMain(turnMain_value);
}

/* Entry action for state 'Stop'. */
void SignalControllerStatechart::enact_main_region_Operating_Processing_Locked_Processing_Stop()
{
	/* Entry action for state 'Stop'. */
	ifaceOperationCallback->inc();
	turnMain_value = SignalControllerStatechart::STOP;
	emit turnMain(turnMain_value);
}

/* Entry action for state 'Failed'. */
void SignalControllerStatechart::enact_main_region_Failed()
{
	/* Entry action for state 'Failed'. */
	ifaceOperationCallback->fail();
}

/* Exit action for state 'Init'. */
void SignalControllerStatechart::exact_main_region_Init()
{
	/* Exit action for state 'Init'. */
	timerService->unsetTimer(this, 0);
}

/* Exit action for state 'Go'. */
void SignalControllerStatechart::exact_main_region_Operating_Processing_Locked_Processing_Go()
{
	/* Exit action for state 'Go'. */
	timerService->unsetTimer(this, 1);
}

/* Exit action for state 'Failed'. */
void SignalControllerStatechart::exact_main_region_Failed()
{
	/* Exit action for state 'Failed'. */
	emit cleared();
}

/* 'default' enter sequence for state Wait for Start */
void SignalControllerStatechart::enseq_main_region_Wait_for_Start_default()
{
	/* 'default' enter sequence for state Wait for Start */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Init */
void SignalControllerStatechart::enseq_main_region_Init_default()
{
	/* 'default' enter sequence for state Init */
	enact_main_region_Init();
	enseq_main_region_Init_Init_process_default();
}

/* 'default' enter sequence for state Turn */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_main_Turn_default()
{
	/* 'default' enter sequence for state Turn */
	enact_main_region_Init_Init_process_Turning_main_Turn();
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Completed */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_main_Completed_default()
{
	/* 'default' enter sequence for state Completed */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Turn */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_distant_Turn_default()
{
	/* 'default' enter sequence for state Turn */
	enact_main_region_Init_Init_process_Turning_distant_Turn();
	stateConfVector[1] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn;
	stateConfVectorPosition = 1;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Completed */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_distant_Completed_default()
{
	/* 'default' enter sequence for state Completed */
	stateConfVector[1] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed;
	stateConfVectorPosition = 1;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Turn */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_shunt_Turn_default()
{
	/* 'default' enter sequence for state Turn */
	enact_main_region_Init_Init_process_Turning_shunt_Turn();
	stateConfVector[2] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn;
	stateConfVectorPosition = 2;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Completed */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_shunt_Completed_default()
{
	/* 'default' enter sequence for state Completed */
	stateConfVector[2] = mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed;
	stateConfVectorPosition = 2;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Operating */
void SignalControllerStatechart::enseq_main_region_Operating_default()
{
	/* 'default' enter sequence for state Operating */
	enact_main_region_Operating();
	enseq_main_region_Operating_Processing_default();
}

/* 'default' enter sequence for state Unlocked */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Unlocked_default()
{
	/* 'default' enter sequence for state Unlocked */
	enact_main_region_Operating_Processing_Unlocked();
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Locked */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Locked_default()
{
	/* 'default' enter sequence for state Locked */
	enact_main_region_Operating_Processing_Locked();
	enseq_main_region_Operating_Processing_Locked_Processing_default();
}

/* 'default' enter sequence for state Go */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Locked_Processing_Go_default()
{
	/* 'default' enter sequence for state Go */
	enact_main_region_Operating_Processing_Locked_Processing_Go();
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Idle */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Locked_Processing_Idle_default()
{
	/* 'default' enter sequence for state Idle */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Stop */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Locked_Processing_Stop_default()
{
	/* 'default' enter sequence for state Stop */
	enact_main_region_Operating_Processing_Locked_Processing_Stop();
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for state Failed */
void SignalControllerStatechart::enseq_main_region_Failed_default()
{
	/* 'default' enter sequence for state Failed */
	enact_main_region_Failed();
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Failed;
	stateConfVectorPosition = 0;
	stateConfVectorChanged = true;
}

/* 'default' enter sequence for region main region */
void SignalControllerStatechart::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* 'default' enter sequence for region Init process */
void SignalControllerStatechart::enseq_main_region_Init_Init_process_default()
{
	/* 'default' enter sequence for region Init process */
	react_main_region_Init_Init_process__entry_Default();
}

/* 'default' enter sequence for region Processing */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_default()
{
	/* 'default' enter sequence for region Processing */
	react_main_region_Operating_Processing__entry_Default();
}

/* 'default' enter sequence for region Processing */
void SignalControllerStatechart::enseq_main_region_Operating_Processing_Locked_Processing_default()
{
	/* 'default' enter sequence for region Processing */
	react_main_region_Operating_Processing_Locked_Processing__entry_Default();
}

/* Default exit sequence for state Wait for Start */
void SignalControllerStatechart::exseq_main_region_Wait_for_Start()
{
	/* Default exit sequence for state Wait for Start */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Init */
void SignalControllerStatechart::exseq_main_region_Init()
{
	/* Default exit sequence for state Init */
	exseq_main_region_Init_Init_process();
	exact_main_region_Init();
}

/* Default exit sequence for state Turning */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning()
{
	/* Default exit sequence for state Turning */
	exseq_main_region_Init_Init_process_Turning_main();
	exseq_main_region_Init_Init_process_Turning_distant();
	exseq_main_region_Init_Init_process_Turning_shunt();
}

/* Default exit sequence for state Turn */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_main_Turn()
{
	/* Default exit sequence for state Turn */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Completed */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_main_Completed()
{
	/* Default exit sequence for state Completed */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Turn */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_distant_Turn()
{
	/* Default exit sequence for state Turn */
	stateConfVector[1] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 1;
}

/* Default exit sequence for state Completed */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_distant_Completed()
{
	/* Default exit sequence for state Completed */
	stateConfVector[1] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 1;
}

/* Default exit sequence for state Turn */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_shunt_Turn()
{
	/* Default exit sequence for state Turn */
	stateConfVector[2] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 2;
}

/* Default exit sequence for state Completed */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_shunt_Completed()
{
	/* Default exit sequence for state Completed */
	stateConfVector[2] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 2;
}

/* Default exit sequence for state Operating */
void SignalControllerStatechart::exseq_main_region_Operating()
{
	/* Default exit sequence for state Operating */
	exseq_main_region_Operating_Processing();
}

/* Default exit sequence for state Unlocked */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Unlocked()
{
	/* Default exit sequence for state Unlocked */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Locked */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Locked()
{
	/* Default exit sequence for state Locked */
	exseq_main_region_Operating_Processing_Locked_Processing();
}

/* Default exit sequence for state Go */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Locked_Processing_Go()
{
	/* Default exit sequence for state Go */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
	exact_main_region_Operating_Processing_Locked_Processing_Go();
}

/* Default exit sequence for state Idle */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Locked_Processing_Idle()
{
	/* Default exit sequence for state Idle */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Stop */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Locked_Processing_Stop()
{
	/* Default exit sequence for state Stop */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Failed */
void SignalControllerStatechart::exseq_main_region_Failed()
{
	/* Default exit sequence for state Failed */
	stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
	stateConfVectorPosition = 0;
	exact_main_region_Failed();
}

/* Default exit sequence for region main region */
void SignalControllerStatechart::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start :
		{
			exseq_main_region_Wait_for_Start();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_main_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_main_Completed();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
		{
			exseq_main_region_Operating_Processing_Unlocked();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Go();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Idle();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Stop();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Failed :
		{
			exseq_main_region_Failed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region) at position 1... */
	switch(stateConfVector[ 1 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region) at position 2... */
	switch(stateConfVector[ 2 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Turn();
			exact_main_region_Init();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Completed();
			exact_main_region_Init();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region Init process */
void SignalControllerStatechart::exseq_main_region_Init_Init_process()
{
	/* Default exit sequence for region Init process */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_main_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_main_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process) at position 1... */
	switch(stateConfVector[ 1 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process) at position 2... */
	switch(stateConfVector[ 2 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region main */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_main()
{
	/* Default exit sequence for region main */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process.Turning.main) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_main_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_main_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region distant */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_distant()
{
	/* Default exit sequence for region distant */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process.Turning.distant) at position 1... */
	switch(stateConfVector[ 1 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_distant_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region shunt */
void SignalControllerStatechart::exseq_main_region_Init_Init_process_Turning_shunt()
{
	/* Default exit sequence for region shunt */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Init.Init_process.Turning.shunt) at position 2... */
	switch(stateConfVector[ 2 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Turn();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed :
		{
			exseq_main_region_Init_Init_process_Turning_shunt_Completed();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region Processing */
void SignalControllerStatechart::exseq_main_region_Operating_Processing()
{
	/* Default exit sequence for region Processing */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
		{
			exseq_main_region_Operating_Processing_Unlocked();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Go();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Idle();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Stop();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default exit sequence for region Processing */
void SignalControllerStatechart::exseq_main_region_Operating_Processing_Locked_Processing()
{
	/* Default exit sequence for region Processing */
	/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Locked.Processing) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Go();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Idle();
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop :
		{
			exseq_main_region_Operating_Processing_Locked_Processing_Stop();
			break;
		}
		default:
			/* do nothing */
			break;
	}
}

/* Default react sequence for initial entry  */
void SignalControllerStatechart::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Wait_for_Start_default();
}

/* Default react sequence for initial entry  */
void SignalControllerStatechart::react_main_region_Init_Init_process__entry_Default()
{
	/* Default react sequence for initial entry  */
	react_main_region_Init_Init_process__sync0();
}

/* Default react sequence for initial entry  */
void SignalControllerStatechart::react_main_region_Operating_Processing__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Operating_Processing_Unlocked_default();
}

/* Default react sequence for initial entry  */
void SignalControllerStatechart::react_main_region_Operating_Processing_Locked_Processing__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Operating_Processing_Locked_Processing_Go_default();
}

/* The reactions of state null. */
void SignalControllerStatechart::react_main_region_Init_Init_process__sync0()
{
	/* The reactions of state null. */
	enseq_main_region_Init_Init_process_Turning_main_Turn_default();
	enseq_main_region_Init_Init_process_Turning_distant_Turn_default();
	enseq_main_region_Init_Init_process_Turning_shunt_Turn_default();
}

/* The reactions of state null. */
void SignalControllerStatechart::react_main_region_Init_Init_process__sync1()
{
	/* The reactions of state null. */
	exseq_main_region_Init();
	ifaceOperationCallback->dec();
	enseq_main_region_Operating_default();
	react(0);
}

sc::integer SignalControllerStatechart::react(const sc::integer transitioned_before) {
	/* State machine reactions. */
	return transitioned_before;
}

sc::integer SignalControllerStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before) {
	/* The reactions of state Wait for Start. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (start_raised)
		{ 
			exseq_main_region_Wait_for_Start();
			enseq_main_region_Init_default();
			react(0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_react(const sc::integer transitioned_before) {
	/* The reactions of state Init. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (failed_raised)
		{ 
			exseq_main_region_Init();
			enseq_main_region_Failed_default();
			react(0);
			transitioned_after = 2;
		}  else
		{
			if (timeEvents[0])
			{ 
				exseq_main_region_Init();
				timeEvents[0] = false;
				enseq_main_region_Failed_default();
				react(0);
				transitioned_after = 2;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_react(const sc::integer transitioned_before) {
	/* The reactions of state Turning. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Init_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_main_Turn_react(const sc::integer transitioned_before) {
	/* The reactions of state Turn. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (completedMain_raised)
		{ 
			exseq_main_region_Init_Init_process_Turning_main_Turn();
			enseq_main_region_Init_Init_process_Turning_main_Completed_default();
			transitioned_after = 0;
		} 
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_main_Completed_react(const sc::integer transitioned_before) {
	/* The reactions of state Completed. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed)))
		{ 
			exseq_main_region_Init_Init_process_Turning();
			react_main_region_Init_Init_process__sync1();
			transitioned_after = 0;
		} 
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_distant_Turn_react(const sc::integer transitioned_before) {
	/* The reactions of state Turn. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (1))
	{ 
		if (completedDistant_raised)
		{ 
			exseq_main_region_Init_Init_process_Turning_distant_Turn();
			enseq_main_region_Init_Init_process_Turning_distant_Completed_default();
			transitioned_after = 1;
		} 
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_distant_Completed_react(const sc::integer transitioned_before) {
	/* The reactions of state Completed. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (1))
	{ 
		if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed)))
		{ 
			exseq_main_region_Init_Init_process_Turning();
			react_main_region_Init_Init_process__sync1();
			transitioned_after = 1;
		} 
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_shunt_Turn_react(const sc::integer transitioned_before) {
	/* The reactions of state Turn. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (2))
	{ 
		if (completedShunt_raised)
		{ 
			exseq_main_region_Init_Init_process_Turning_shunt_Turn();
			enseq_main_region_Init_Init_process_Turning_shunt_Completed_default();
			main_region_Init_Init_process_Turning_react(0);
			transitioned_after = 2;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Init_Init_process_Turning_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_shunt_Completed_react(const sc::integer transitioned_before) {
	/* The reactions of state Completed. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (2))
	{ 
		if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed)))
		{ 
			exseq_main_region_Init_Init_process_Turning();
			react_main_region_Init_Init_process__sync1();
			transitioned_after = 2;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Init_Init_process_Turning_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_react(const sc::integer transitioned_before) {
	/* The reactions of state Operating. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (failed_raised)
		{ 
			exseq_main_region_Operating();
			enseq_main_region_Failed_default();
			react(0);
			transitioned_after = 0;
		}  else
		{
			if (start_raised)
			{ 
				exseq_main_region_Operating();
				enseq_main_region_Init_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before) {
	/* The reactions of state Unlocked. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (enable_raised)
		{ 
			exseq_main_region_Operating_Processing_Unlocked();
			ifaceOperationCallback->prepare();
			enseq_main_region_Operating_Processing_Locked_default();
			main_region_Operating_react(0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Operating_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before) {
	/* The reactions of state Locked. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Operating_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_Processing_Locked_Processing_Go_react(const sc::integer transitioned_before) {
	/* The reactions of state Go. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (completedMain_raised)
		{ 
			exseq_main_region_Operating_Processing_Locked_Processing_Go();
			ifaceOperationCallback->dec();
			enseq_main_region_Operating_Processing_Locked_Processing_Idle_default();
			main_region_Operating_Processing_Locked_react(0);
			transitioned_after = 0;
		}  else
		{
			if (timeEvents[1])
			{ 
				exseq_main_region_Operating();
				timeEvents[1] = false;
				enseq_main_region_Failed_default();
				react(0);
				transitioned_after = 0;
			} 
		}
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_Processing_Locked_Processing_Idle_react(const sc::integer transitioned_before) {
	/* The reactions of state Idle. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (disable_raised)
		{ 
			exseq_main_region_Operating_Processing_Locked_Processing_Idle();
			enseq_main_region_Operating_Processing_Locked_Processing_Stop_default();
			main_region_Operating_Processing_Locked_react(0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Operating_Processing_Locked_Processing_Stop_react(const sc::integer transitioned_before) {
	/* The reactions of state Stop. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (completedMain_raised)
		{ 
			exseq_main_region_Operating_Processing_Locked();
			ifaceOperationCallback->dec();
			enseq_main_region_Operating_Processing_Unlocked_default();
			main_region_Operating_react(0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
	} 
	return transitioned_after;
}

sc::integer SignalControllerStatechart::main_region_Failed_react(const sc::integer transitioned_before) {
	/* The reactions of state Failed. */
	sc::integer transitioned_after = transitioned_before;
	if ((transitioned_after) < (0))
	{ 
		if (clear_raised)
		{ 
			exseq_main_region_Failed();
			enseq_main_region_Wait_for_Start_default();
			react(0);
			transitioned_after = 0;
		} 
	} 
	/* If no transition was taken then execute local reactions */
	if ((transitioned_after) == (transitioned_before))
	{ 
		transitioned_after = react(transitioned_before);
	} 
	return transitioned_after;
}

void SignalControllerStatechart::clearInEvents() {
	start_raised = false;
	clear_raised = false;
	started_raised = false;
	failed_raised = false;
	enable_raised = false;
	disable_raised = false;
	completedMain_raised = false;
	completedDistant_raised = false;
	completedShunt_raised = false;
	timeEvents[0] = false;
	timeEvents[1] = false;
}

void SignalControllerStatechart::microStep() {
	sc::integer transitioned = -1;
	stateConfVectorPosition = 0;
	switch(stateConfVector[ 0 ])
	{
		case mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start :
		{
			transitioned = main_region_Wait_for_Start_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn :
		{
			transitioned = main_region_Init_Init_process_Turning_main_Turn_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed :
		{
			transitioned = main_region_Init_Init_process_Turning_main_Completed_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
		{
			transitioned = main_region_Operating_Processing_Unlocked_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Go :
		{
			transitioned = main_region_Operating_Processing_Locked_Processing_Go_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Idle :
		{
			transitioned = main_region_Operating_Processing_Locked_Processing_Idle_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Locked_Processing_Stop :
		{
			transitioned = main_region_Operating_Processing_Locked_Processing_Stop_react(transitioned);
			break;
		}
		case mrw::statechart::SignalControllerStatechart::State::main_region_Failed :
		{
			transitioned = main_region_Failed_react(transitioned);
			break;
		}
		default:
			/* do nothing */
			break;
	}
	if ((stateConfVectorPosition) < (1))
	{ 
		switch(stateConfVector[ 1 ])
		{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn :
			{
				transitioned = main_region_Init_Init_process_Turning_distant_Turn_react(transitioned);
				break;
			}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed :
			{
				transitioned = main_region_Init_Init_process_Turning_distant_Completed_react(transitioned);
				break;
			}
			default:
				/* do nothing */
				break;
		}
	} 
	if ((stateConfVectorPosition) < (2))
	{ 
		switch(stateConfVector[ 2 ])
		{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn :
			{
				main_region_Init_Init_process_Turning_shunt_Turn_react(transitioned);
				break;
			}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed :
			{
				main_region_Init_Init_process_Turning_shunt_Completed_react(transitioned);
				break;
			}
			default:
				/* do nothing */
				break;
		}
	} 
}

void SignalControllerStatechart::runCycle() {
	/* Performs a 'run to completion' step. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	dispatchEvent(getNextEvent());
	do
	{ 
		do
		{ 
			stateConfVectorChanged = false;
			microStep();
		} while (stateConfVectorChanged);
		clearInEvents();
		dispatchEvent(getNextEvent());
	} while (((((((((((start_raised) || (clear_raised)) || (started_raised)) || (failed_raised)) || (enable_raised)) || (disable_raised)) || (completedMain_raised)) || (completedDistant_raised)) || (completedShunt_raised)) || (timeEvents[0])) || (timeEvents[1]));
	isExecuting = false;
}

void SignalControllerStatechart::enter() {
	/* Activates the state machine. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	/* Default enter sequence for statechart SignalControllerStatechart */
	enseq_main_region_default();
	do
	{ 
		stateConfVectorChanged = false;
		microStep();
	} while (stateConfVectorChanged);
	isExecuting = false;
}

void SignalControllerStatechart::exit() {
	/* Deactivates the state machine. */
	if (isExecuting)
	{ 
		return;
	} 
	isExecuting = true;
	/* Default exit sequence for statechart SignalControllerStatechart */
	exseq_main_region();
	isExecuting = false;
}

/* Can be used by the client code to trigger a run to completion step without raising an event. */
void SignalControllerStatechart::triggerWithoutEvent() {
	runCycle();
}

}
}
