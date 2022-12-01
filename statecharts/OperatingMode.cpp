/* Copyright (C) Steffen A. Mork */

#include "OperatingMode.h"

/*! \file
Implementation of the state machine 'OperatingMode'
*/

namespace mrw
{
	namespace statechart
	{



		OperatingMode::OperatingMode(QObject * parent) :
			QObject(parent),
			timeout(3000),
			retry(150),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			stateConfVectorChanged(false),
			connected_raised(false),
			clear_raised(false),
			inquired_raised(false),
			fail_raised(false),
			edit_raised(false),
			operate_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::OperatingMode::State::NO_STATE;
			}

			clearInEvents();
		}

		OperatingMode::~OperatingMode()
		{
		}



		mrw::statechart::OperatingMode::EventInstance * OperatingMode::getNextEvent()
		{
			mrw::statechart::OperatingMode::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void OperatingMode::dispatchEvent(mrw::statechart::OperatingMode::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::OperatingMode::Event::connected:
				{
					connected_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::inquired:
				{
					inquired_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::fail:
				{
					fail_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::edit:
				{
					edit_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::operate:
				{
					operate_raised = true;
					break;
				}


			case mrw::statechart::OperatingMode::Event::_te0_main_region_Init_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::OperatingMode::Event::_te0_main_region_Init_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::OperatingMode::connected()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::connected));
			runCycle();
		}


		void mrw::statechart::OperatingMode::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::clear));
			runCycle();
		}


		void mrw::statechart::OperatingMode::inquired()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::inquired));
			runCycle();
		}


		void mrw::statechart::OperatingMode::fail()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::fail));
			runCycle();
		}


		void mrw::statechart::OperatingMode::edit()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::edit));
			runCycle();
		}


		void mrw::statechart::OperatingMode::operate()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::operate));
			runCycle();
		}



		bool OperatingMode::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::OperatingMode::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool OperatingMode::isFinal() const
		{
			return false;
		}

		bool OperatingMode::check() const
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


		void OperatingMode::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * OperatingMode::getTimerService()
		{
			return timerService;
		}

		sc::integer OperatingMode::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void OperatingMode::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::OperatingMode::Event>(evid + static_cast<sc::integer>(mrw::statechart::OperatingMode::Event::_te0_main_region_Init_))));
				runCycle();
			}
		}


		bool OperatingMode::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::OperatingMode::State::main_region_Prepare_Bus :
				{
					return  (stateConfVector[scvi_main_region_Prepare_Bus] == mrw::statechart::OperatingMode::State::main_region_Prepare_Bus);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] == mrw::statechart::OperatingMode::State::main_region_Init);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Editing :
				{
					return  (stateConfVector[scvi_main_region_Editing] == mrw::statechart::OperatingMode::State::main_region_Editing);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Fail :
				{
					return  (stateConfVector[scvi_main_region_Fail] == mrw::statechart::OperatingMode::State::main_region_Fail);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] == mrw::statechart::OperatingMode::State::main_region_Operating);
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

		sc::integer OperatingMode::getTimeout() const
		{
			return timeout;
		}

		void OperatingMode::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		sc::integer OperatingMode::getRetry() const
		{
			return retry;
		}

		void OperatingMode::setRetry(sc::integer retry_)
		{
			this->retry = retry_;
		}

		void OperatingMode::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Prepare Bus'. */
		void OperatingMode::enact_main_region_Prepare_Bus()
		{
			/* Entry action for state 'Prepare Bus'. */
			ifaceOperationCallback->connectBus();
		}

		/* Entry action for state 'Init'. */
		void OperatingMode::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 0, timeout, false);
			ifaceOperationCallback->reset();
			emit inquire();
		}

		/* Entry action for state 'Editing'. */
		void OperatingMode::enact_main_region_Editing()
		{
			/* Entry action for state 'Editing'. */
			emit editing();
		}

		/* Entry action for state 'Fail'. */
		void OperatingMode::enact_main_region_Fail()
		{
			/* Entry action for state 'Fail'. */
			emit failed();
		}

		/* Entry action for state 'Operating'. */
		void OperatingMode::enact_main_region_Operating()
		{
			/* Entry action for state 'Operating'. */
			emit operating();
		}

		/* Exit action for state 'Init'. */
		void OperatingMode::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 0);
		}

		/* 'default' enter sequence for state Prepare Bus */
		void OperatingMode::enseq_main_region_Prepare_Bus_default()
		{
			/* 'default' enter sequence for state Prepare Bus */
			enact_main_region_Prepare_Bus();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Prepare_Bus;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Init */
		void OperatingMode::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Init;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Editing */
		void OperatingMode::enseq_main_region_Editing_default()
		{
			/* 'default' enter sequence for state Editing */
			enact_main_region_Editing();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Editing;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Fail */
		void OperatingMode::enseq_main_region_Fail_default()
		{
			/* 'default' enter sequence for state Fail */
			enact_main_region_Fail();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Fail;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Operating */
		void OperatingMode::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enact_main_region_Operating();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Operating;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for region main region */
		void OperatingMode::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Prepare Bus */
		void OperatingMode::exseq_main_region_Prepare_Bus()
		{
			/* Default exit sequence for state Prepare Bus */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
		}

		/* Default exit sequence for state Init */
		void OperatingMode::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
			exact_main_region_Init();
		}

		/* Default exit sequence for state Editing */
		void OperatingMode::exseq_main_region_Editing()
		{
			/* Default exit sequence for state Editing */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
		}

		/* Default exit sequence for state Fail */
		void OperatingMode::exseq_main_region_Fail()
		{
			/* Default exit sequence for state Fail */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
		}

		/* Default exit sequence for state Operating */
		void OperatingMode::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void OperatingMode::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.OperatingMode.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingMode::State::main_region_Prepare_Bus :
				{
					exseq_main_region_Prepare_Bus();
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Init :
				{
					exseq_main_region_Init();
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Editing :
				{
					exseq_main_region_Editing();
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Fail :
				{
					exseq_main_region_Fail();
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Operating :
				{
					exseq_main_region_Operating();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default react sequence for initial entry  */
		void OperatingMode::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Prepare_Bus_default();
		}

		sc::integer OperatingMode::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer OperatingMode::main_region_Prepare_Bus_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Prepare Bus. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (connected_raised)
				{
					exseq_main_region_Prepare_Bus();
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

		sc::integer OperatingMode::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[0])
				{
					exseq_main_region_Init();
					timeEvents[0] = false;
					enseq_main_region_Fail_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (inquired_raised)
					{
						exseq_main_region_Init();
						enseq_main_region_Operating_default();
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

		sc::integer OperatingMode::main_region_Editing_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Editing. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (operate_raised)
				{
					exseq_main_region_Editing();
					enseq_main_region_Operating_default();
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

		sc::integer OperatingMode::main_region_Fail_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Fail. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (clear_raised)
				{
					exseq_main_region_Fail();
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

		sc::integer OperatingMode::main_region_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (edit_raised)
				{
					exseq_main_region_Operating();
					enseq_main_region_Editing_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (fail_raised)
					{
						exseq_main_region_Operating();
						enseq_main_region_Fail_default();
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

		void OperatingMode::clearInEvents()
		{
			connected_raised = false;
			clear_raised = false;
			inquired_raised = false;
			fail_raised = false;
			edit_raised = false;
			operate_raised = false;
			timeEvents[0] = false;
		}

		void OperatingMode::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingMode::State::main_region_Prepare_Bus :
				{
					main_region_Prepare_Bus_react(-1);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Init :
				{
					main_region_Init_react(-1);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Editing :
				{
					main_region_Editing_react(-1);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Fail :
				{
					main_region_Fail_react(-1);
					break;
				}
			case mrw::statechart::OperatingMode::State::main_region_Operating :
				{
					main_region_Operating_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void OperatingMode::runCycle()
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
				do
				{
					stateConfVectorChanged = false;
					microStep();
				}
				while (stateConfVectorChanged);
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while (((((((connected_raised) || (clear_raised)) || (inquired_raised)) || (fail_raised)) || (edit_raised)) || (operate_raised)) || (timeEvents[0]));
			isExecuting = false;
		}

		void OperatingMode::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart OperatingMode */
			enseq_main_region_default();
			do
			{
				stateConfVectorChanged = false;
				microStep();
			}
			while (stateConfVectorChanged);
			isExecuting = false;
		}

		void OperatingMode::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart OperatingMode */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void OperatingMode::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
