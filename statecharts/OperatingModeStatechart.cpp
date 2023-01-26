/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#include "OperatingModeStatechart.h"

/*! \file
Implementation of the state machine 'OperatingModeStatechart'
*/

namespace mrw
{
	namespace statechart
	{

		const sc::integer OperatingModeStatechart::timeout = 5000;

		const sc::integer OperatingModeStatechart::Can::timeout = 250;



		OperatingModeStatechart::OperatingModeStatechart(QObject * parent) :
			QObject(parent),
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
			finalize_raised(false),
			completed_raised(false),
			operating_value(false),
			editing_value(false)
		{
			this->ifaceCan.parent = this;
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		OperatingModeStatechart::~OperatingModeStatechart()
		{
		}

		OperatingModeStatechart::Can::Can(OperatingModeStatechart * parent_) :
			connected_raised(false),
			parent(parent_),
			ifaceCanOperationCallback(nullptr)
		{
		}



		mrw::statechart::OperatingModeStatechart::EventInstance * OperatingModeStatechart::getNextEvent()
		{
			mrw::statechart::OperatingModeStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void OperatingModeStatechart::dispatchEvent(mrw::statechart::OperatingModeStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::OperatingModeStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::started:
				{
					started_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::edit:
				{
					edit_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::operate:
				{
					operate_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::init:
				{
					init_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::finalize:
				{
					finalize_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::completed:
				{
					completed_raised = true;
					break;
				}

			case mrw::statechart::OperatingModeStatechart::Event::Can_connected:
				{
					ifaceCan.connected_raised = true;
					break;
				}

			case mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Prepare_Bus_:
			case mrw::statechart::OperatingModeStatechart::Event::_te1_main_region_Init_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Prepare_Bus_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::OperatingModeStatechart::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::clear));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::started()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::started));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::failed));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::edit()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::edit));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::operate()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::operate));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::init()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::init));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::finalize()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::finalize));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::completed()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::completed));
			runCycle();
		}


		void mrw::statechart::OperatingModeStatechart::can_connected()
		{
			incomingEventQueue.push_back(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::Can_connected));
			runCycle();
		}



		bool OperatingModeStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		bool OperatingModeStatechart::isFinal() const
		{
			return (stateConfVector[0] == mrw::statechart::OperatingModeStatechart::State::main_region__final_);
		}

		bool OperatingModeStatechart::check() const
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


		void OperatingModeStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * OperatingModeStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer OperatingModeStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void OperatingModeStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::OperatingModeStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Prepare_Bus_))));
				runCycle();
			}
		}


		bool OperatingModeStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Prepare_Bus :
				{
					return  (stateConfVector[scvi_main_region_Prepare_Bus] == mrw::statechart::OperatingModeStatechart::State::main_region_Prepare_Bus);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] == mrw::statechart::OperatingModeStatechart::State::main_region_Init);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Editing :
				{
					return  (stateConfVector[scvi_main_region_Editing] == mrw::statechart::OperatingModeStatechart::State::main_region_Editing);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::OperatingModeStatechart::State::main_region_Failed);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] == mrw::statechart::OperatingModeStatechart::State::main_region_Operating);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					return  (stateConfVector[scvi_main_region_Exit] == mrw::statechart::OperatingModeStatechart::State::main_region_Exit);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					return  (stateConfVector[scvi_main_region__final_] == mrw::statechart::OperatingModeStatechart::State::main_region__final_);
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

		sc::integer OperatingModeStatechart::getTimeout()
		{
			return timeout;
		}

		void OperatingModeStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}
		OperatingModeStatechart::Can * OperatingModeStatechart::can()
		{
			return &ifaceCan;
		}
		sc::integer OperatingModeStatechart::Can::getTimeout()
		{
			return timeout;
		}

		void OperatingModeStatechart::Can::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceCanOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Prepare Bus'. */
		void OperatingModeStatechart::enact_main_region_Prepare_Bus()
		{
			/* Entry action for state 'Prepare Bus'. */
			timerService->setTimer(this, 0, OperatingModeStatechart::Can::timeout, false);
			ifaceCan.ifaceCanOperationCallback->connectBus();
		}

		/* Entry action for state 'Init'. */
		void OperatingModeStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 1, OperatingModeStatechart::timeout, false);
			ifaceOperationCallback->resetTransaction();
			emit start();
		}

		/* Entry action for state 'Editing'. */
		void OperatingModeStatechart::enact_main_region_Editing()
		{
			/* Entry action for state 'Editing'. */
			editing_value = true;
			emit editing(editing_value);
		}

		/* Entry action for state 'Failed'. */
		void OperatingModeStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			emit failing();
		}

		/* Entry action for state 'Operating'. */
		void OperatingModeStatechart::enact_main_region_Operating()
		{
			/* Entry action for state 'Operating'. */
			operating_value = true;
			emit operating(operating_value);
		}

		/* Entry action for state 'Exit'. */
		void OperatingModeStatechart::enact_main_region_Exit()
		{
			/* Entry action for state 'Exit'. */
			ifaceOperationCallback->disableRoutes();
		}

		/* Exit action for state 'Prepare Bus'. */
		void OperatingModeStatechart::exact_main_region_Prepare_Bus()
		{
			/* Exit action for state 'Prepare Bus'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Init'. */
		void OperatingModeStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 1);
		}

		/* Exit action for state 'Editing'. */
		void OperatingModeStatechart::exact_main_region_Editing()
		{
			/* Exit action for state 'Editing'. */
			editing_value = false;
			emit editing(editing_value);
		}

		/* Exit action for state 'Failed'. */
		void OperatingModeStatechart::exact_main_region_Failed()
		{
			/* Exit action for state 'Failed'. */
			emit cleared();
		}

		/* Exit action for state 'Operating'. */
		void OperatingModeStatechart::exact_main_region_Operating()
		{
			/* Exit action for state 'Operating'. */
			operating_value = false;
			emit operating(operating_value);
		}

		/* 'default' enter sequence for state Prepare Bus */
		void OperatingModeStatechart::enseq_main_region_Prepare_Bus_default()
		{
			/* 'default' enter sequence for state Prepare Bus */
			enact_main_region_Prepare_Bus();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Prepare_Bus;
		}

		/* 'default' enter sequence for state Init */
		void OperatingModeStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Init;
		}

		/* 'default' enter sequence for state Editing */
		void OperatingModeStatechart::enseq_main_region_Editing_default()
		{
			/* 'default' enter sequence for state Editing */
			enact_main_region_Editing();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Editing;
		}

		/* 'default' enter sequence for state Failed */
		void OperatingModeStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Failed;
		}

		/* 'default' enter sequence for state Operating */
		void OperatingModeStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enact_main_region_Operating();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Operating;
		}

		/* 'default' enter sequence for state Exit */
		void OperatingModeStatechart::enseq_main_region_Exit_default()
		{
			/* 'default' enter sequence for state Exit */
			enact_main_region_Exit();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Exit;
		}

		/* Default enter sequence for final state */
		void OperatingModeStatechart::enseq_main_region__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region__final_;
		}

		/* 'default' enter sequence for region main region */
		void OperatingModeStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Prepare Bus */
		void OperatingModeStatechart::exseq_main_region_Prepare_Bus()
		{
			/* Default exit sequence for state Prepare Bus */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Prepare_Bus();
		}

		/* Default exit sequence for state Init */
		void OperatingModeStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Init();
		}

		/* Default exit sequence for state Editing */
		void OperatingModeStatechart::exseq_main_region_Editing()
		{
			/* Default exit sequence for state Editing */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Editing();
		}

		/* Default exit sequence for state Failed */
		void OperatingModeStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Failed();
		}

		/* Default exit sequence for state Operating */
		void OperatingModeStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Operating();
		}

		/* Default exit sequence for state Exit */
		void OperatingModeStatechart::exseq_main_region_Exit()
		{
			/* Default exit sequence for state Exit */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for final state. */
		void OperatingModeStatechart::exseq_main_region__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void OperatingModeStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.OperatingModeStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Prepare_Bus :
				{
					exseq_main_region_Prepare_Bus();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Init :
				{
					exseq_main_region_Init();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Editing :
				{
					exseq_main_region_Editing();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Operating :
				{
					exseq_main_region_Operating();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					exseq_main_region_Exit();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					exseq_main_region__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void OperatingModeStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasActiveRoutes())
			{
				enseq_main_region_Exit_default();
			}
			else
			{
				emit quit();
				enseq_main_region__final__default();
			}
		}

		/* Default react sequence for initial entry  */
		void OperatingModeStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Prepare_Bus_default();
		}

		sc::integer OperatingModeStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer OperatingModeStatechart::main_region_Prepare_Bus_react(const sc::integer transitioned_before)
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

		sc::integer OperatingModeStatechart::main_region_Init_react(const sc::integer transitioned_before)
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

		sc::integer OperatingModeStatechart::main_region_Editing_react(const sc::integer transitioned_before)
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

		sc::integer OperatingModeStatechart::main_region_Failed_react(const sc::integer transitioned_before)
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

		sc::integer OperatingModeStatechart::main_region_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((edit_raised)) && ((!ifaceOperationCallback->hasActiveRoutes())))
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
					else
					{
						if (finalize_raised)
						{
							exseq_main_region_Operating();
							react_main_region__choice_0();
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

		sc::integer OperatingModeStatechart::main_region_Exit_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Exit. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Exit();
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

		sc::integer OperatingModeStatechart::main_region__final__react(const sc::integer transitioned_before)
		{
			/* The reactions of state null. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		void OperatingModeStatechart::clearInEvents()
		{
			clear_raised = false;
			started_raised = false;
			failed_raised = false;
			edit_raised = false;
			operate_raised = false;
			init_raised = false;
			finalize_raised = false;
			completed_raised = false;
			ifaceCan.connected_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
		}

		void OperatingModeStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Prepare_Bus :
				{
					main_region_Prepare_Bus_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Init :
				{
					main_region_Init_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Editing :
				{
					main_region_Editing_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Operating :
				{
					main_region_Operating_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					main_region_Exit_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					main_region__final__react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void OperatingModeStatechart::runCycle()
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
			while (((((((((((clear_raised) || (started_raised)) || (failed_raised)) || (edit_raised)) || (operate_raised)) || (init_raised)) || (finalize_raised)) || (completed_raised)) || (ifaceCan.connected_raised)) || (timeEvents[0])) || (timeEvents[1]));
			isExecuting = false;
		}

		void OperatingModeStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart OperatingModeStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void OperatingModeStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart OperatingModeStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void OperatingModeStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
