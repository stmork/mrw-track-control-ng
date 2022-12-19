/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#include "SignalStatechart.h"

/*! \file
Implementation of the state machine 'SignalStatechart'
*/

namespace mrw
{
	namespace statechart
	{

		const sc::integer SignalStatechart::FREE = 0;
		const sc::integer SignalStatechart::SHUNTING = 1;
		const sc::integer SignalStatechart::TOUR = 2;
		const sc::integer SignalStatechart::OFF = -1;
		const sc::integer SignalStatechart::STOP = 0;
		const sc::integer SignalStatechart::GO = 1;
		const sc::integer SignalStatechart::SLOW = 2;



		SignalStatechart::SignalStatechart(QObject * parent) :
			QObject(parent),
			timeout(2000),
			signalState(SignalStatechart::STOP),
			sectionState(SignalStatechart::FREE),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			turn_raised(false),
			turn_value(0),
			queued_raised(false),
			ok_raised(false),
			fail_raised(false),
			clear_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::SignalStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		SignalStatechart::~SignalStatechart()
		{
		}



		mrw::statechart::SignalStatechart::EventInstance * SignalStatechart::getNextEvent()
		{
			mrw::statechart::SignalStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void SignalStatechart::dispatchEvent(mrw::statechart::SignalStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SignalStatechart::Event::turn:
				{
					mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer> * e = static_cast<mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer>*>(event);
					if (e != 0)
					{
						turn_value = e->value;
						turn_raised = true;
					}
					break;
				}
			case mrw::statechart::SignalStatechart::Event::queued:
				{
					queued_raised = true;
					break;
				}
			case mrw::statechart::SignalStatechart::Event::ok:
				{
					ok_raised = true;
					break;
				}
			case mrw::statechart::SignalStatechart::Event::fail:
				{
					fail_raised = true;
					break;
				}
			case mrw::statechart::SignalStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}


			case mrw::statechart::SignalStatechart::Event::_te0_main_region_Turning_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Turning_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::SignalStatechart::turn(sc::integer turn_)
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer>(mrw::statechart::SignalStatechart::Event::turn, turn_));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::queued()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::queued));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::ok()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::ok));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::fail()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::fail));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::clear));
			runCycle();
		}



		bool SignalStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SignalStatechart::isFinal() const
		{
			return false;
		}

		bool SignalStatechart::check() const
		{
			if (timerService == nullptr)
			{
				return false;
			}
			if (this->ifaceOperationCallback == nullptr)
			{
				return false;
			}
			return true;
		}


		void SignalStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * SignalStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer SignalStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void SignalStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SignalStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Turning_))));
				runCycle();
			}
		}


		bool SignalStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::SignalStatechart::State::main_region_Idle :
				{
					return  (stateConfVector[scvi_main_region_Idle] == mrw::statechart::SignalStatechart::State::main_region_Idle);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning :
				{
					return  (stateConfVector[scvi_main_region_Turning] >= mrw::statechart::SignalStatechart::State::main_region_Turning && stateConfVector[scvi_main_region_Turning] <= mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					return  (stateConfVector[scvi_main_region_Turning_Turn_processing_Pending] == mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn :
				{
					return  (stateConfVector[scvi_main_region_Turning_Turn_processing_Turn] == mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Fail :
				{
					return  (stateConfVector[scvi_main_region_Fail] == mrw::statechart::SignalStatechart::State::main_region_Fail);
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

		sc::integer SignalStatechart::getTimeout() const
		{
			return timeout;
		}

		void SignalStatechart::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		sc::integer SignalStatechart::getSignalState() const
		{
			return signalState;
		}

		void SignalStatechart::setSignalState(sc::integer signalState_)
		{
			this->signalState = signalState_;
		}

		sc::integer SignalStatechart::getSectionState() const
		{
			return sectionState;
		}

		void SignalStatechart::setSectionState(sc::integer sectionState_)
		{
			this->sectionState = sectionState_;
		}

		sc::integer SignalStatechart::getFREE()
		{
			return FREE;
		}

		sc::integer SignalStatechart::getSHUNTING()
		{
			return SHUNTING;
		}

		sc::integer SignalStatechart::getTOUR()
		{
			return TOUR;
		}

		sc::integer SignalStatechart::getOFF()
		{
			return OFF;
		}

		sc::integer SignalStatechart::getSTOP()
		{
			return STOP;
		}

		sc::integer SignalStatechart::getGO()
		{
			return GO;
		}

		sc::integer SignalStatechart::getSLOW()
		{
			return SLOW;
		}

		void SignalStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Idle'. */
		void SignalStatechart::enact_main_region_Idle()
		{
			/* Entry action for state 'Idle'. */
			ifaceOperationCallback->idle();
		}

		/* Entry action for state 'Turning'. */
		void SignalStatechart::enact_main_region_Turning()
		{
			/* Entry action for state 'Turning'. */
			timerService->setTimer(this, 0, timeout, false);
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Turning_Turn_processing_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->send(turn_value);
		}

		/* Entry action for state 'Fail'. */
		void SignalStatechart::enact_main_region_Fail()
		{
			/* Entry action for state 'Fail'. */
			signalState = SignalStatechart::OFF;
			ifaceOperationCallback->send(signalState);
			emit failed();
		}

		/* Exit action for state 'Turning'. */
		void SignalStatechart::exact_main_region_Turning()
		{
			/* Exit action for state 'Turning'. */
			timerService->unsetTimer(this, 0);
		}

		/* 'default' enter sequence for state Idle */
		void SignalStatechart::enseq_main_region_Idle_default()
		{
			/* 'default' enter sequence for state Idle */
			enact_main_region_Idle();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Idle;
		}

		/* 'default' enter sequence for state Turning */
		void SignalStatechart::enseq_main_region_Turning_default()
		{
			/* 'default' enter sequence for state Turning */
			enact_main_region_Turning();
			enseq_main_region_Turning_Turn_processing_default();
		}

		/* 'default' enter sequence for state Pending */
		void SignalStatechart::enseq_main_region_Turning_Turn_processing_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending;
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Turning_Turn_processing_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Turning_Turn_processing_Turn();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn;
		}

		/* 'default' enter sequence for state Fail */
		void SignalStatechart::enseq_main_region_Fail_default()
		{
			/* 'default' enter sequence for state Fail */
			enact_main_region_Fail();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Fail;
		}

		/* 'default' enter sequence for region main region */
		void SignalStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region Turn processing */
		void SignalStatechart::enseq_main_region_Turning_Turn_processing_default()
		{
			/* 'default' enter sequence for region Turn processing */
			react_main_region_Turning_Turn_processing__entry_Default();
		}

		/* Default exit sequence for state Idle */
		void SignalStatechart::exseq_main_region_Idle()
		{
			/* Default exit sequence for state Idle */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Turning */
		void SignalStatechart::exseq_main_region_Turning()
		{
			/* Default exit sequence for state Turning */
			exseq_main_region_Turning_Turn_processing();
			exact_main_region_Turning();
		}

		/* Default exit sequence for state Pending */
		void SignalStatechart::exseq_main_region_Turning_Turn_processing_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Turning_Turn_processing_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Fail */
		void SignalStatechart::exseq_main_region_Fail()
		{
			/* Default exit sequence for state Fail */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void SignalStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Idle :
				{
					exseq_main_region_Idle();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					exseq_main_region_Turning_Turn_processing_Pending();
					exact_main_region_Turning();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn :
				{
					exseq_main_region_Turning_Turn_processing_Turn();
					exact_main_region_Turning();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Fail :
				{
					exseq_main_region_Fail();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Turn processing */
		void SignalStatechart::exseq_main_region_Turning_Turn_processing()
		{
			/* Default exit sequence for region Turn processing */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Turning.Turn_processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					exseq_main_region_Turning_Turn_processing_Pending();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn :
				{
					exseq_main_region_Turning_Turn_processing_Turn();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasSignal())
			{
				enseq_main_region_Turning_default();
			}
			else
			{
				emit completed();
				enseq_main_region_Idle_default();
			}
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Idle_default();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Turning_Turn_processing__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Turning_Turn_processing_Turn_default();
		}

		sc::integer SignalStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer SignalStatechart::main_region_Idle_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Idle. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (turn_raised)
				{
					exseq_main_region_Idle();
					react_main_region__choice_0();
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

		sc::integer SignalStatechart::main_region_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turning. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (ok_raised)
				{
					exseq_main_region_Turning();
					signalState = turn_value;
					emit completed();
					enseq_main_region_Idle_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (fail_raised)
					{
						exseq_main_region_Turning();
						enseq_main_region_Fail_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (timeEvents[0])
						{
							exseq_main_region_Turning();
							timeEvents[0] = false;
							enseq_main_region_Fail_default();
							react(0);
							transitioned_after = 0;
						}
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

		sc::integer SignalStatechart::main_region_Turning_Turn_processing_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Turning_Turn_processing_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Turning_Turn_processing_Turn();
					enseq_main_region_Turning_Turn_processing_Pending_default();
					main_region_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Fail_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Fail. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (clear_raised)
				{
					exseq_main_region_Fail();
					enseq_main_region_Idle_default();
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

		void SignalStatechart::clearInEvents()
		{
			turn_raised = false;
			queued_raised = false;
			ok_raised = false;
			fail_raised = false;
			clear_raised = false;
			timeEvents[0] = false;
		}

		void SignalStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Idle :
				{
					main_region_Idle_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					main_region_Turning_Turn_processing_Pending_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Turn :
				{
					main_region_Turning_Turn_processing_Turn_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Fail :
				{
					main_region_Fail_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void SignalStatechart::runCycle()
		{
			/* Performs a 'run to completion' step. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			dispatchEvent(getNextEvent());
			do
			{
				microStep();
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while ((((((turn_raised) || (queued_raised)) || (ok_raised)) || (fail_raised)) || (clear_raised)) || (timeEvents[0]));
			isExecuting = false;
		}

		void SignalStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart SignalStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void SignalStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart SignalStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SignalStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
