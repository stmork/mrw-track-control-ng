/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#include "SignalControllerStatechart.h"

/*! \file
Implementation of the state machine 'SignalControllerStatechart'
*/

namespace mrw
{
	namespace statechart
	{

		const sc::integer SignalControllerStatechart::timeout = 5000;
		const sc::integer SignalControllerStatechart::delay = 350;
		const sc::integer SignalControllerStatechart::OFF = -1;
		const sc::integer SignalControllerStatechart::STOP = 0;
		const sc::integer SignalControllerStatechart::GO = 1;



		SignalControllerStatechart::SignalControllerStatechart() :
			symbol(SignalControllerStatechart::STOP),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			stateConfVectorPosition(0),
			stateConfVectorChanged(false),
			start_raised(false),
			clear_raised(false),
			failed_raised(false),
			enable_raised(false),
			extend_raised(false),
			disable_raised(false),
			turnMain_value(0),
			turnMain_raised(false),
			turnDistant_value(0),
			turnDistant_raised(false),
			turnShunt_value(0),
			turnShunt_raised(false),
			completedMain_raised(false),
			completedDistant_raised(false),
			completedShunt_raised(false),
			started_raised(false),
			cleared_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			}

			clearInEvents();
			clearOutEvents();
		}

		SignalControllerStatechart::~SignalControllerStatechart()
		{
		}



		mrw::statechart::SignalControllerStatechart::EventInstance * SignalControllerStatechart::getNextEvent()
		{
			mrw::statechart::SignalControllerStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void SignalControllerStatechart::dispatchEvent(mrw::statechart::SignalControllerStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
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
			case mrw::statechart::SignalControllerStatechart::Event::extend:
				{
					extend_raised = true;
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
			case mrw::statechart::SignalControllerStatechart::Event::_te1_main_region_Operating_Processing_Shunting_State_Processing_Waiting_:
			case mrw::statechart::SignalControllerStatechart::Event::_te2_main_region_Operating_Processing_Tour_State_Processing_Waiting_:
			case mrw::statechart::SignalControllerStatechart::Event::_te3_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_:
			case mrw::statechart::SignalControllerStatechart::Event::_te4_main_region_Operating_Processing_Pending_:
			case mrw::statechart::SignalControllerStatechart::Event::_te5_main_region_Operating_Processing_Pending_Pending_Delay_:
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


		void mrw::statechart::SignalControllerStatechart::raiseStart()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::start));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseClear()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::clear));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseFailed()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::failed));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseEnable()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::enable));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseExtend()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::extend));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseDisable()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::disable));
			runCycle();
		}


		bool mrw::statechart::SignalControllerStatechart::isRaisedTurnMain()
		{
			return turnMain_raised;
		}


		sc::integer mrw::statechart::SignalControllerStatechart::getTurnMainValue()
		{
			return turnMain_value;
		}


		bool mrw::statechart::SignalControllerStatechart::isRaisedTurnDistant()
		{
			return turnDistant_raised;
		}


		sc::integer mrw::statechart::SignalControllerStatechart::getTurnDistantValue()
		{
			return turnDistant_value;
		}


		bool mrw::statechart::SignalControllerStatechart::isRaisedTurnShunt()
		{
			return turnShunt_raised;
		}


		sc::integer mrw::statechart::SignalControllerStatechart::getTurnShuntValue()
		{
			return turnShunt_value;
		}


		void mrw::statechart::SignalControllerStatechart::raiseCompletedMain()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedMain));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseCompletedDistant()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedDistant));
			runCycle();
		}


		void mrw::statechart::SignalControllerStatechart::raiseCompletedShunt()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalControllerStatechart::EventInstance(mrw::statechart::SignalControllerStatechart::Event::completedShunt));
			runCycle();
		}


		bool mrw::statechart::SignalControllerStatechart::isRaisedStarted()
		{
			return started_raised;
		}


		bool mrw::statechart::SignalControllerStatechart::isRaisedCleared()
		{
			return cleared_raised;
		}



		bool SignalControllerStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SignalControllerStatechart::State::NO_STATE || stateConfVector[1] != mrw::statechart::SignalControllerStatechart::State::NO_STATE || stateConfVector[2] != mrw::statechart::SignalControllerStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SignalControllerStatechart::isFinal() const
		{
			return false;
		}

		bool SignalControllerStatechart::check() const
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


		void SignalControllerStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * SignalControllerStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer SignalControllerStatechart::getNumberOfParallelTimeEvents()
		{
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
					return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Unlocked] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Shunting_State] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State && stateConfVector[scvi_main_region_Operating_Processing_Shunting_State] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Shunting_State_Processing_Idle] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting && stateConfVector[scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State && stateConfVector[scvi_main_region_Operating_Processing_Tour_State] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting && stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Tour_State_Processing_Idle] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending] >= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending && stateConfVector[scvi_main_region_Operating_Processing_Pending] <= mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Pending_Go_Main] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Pending_Go_Distant] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Pending_Go_Shunt] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Pending_Delay] == mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay);
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

		sc::integer SignalControllerStatechart::getTimeout()
		{
			return timeout;
		}

		sc::integer SignalControllerStatechart::getDelay()
		{
			return delay;
		}

		sc::integer SignalControllerStatechart::getSymbol() const
		{
			return symbol;
		}

		void SignalControllerStatechart::setSymbol(sc::integer symbol_)
		{
			this->symbol = symbol_;
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

		void SignalControllerStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Init'. */
		void SignalControllerStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 0, SignalControllerStatechart::timeout, false);
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Turning'. */
		void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning()
		{
			/* Entry action for state 'Turning'. */
			ifaceOperationCallback->pending();
		}

		/* Entry action for state 'Turn'. */
		void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_main_Turn()
		{
			/* Entry action for state 'Turn'. */
			turnMain_value = SignalControllerStatechart::STOP;
			turnMain_raised = true;
		}

		/* Entry action for state 'Turn'. */
		void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_distant_Turn()
		{
			/* Entry action for state 'Turn'. */
			if (!ifaceOperationCallback->hasMainSignal())
			{
				turnDistant_value = SignalControllerStatechart::STOP;
				turnDistant_raised = true;
			}
			if (ifaceOperationCallback->hasMainSignal())
			{
				turnDistant_value = SignalControllerStatechart::OFF;
				turnDistant_raised = true;
			}
		}

		/* Entry action for state 'Turn'. */
		void SignalControllerStatechart::enact_main_region_Init_Init_process_Turning_shunt_Turn()
		{
			/* Entry action for state 'Turn'. */
			if (!ifaceOperationCallback->isMainAndShunt())
			{
				turnShunt_value = SignalControllerStatechart::STOP;
				turnShunt_raised = true;
			}
			if (ifaceOperationCallback->isMainAndShunt())
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

		/* Entry action for state 'Shunting State'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Shunting_State()
		{
			/* Entry action for state 'Shunting State'. */
			ifaceOperationCallback->lock(true);
		}

		/* Entry action for state 'Waiting'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting()
		{
			/* Entry action for state 'Waiting'. */
			timerService->setTimer(this, 1, SignalControllerStatechart::timeout, false);
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Extend'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend()
		{
			/* Entry action for state 'Extend'. */
			turnShunt_value = symbol;
			turnShunt_raised = true;
		}

		/* Entry action for state 'Stop'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop()
		{
			/* Entry action for state 'Stop'. */
			turnShunt_value = symbol;
			turnShunt_raised = true;
		}

		/* Entry action for state 'Tour State'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State()
		{
			/* Entry action for state 'Tour State'. */
			ifaceOperationCallback->lock(true);
		}

		/* Entry action for state 'Waiting'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting()
		{
			/* Entry action for state 'Waiting'. */
			timerService->setTimer(this, 2, SignalControllerStatechart::timeout, false);
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Stop Main'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main()
		{
			/* Entry action for state 'Stop Main'. */
			turnMain_value = symbol;
			turnMain_raised = true;
		}

		/* Entry action for state 'Stop Distant'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant()
		{
			/* Entry action for state 'Stop Distant'. */
			turnDistant_value = SignalControllerStatechart::STOP;
			turnDistant_raised = true;
		}

		/* Entry action for state 'Off Distant'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant()
		{
			/* Entry action for state 'Off Distant'. */
			turnDistant_value = SignalControllerStatechart::OFF;
			turnDistant_raised = true;
		}

		/* Entry action for state 'Delay'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay()
		{
			/* Entry action for state 'Delay'. */
			timerService->setTimer(this, 3, SignalControllerStatechart::delay, false);
		}

		/* Entry action for state 'Stop Shunt'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt()
		{
			/* Entry action for state 'Stop Shunt'. */
			turnShunt_value = symbol;
			turnShunt_raised = true;
		}

		/* Entry action for state 'Extend'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend()
		{
			/* Entry action for state 'Extend'. */
			turnDistant_value = symbol;
			turnDistant_raised = true;
		}

		/* Entry action for state 'Pending'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Pending()
		{
			/* Entry action for state 'Pending'. */
			timerService->setTimer(this, 4, SignalControllerStatechart::timeout, false);
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
		}

		/* Entry action for state 'Go Main'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Pending_Pending_Go_Main()
		{
			/* Entry action for state 'Go Main'. */
			turnMain_value = symbol;
			turnMain_raised = true;
			if (ifaceOperationCallback->isLightSignal())
			{
				turnDistant_value = SignalControllerStatechart::STOP;
				turnDistant_raised = true;
			}
		}

		/* Entry action for state 'Go Distant'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Pending_Pending_Go_Distant()
		{
			/* Entry action for state 'Go Distant'. */
			turnDistant_value = symbol;
			turnDistant_raised = true;
		}

		/* Entry action for state 'Go Shunt'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Pending_Pending_Go_Shunt()
		{
			/* Entry action for state 'Go Shunt'. */
			turnShunt_value = symbol;
			turnShunt_raised = true;
		}

		/* Entry action for state 'Delay'. */
		void SignalControllerStatechart::enact_main_region_Operating_Processing_Pending_Pending_Delay()
		{
			/* Entry action for state 'Delay'. */
			timerService->setTimer(this, 5, SignalControllerStatechart::delay, false);
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
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Waiting'. */
		void SignalControllerStatechart::exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting()
		{
			/* Exit action for state 'Waiting'. */
			timerService->unsetTimer(this, 1);
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Waiting'. */
		void SignalControllerStatechart::exact_main_region_Operating_Processing_Tour_State_Processing_Waiting()
		{
			/* Exit action for state 'Waiting'. */
			timerService->unsetTimer(this, 2);
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Delay'. */
		void SignalControllerStatechart::exact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay()
		{
			/* Exit action for state 'Delay'. */
			timerService->unsetTimer(this, 3);
		}

		/* Exit action for state 'Pending'. */
		void SignalControllerStatechart::exact_main_region_Operating_Processing_Pending()
		{
			/* Exit action for state 'Pending'. */
			timerService->unsetTimer(this, 4);
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Delay'. */
		void SignalControllerStatechart::exact_main_region_Operating_Processing_Pending_Pending_Delay()
		{
			/* Exit action for state 'Delay'. */
			timerService->unsetTimer(this, 5);
		}

		/* Exit action for state 'Failed'. */
		void SignalControllerStatechart::exact_main_region_Failed()
		{
			/* Exit action for state 'Failed'. */
			cleared_raised = true;
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

		/* 'default' enter sequence for state Turning */
		void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_default()
		{
			/* 'default' enter sequence for state Turning */
			enact_main_region_Init_Init_process_Turning();
			enseq_main_region_Init_Init_process_Turning_main_default();
			enseq_main_region_Init_Init_process_Turning_distant_default();
			enseq_main_region_Init_Init_process_Turning_shunt_default();
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

		/* 'default' enter sequence for state Idle */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Shunting_State_Processing_Idle_default()
		{
			/* 'default' enter sequence for state Idle */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Extend */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_default()
		{
			/* 'default' enter sequence for state Extend */
			enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Stop */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_default()
		{
			/* 'default' enter sequence for state Stop */
			enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Stop Main */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_default()
		{
			/* 'default' enter sequence for state Stop Main */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Stop Distant */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_default()
		{
			/* 'default' enter sequence for state Stop Distant */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Off Distant */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_default()
		{
			/* 'default' enter sequence for state Off Distant */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Delay */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_default()
		{
			/* 'default' enter sequence for state Delay */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Stop Shunt */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_default()
		{
			/* 'default' enter sequence for state Stop Shunt */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Extend */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_default()
		{
			/* 'default' enter sequence for state Extend */
			enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Idle */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Tour_State_Processing_Idle_default()
		{
			/* 'default' enter sequence for state Idle */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Go Main */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Pending_Pending_Go_Main_default()
		{
			/* 'default' enter sequence for state Go Main */
			enact_main_region_Operating_Processing_Pending_Pending_Go_Main();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Go Distant */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Pending_Pending_Go_Distant_default()
		{
			/* 'default' enter sequence for state Go Distant */
			enact_main_region_Operating_Processing_Pending_Pending_Go_Distant();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Go Shunt */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt_default()
		{
			/* 'default' enter sequence for state Go Shunt */
			enact_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Delay */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_Pending_Pending_Delay_default()
		{
			/* 'default' enter sequence for state Delay */
			enact_main_region_Operating_Processing_Pending_Pending_Delay();
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay;
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

		/* 'default' enter sequence for region main */
		void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_main_default()
		{
			/* 'default' enter sequence for region main */
			react_main_region_Init_Init_process_Turning_main__entry_Default();
		}

		/* 'default' enter sequence for region distant */
		void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_distant_default()
		{
			/* 'default' enter sequence for region distant */
			react_main_region_Init_Init_process_Turning_distant__entry_Default();
		}

		/* 'default' enter sequence for region shunt */
		void SignalControllerStatechart::enseq_main_region_Init_Init_process_Turning_shunt_default()
		{
			/* 'default' enter sequence for region shunt */
			react_main_region_Init_Init_process_Turning_shunt__entry_Default();
		}

		/* 'default' enter sequence for region Processing */
		void SignalControllerStatechart::enseq_main_region_Operating_Processing_default()
		{
			/* 'default' enter sequence for region Processing */
			react_main_region_Operating_Processing__entry_Default();
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

		/* Default exit sequence for state Shunting State */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State()
		{
			/* Default exit sequence for state Shunting State */
			exseq_main_region_Operating_Processing_Shunting_State_Processing();
		}

		/* Default exit sequence for state Idle */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle()
		{
			/* Default exit sequence for state Idle */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Waiting */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting()
		{
			/* Default exit sequence for state Waiting */
			exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting();
			exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
		}

		/* Default exit sequence for state Extend */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend()
		{
			/* Default exit sequence for state Extend */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Stop */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop()
		{
			/* Default exit sequence for state Stop */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Tour State */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State()
		{
			/* Default exit sequence for state Tour State */
			exseq_main_region_Operating_Processing_Tour_State_Processing();
		}

		/* Default exit sequence for state Waiting */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting()
		{
			/* Default exit sequence for state Waiting */
			exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting();
			exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
		}

		/* Default exit sequence for state Stop Main */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main()
		{
			/* Default exit sequence for state Stop Main */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Stop Distant */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant()
		{
			/* Default exit sequence for state Stop Distant */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Off Distant */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant()
		{
			/* Default exit sequence for state Off Distant */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Delay */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay()
		{
			/* Default exit sequence for state Delay */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
			exact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
		}

		/* Default exit sequence for state Stop Shunt */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt()
		{
			/* Default exit sequence for state Stop Shunt */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Extend */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend()
		{
			/* Default exit sequence for state Extend */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Idle */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Idle()
		{
			/* Default exit sequence for state Idle */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Pending */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending()
		{
			/* Default exit sequence for state Pending */
			exseq_main_region_Operating_Processing_Pending_Pending();
			exact_main_region_Operating_Processing_Pending();
		}

		/* Default exit sequence for state Go Main */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending_Pending_Go_Main()
		{
			/* Default exit sequence for state Go Main */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Go Distant */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending_Pending_Go_Distant()
		{
			/* Default exit sequence for state Go Distant */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Go Shunt */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt()
		{
			/* Default exit sequence for state Go Shunt */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Delay */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending_Pending_Delay()
		{
			/* Default exit sequence for state Delay */
			stateConfVector[0] = mrw::statechart::SignalControllerStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
			exact_main_region_Operating_Processing_Pending_Pending_Delay();
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
			switch (stateConfVector[ 0 ])
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
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Main();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Distant();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Delay();
					exact_main_region_Operating_Processing_Pending();
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
			switch (stateConfVector[ 1 ])
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
			switch (stateConfVector[ 2 ])
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
			switch (stateConfVector[ 0 ])
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
			switch (stateConfVector[ 1 ])
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
			switch (stateConfVector[ 2 ])
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
			switch (stateConfVector[ 0 ])
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
			switch (stateConfVector[ 1 ])
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
			switch (stateConfVector[ 2 ])
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
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					exseq_main_region_Operating_Processing_Unlocked();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Main();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Distant();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Delay();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Processing */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing()
		{
			/* Default exit sequence for region Processing */
			/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Shunting_State.Processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
					exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Shunt waiting */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting()
		{
			/* Default exit sequence for region Shunt waiting */
			/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Shunting_State.Processing.Waiting.Shunt_waiting) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Processing */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing()
		{
			/* Default exit sequence for region Processing */
			/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Tour_State.Processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
					exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Tour waiting */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting()
		{
			/* Default exit sequence for region Tour waiting */
			/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Tour_State.Processing.Waiting.Tour_waiting) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Pending */
		void SignalControllerStatechart::exseq_main_region_Operating_Processing_Pending_Pending()
		{
			/* Default exit sequence for region Pending */
			/* Handle exit of all possible states (of mrw.statechart.SignalControllerStatechart.main_region.Operating.Processing.Pending.Pending) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Main();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Distant();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Delay();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isMainAndShunt())
			{
				enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_default();
			}
			else
			{
				enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_default();
			}
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_1()
		{
			/* The reactions of state null. */
			if ((ifaceOperationCallback->hasMainSignal()) && (ifaceOperationCallback->isLightSignal()))
			{
				enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_default();
			}
			else
			{
				exseq_main_region_Operating_Processing_Tour_State();
				ifaceOperationCallback->dec();
				enseq_main_region_Operating_Processing_Unlocked_default();
				main_region_Operating_react(0);
			}
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Operating_Processing_Pending_Pending__choice_0()
		{
			/* The reactions of state null. */
			if ((ifaceOperationCallback->isMainAndShunt()) && (ifaceOperationCallback->isTour()))
			{
				enseq_main_region_Operating_Processing_Pending_Pending_Go_Main_default();
			}
			else
			{
				enseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt_default();
			}
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Operating_Processing_Pending_Pending__choice_1()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isTour())
			{
				enseq_main_region_Operating_Processing_Pending_Pending_Go_Main_default();
			}
			else
			{
				exseq_main_region_Operating_Processing_Pending();
				enact_main_region_Operating_Processing_Shunting_State();
				enseq_main_region_Operating_Processing_Shunting_State_Processing_Idle_default();
				main_region_Operating_react(0);
			}
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Operating_Processing_Pending_Pending__choice_2()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isLightSignal())
			{
				enseq_main_region_Operating_Processing_Pending_Pending_Delay_default();
			}
			else
			{
				enseq_main_region_Operating_Processing_Pending_Pending_Go_Distant_default();
			}
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region_Init_Init_process_Turning_main__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Init_Init_process_Turning_main_Turn_default();
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region_Init_Init_process_Turning_distant__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Init_Init_process_Turning_distant_Turn_default();
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region_Init_Init_process_Turning_shunt__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Init_Init_process_Turning_shunt_Turn_default();
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region_Init_Init_process__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Init_Init_process_Turning_default();
		}

		/* Default react sequence for initial entry  */
		void SignalControllerStatechart::react_main_region_Operating_Processing__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Operating_Processing_Unlocked_default();
		}

		/* The reactions of state null. */
		void SignalControllerStatechart::react_main_region_Init_Init_process__sync0()
		{
			/* The reactions of state null. */
			exseq_main_region_Init();
			enseq_main_region_Operating_default();
			react(0);
		}

		sc::integer SignalControllerStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer SignalControllerStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before)
		{
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

		sc::integer SignalControllerStatechart::main_region_Init_react(const sc::integer transitioned_before)
		{
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
				}
				else
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

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_react(const sc::integer transitioned_before)
		{
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

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_main_Turn_react(const sc::integer transitioned_before)
		{
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

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_main_Completed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Completed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed)))
				{
					exseq_main_region_Init_Init_process_Turning();
					react_main_region_Init_Init_process__sync0();
					transitioned_after = 0;
				}
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_distant_Turn_react(const sc::integer transitioned_before)
		{
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

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_distant_Completed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Completed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (1))
			{
				if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed)))
				{
					exseq_main_region_Init_Init_process_Turning();
					react_main_region_Init_Init_process__sync0();
					transitioned_after = 1;
				}
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_shunt_Turn_react(const sc::integer transitioned_before)
		{
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

		sc::integer SignalControllerStatechart::main_region_Init_Init_process_Turning_shunt_Completed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Completed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (2))
			{
				if ((isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed)) && (isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed)))
				{
					exseq_main_region_Init_Init_process_Turning();
					react_main_region_Init_Init_process__sync0();
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

		sc::integer SignalControllerStatechart::main_region_Operating_react(const sc::integer transitioned_before)
		{
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
				}
				else
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

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Unlocked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((enable_raised)) && (((symbol) == (SignalControllerStatechart::GO))))
				{
					exseq_main_region_Operating_Processing_Unlocked();
					enact_main_region_Operating_Processing_Pending();
					react_main_region_Operating_Processing_Pending_Pending__choice_0();
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

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Shunting_State_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Shunting State. */
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

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Shunting_State_Processing_Idle_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Idle. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (disable_raised)
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
					symbol = SignalControllerStatechart::STOP;
					enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_default();
					main_region_Operating_Processing_Shunting_State_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (extend_raised)
					{
						exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
						enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
						enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_default();
						main_region_Operating_Processing_Shunting_State_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Shunting_State_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Shunting_State_Processing_Waiting_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Waiting. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
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
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Shunting_State_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Extend. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedShunt_raised)
				{
					exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
					enseq_main_region_Operating_Processing_Shunting_State_Processing_Idle_default();
					main_region_Operating_Processing_Shunting_State_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Shunting_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Stop. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedShunt_raised)
				{
					exseq_main_region_Operating_Processing_Shunting_State();
					enseq_main_region_Operating_Processing_Unlocked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Shunting_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Tour State. */
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

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Waiting. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[2])
				{
					exseq_main_region_Operating();
					timeEvents[2] = false;
					enseq_main_region_Failed_default();
					react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Stop Main. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedMain_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
					enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_default();
					main_region_Operating_Processing_Tour_State_Processing_Waiting_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Stop Distant. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedDistant_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
					react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_1();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Off Distant. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedDistant_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State();
					enseq_main_region_Operating_Processing_Unlocked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Delay. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[3])
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
					timeEvents[3] = false;
					enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_default();
					main_region_Operating_Processing_Tour_State_Processing_Waiting_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Stop Shunt. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedShunt_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
					enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_default();
					main_region_Operating_Processing_Tour_State_Processing_Waiting_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Extend. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedDistant_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					enseq_main_region_Operating_Processing_Tour_State_Processing_Idle_default();
					main_region_Operating_Processing_Tour_State_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_Processing_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Tour_State_Processing_Idle_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Idle. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (disable_raised)
				{
					exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
					symbol = SignalControllerStatechart::STOP;
					enact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
					react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_0();
					main_region_Operating_Processing_Tour_State_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (extend_raised)
					{
						exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
						enact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
						enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_default();
						main_region_Operating_Processing_Tour_State_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Tour_State_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[4])
				{
					exseq_main_region_Operating();
					timeEvents[4] = false;
					enseq_main_region_Failed_default();
					react(0);
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

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Pending_Pending_Go_Main_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Go Main. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedMain_raised)
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Main();
					react_main_region_Operating_Processing_Pending_Pending__choice_2();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Pending_Pending_Go_Distant_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Go Distant. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedDistant_raised)
				{
					exseq_main_region_Operating_Processing_Pending();
					enact_main_region_Operating_Processing_Tour_State();
					enseq_main_region_Operating_Processing_Tour_State_Processing_Idle_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Pending_Pending_Go_Shunt_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Go Shunt. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completedShunt_raised)
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
					react_main_region_Operating_Processing_Pending_Pending__choice_1();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Operating_Processing_Pending_Pending_Delay_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Delay. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[5])
				{
					exseq_main_region_Operating_Processing_Pending_Pending_Delay();
					timeEvents[5] = false;
					enseq_main_region_Operating_Processing_Pending_Pending_Go_Distant_default();
					main_region_Operating_Processing_Pending_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalControllerStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
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

		void SignalControllerStatechart::clearOutEvents()
		{
			turnMain_raised = false;
			turnDistant_raised = false;
			turnShunt_raised = false;
			started_raised = false;
			cleared_raised = false;
		}

		void SignalControllerStatechart::clearInEvents()
		{
			start_raised = false;
			clear_raised = false;
			failed_raised = false;
			enable_raised = false;
			extend_raised = false;
			disable_raised = false;
			completedMain_raised = false;
			completedDistant_raised = false;
			completedShunt_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
			timeEvents[3] = false;
			timeEvents[4] = false;
			timeEvents[5] = false;
		}

		void SignalControllerStatechart::microStep()
		{
			sc::integer transitioned = -1;
			stateConfVectorPosition = 0;
			switch (stateConfVector[ 0 ])
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
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle :
				{
					transitioned = main_region_Operating_Processing_Shunting_State_Processing_Idle_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend :
				{
					transitioned = main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop :
				{
					transitioned = main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle :
				{
					transitioned = main_region_Operating_Processing_Tour_State_Processing_Idle_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main :
				{
					transitioned = main_region_Operating_Processing_Pending_Pending_Go_Main_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant :
				{
					transitioned = main_region_Operating_Processing_Pending_Pending_Go_Distant_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt :
				{
					transitioned = main_region_Operating_Processing_Pending_Pending_Go_Shunt_react(transitioned);
					break;
				}
			case mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay :
				{
					transitioned = main_region_Operating_Processing_Pending_Pending_Delay_react(transitioned);
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
				switch (stateConfVector[ 1 ])
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
				switch (stateConfVector[ 2 ])
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

		void SignalControllerStatechart::runCycle()
		{
			/* Performs a 'run to completion' step. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			clearOutEvents();
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
			while (((((((((((((((start_raised) || (clear_raised)) || (failed_raised)) || (enable_raised)) || (extend_raised)) || (disable_raised)) || (completedMain_raised)) || (completedDistant_raised)) || (completedShunt_raised)) || (timeEvents[0])) || (timeEvents[1])) || (timeEvents[2])) || (timeEvents[3])) || (timeEvents[4])) || (timeEvents[5]));
			isExecuting = false;
		}

		void SignalControllerStatechart::enter()
		{
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
			}
			while (stateConfVectorChanged);
			isExecuting = false;
		}

		void SignalControllerStatechart::exit()
		{
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
		void SignalControllerStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
