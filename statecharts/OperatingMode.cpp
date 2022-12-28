/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

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
			timeout(5000),
			timerService(nullptr),
			ifaceCan(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			clear_raised(false),
			started_raised(false),
			failed_raised(false),
			edit_raised(false),
			operate_raised(false),
			init_raised(false),
			operating_value(false),
			editing_value(false)
		{
			this->ifaceCan.parent = this;
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::OperatingMode::State::NO_STATE;
			}

			clearInEvents();
		}

		OperatingMode::~OperatingMode()
		{
		}

		OperatingMode::Can::Can(OperatingMode * parent_) :
			connected_raised(false),
			parent(parent_),
			ifaceCanOperationCallback(nullptr)
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
			case mrw::statechart::OperatingMode::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::started:
				{
					started_raised = true;
					break;
				}
			case mrw::statechart::OperatingMode::Event::failed:
				{
					failed_raised = true;
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
			case mrw::statechart::OperatingMode::Event::init:
				{
					init_raised = true;
					break;
				}

			case mrw::statechart::OperatingMode::Event::Can_connected:
				{
					ifaceCan.connected_raised = true;
					break;
				}

			case mrw::statechart::OperatingMode::Event::_te0_main_region_Prepare_Bus_:
			case mrw::statechart::OperatingMode::Event::_te1_main_region_Init_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::OperatingMode::Event::_te0_main_region_Prepare_Bus_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::OperatingMode::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::clear));
			runCycle();
		}


		void mrw::statechart::OperatingMode::started()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::started));
			runCycle();
		}


		void mrw::statechart::OperatingMode::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::failed));
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


		void mrw::statechart::OperatingMode::init()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::init));
			runCycle();
		}


		void mrw::statechart::OperatingMode::can_connected()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingMode::EventInstance(mrw::statechart::OperatingMode::Event::Can_connected));
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
			if (this->ifaceCan.ifaceCanOperationCallback == nullptr)
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
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::OperatingMode::Event>(evid + static_cast<sc::integer>(mrw::statechart::OperatingMode::Event::_te0_main_region_Prepare_Bus_))));
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
			case mrw::statechart::OperatingMode::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::OperatingMode::State::main_region_Failed);
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

		void OperatingMode::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}
		OperatingMode::Can * OperatingMode::can()
		{
			return &ifaceCan;
		}
		void OperatingMode::Can::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceCanOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Prepare Bus'. */
		void OperatingMode::enact_main_region_Prepare_Bus()
		{
			/* Entry action for state 'Prepare Bus'. */
			timerService->setTimer(this, 0, timeout, false);
			ifaceCan.ifaceCanOperationCallback->connectBus();
		}

		/* Entry action for state 'Init'. */
		void OperatingMode::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 1, timeout, false);
			ifaceOperationCallback->reset();
			emit start();
		}

		/* Entry action for state 'Editing'. */
		void OperatingMode::enact_main_region_Editing()
		{
			/* Entry action for state 'Editing'. */
			editing_value = true;
			emit editing(editing_value);
		}

		/* Entry action for state 'Failed'. */
		void OperatingMode::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			emit failing();
		}

		/* Entry action for state 'Operating'. */
		void OperatingMode::enact_main_region_Operating()
		{
			/* Entry action for state 'Operating'. */
			operating_value = true;
			emit operating(operating_value);
		}

		/* Exit action for state 'Prepare Bus'. */
		void OperatingMode::exact_main_region_Prepare_Bus()
		{
			/* Exit action for state 'Prepare Bus'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Init'. */
		void OperatingMode::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 1);
		}

		/* Exit action for state 'Editing'. */
		void OperatingMode::exact_main_region_Editing()
		{
			/* Exit action for state 'Editing'. */
			editing_value = false;
			emit editing(editing_value);
		}

		/* Exit action for state 'Failed'. */
		void OperatingMode::exact_main_region_Failed()
		{
			/* Exit action for state 'Failed'. */
			emit cleared();
		}

		/* Exit action for state 'Operating'. */
		void OperatingMode::exact_main_region_Operating()
		{
			/* Exit action for state 'Operating'. */
			operating_value = false;
			emit operating(operating_value);
		}

		/* 'default' enter sequence for state Prepare Bus */
		void OperatingMode::enseq_main_region_Prepare_Bus_default()
		{
			/* 'default' enter sequence for state Prepare Bus */
			enact_main_region_Prepare_Bus();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Prepare_Bus;
		}

		/* 'default' enter sequence for state Init */
		void OperatingMode::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Init;
		}

		/* 'default' enter sequence for state Editing */
		void OperatingMode::enseq_main_region_Editing_default()
		{
			/* 'default' enter sequence for state Editing */
			enact_main_region_Editing();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Editing;
		}

		/* 'default' enter sequence for state Failed */
		void OperatingMode::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Failed;
		}

		/* 'default' enter sequence for state Operating */
		void OperatingMode::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enact_main_region_Operating();
			stateConfVector[0] = mrw::statechart::OperatingMode::State::main_region_Operating;
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
			exact_main_region_Prepare_Bus();
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
			exact_main_region_Editing();
		}

		/* Default exit sequence for state Failed */
		void OperatingMode::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
			exact_main_region_Failed();
		}

		/* Default exit sequence for state Operating */
		void OperatingMode::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::OperatingMode::State::NO_STATE;
			exact_main_region_Operating();
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
			case mrw::statechart::OperatingMode::State::main_region_Failed :
				{
					exseq_main_region_Failed();
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
				if (ifaceCan.connected_raised)
				{
					exseq_main_region_Prepare_Bus();
					enseq_main_region_Init_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Prepare_Bus();
						timeEvents[0] = false;
						enseq_main_region_Failed_default();
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

		sc::integer OperatingMode::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Init();
					timeEvents[1] = false;
					enseq_main_region_Failed_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (failed_raised)
					{
						exseq_main_region_Init();
						enseq_main_region_Failed_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (started_raised)
						{
							exseq_main_region_Init();
							enseq_main_region_Operating_default();
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

		sc::integer OperatingMode::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((clear_raised)) && ((ifaceCan.ifaceCanOperationCallback->isConnected())))
				{
					exseq_main_region_Failed();
					enseq_main_region_Init_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (edit_raised)
					{
						exseq_main_region_Failed();
						enseq_main_region_Editing_default();
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
					if (init_raised)
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

		void OperatingMode::clearInEvents()
		{
			clear_raised = false;
			started_raised = false;
			failed_raised = false;
			edit_raised = false;
			operate_raised = false;
			init_raised = false;
			ifaceCan.connected_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
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
			case mrw::statechart::OperatingMode::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
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
				microStep();
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while (((((((((clear_raised) || (started_raised)) || (failed_raised)) || (edit_raised)) || (operate_raised)) || (init_raised)) || (ifaceCan.connected_raised)) || (timeEvents[0])) || (timeEvents[1]));
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
