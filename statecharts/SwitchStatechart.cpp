/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#include "SwitchStatechart.h"

/*! \file
Implementation of the state machine 'SwitchStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		SwitchStatechart::SwitchStatechart(QObject * parent) :
			QObject(parent),
			timeout(2000),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			stateConfVectorChanged(false),
			inquire_raised(false),
			leftResponse_raised(false),
			rightResponse_raised(false),
			response_raised(false),
			clear_raised(false),
			queued_raised(false),
			failed_raised(false),
			unlock_raised(false),
			turn_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::SwitchStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		SwitchStatechart::~SwitchStatechart()
		{
		}



		mrw::statechart::SwitchStatechart::EventInstance * SwitchStatechart::getNextEvent()
		{
			mrw::statechart::SwitchStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void SwitchStatechart::dispatchEvent(mrw::statechart::SwitchStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SwitchStatechart::Event::inquire:
				{
					inquire_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::leftResponse:
				{
					leftResponse_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::rightResponse:
				{
					rightResponse_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::response:
				{
					response_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::queued:
				{
					queued_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::unlock:
				{
					unlock_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::turn:
				{
					turn_raised = true;
					break;
				}


			case mrw::statechart::SwitchStatechart::Event::_te0_main_region_Init_:
			case mrw::statechart::SwitchStatechart::Event::_te1_main_region_Operating_operating_Turning_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SwitchStatechart::Event::_te0_main_region_Init_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::SwitchStatechart::inquire()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::inquire));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::leftResponse()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::leftResponse));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::rightResponse()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::rightResponse));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::response()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::response));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::clear));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::queued()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::queued));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::failed));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::unlock()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::unlock));
			runCycle();
		}


		void mrw::statechart::SwitchStatechart::turn()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::turn));
			runCycle();
		}



		bool SwitchStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SwitchStatechart::isFinal() const
		{
			return false;
		}

		bool SwitchStatechart::check() const
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


		void SwitchStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * SwitchStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer SwitchStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void SwitchStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SwitchStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SwitchStatechart::Event::_te0_main_region_Init_))));
				runCycle();
			}
		}


		bool SwitchStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Start] == mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] == mrw::statechart::SwitchStatechart::State::main_region_Init);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::SwitchStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Unlocked] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Locked] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning] >= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning && stateConfVector[scvi_main_region_Operating_operating_Turning] <= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_r1_Turn_Right] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_r1_Turn_Left] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_r1_Pending] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::SwitchStatechart::State::main_region_Failed);
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

		sc::integer SwitchStatechart::getTimeout() const
		{
			return timeout;
		}

		void SwitchStatechart::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		void SwitchStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Init'. */
		void SwitchStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 0, timeout, false);
			emit entered();
			ifaceOperationCallback->request();
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Unlocked'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Unlocked()
		{
			/* Entry action for state 'Unlocked'. */
			ifaceOperationCallback->lock(false);
		}

		/* Entry action for state 'Locked'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Locked()
		{
			/* Entry action for state 'Locked'. */
			ifaceOperationCallback->lock(true);
		}

		/* Entry action for state 'Turning'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Turning()
		{
			/* Entry action for state 'Turning'. */
			timerService->setTimer(this, 1, timeout, false);
			ifaceOperationCallback->pending();
		}

		/* Entry action for state 'Turn Right'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Turning_r1_Turn_Right()
		{
			/* Entry action for state 'Turn Right'. */
			ifaceOperationCallback->right();
		}

		/* Entry action for state 'Turn Left'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Turning_r1_Turn_Left()
		{
			/* Entry action for state 'Turn Left'. */
			ifaceOperationCallback->left();
		}

		/* Entry action for state 'Failed'. */
		void SwitchStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail();
		}

		/* Exit action for state 'Init'. */
		void SwitchStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Turning'. */
		void SwitchStatechart::exact_main_region_Operating_operating_Turning()
		{
			/* Exit action for state 'Turning'. */
			timerService->unsetTimer(this, 1);
		}

		/* 'default' enter sequence for state Wait for Start */
		void SwitchStatechart::enseq_main_region_Wait_for_Start_default()
		{
			/* 'default' enter sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Init */
		void SwitchStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Init;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Operating */
		void SwitchStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enseq_main_region_Operating_operating_default();
		}

		/* 'default' enter sequence for state Unlocked */
		void SwitchStatechart::enseq_main_region_Operating_operating_Unlocked_default()
		{
			/* 'default' enter sequence for state Unlocked */
			enact_main_region_Operating_operating_Unlocked();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Locked */
		void SwitchStatechart::enseq_main_region_Operating_operating_Locked_default()
		{
			/* 'default' enter sequence for state Locked */
			enact_main_region_Operating_operating_Locked();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Turn Right */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_r1_Turn_Right_default()
		{
			/* 'default' enter sequence for state Turn Right */
			enact_main_region_Operating_operating_Turning_r1_Turn_Right();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Turn Left */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_r1_Turn_Left_default()
		{
			/* 'default' enter sequence for state Turn Left */
			enact_main_region_Operating_operating_Turning_r1_Turn_Left();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Pending */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_r1_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Failed */
		void SwitchStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Failed;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for region main region */
		void SwitchStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region operating */
		void SwitchStatechart::enseq_main_region_Operating_operating_default()
		{
			/* 'default' enter sequence for region operating */
			react_main_region_Operating_operating__entry_Default();
		}

		/* Default exit sequence for state Wait for Start */
		void SwitchStatechart::exseq_main_region_Wait_for_Start()
		{
			/* Default exit sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Init */
		void SwitchStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
			exact_main_region_Init();
		}

		/* Default exit sequence for state Operating */
		void SwitchStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			exseq_main_region_Operating_operating();
		}

		/* Default exit sequence for state Unlocked */
		void SwitchStatechart::exseq_main_region_Operating_operating_Unlocked()
		{
			/* Default exit sequence for state Unlocked */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Locked */
		void SwitchStatechart::exseq_main_region_Operating_operating_Locked()
		{
			/* Default exit sequence for state Locked */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Turning */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning()
		{
			/* Default exit sequence for state Turning */
			exseq_main_region_Operating_operating_Turning_r1();
			exact_main_region_Operating_operating_Turning();
		}

		/* Default exit sequence for state Turn Right */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_r1_Turn_Right()
		{
			/* Default exit sequence for state Turn Right */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Turn Left */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_r1_Turn_Left()
		{
			/* Default exit sequence for state Turn Left */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Pending */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_r1_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Failed */
		void SwitchStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void SwitchStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.SwitchStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start :
				{
					exseq_main_region_Wait_for_Start();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Init :
				{
					exseq_main_region_Init();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked :
				{
					exseq_main_region_Operating_operating_Unlocked();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked :
				{
					exseq_main_region_Operating_operating_Locked();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Right();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Left();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending :
				{
					exseq_main_region_Operating_operating_Turning_r1_Pending();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region operating */
		void SwitchStatechart::exseq_main_region_Operating_operating()
		{
			/* Default exit sequence for region operating */
			/* Handle exit of all possible states (of mrw.statechart.SwitchStatechart.main_region.Operating.operating) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked :
				{
					exseq_main_region_Operating_operating_Unlocked();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked :
				{
					exseq_main_region_Operating_operating_Locked();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Right();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Left();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending :
				{
					exseq_main_region_Operating_operating_Turning_r1_Pending();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region r1 */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_r1()
		{
			/* Default exit sequence for region r1 */
			/* Handle exit of all possible states (of mrw.statechart.SwitchStatechart.main_region.Operating.operating.Turning.r1) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Right();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Left();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending :
				{
					exseq_main_region_Operating_operating_Turning_r1_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SwitchStatechart::react_main_region_Operating_operating__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isFree())
			{
				enseq_main_region_Operating_operating_Unlocked_default();
			}
			else
			{
				enseq_main_region_Operating_operating_Locked_default();
			}
		}

		/* The reactions of state null. */
		void SwitchStatechart::react_main_region_Operating_operating_Turning_r1__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->doTurnLeft())
			{
				enseq_main_region_Operating_operating_Turning_r1_Turn_Left_default();
			}
			else
			{
				enseq_main_region_Operating_operating_Turning_r1_Turn_Right_default();
			}
		}

		/* Default react sequence for initial entry  */
		void SwitchStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
		}

		/* Default react sequence for initial entry  */
		void SwitchStatechart::react_main_region_Operating_operating__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Operating_operating_Unlocked_default();
		}

		sc::integer SwitchStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer SwitchStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait for Start. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (inquire_raised)
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

		sc::integer SwitchStatechart::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Init();
					ifaceOperationCallback->dec();
					emit inquired();
					enseq_main_region_Operating_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Init();
						timeEvents[0] = false;
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

		sc::integer SwitchStatechart::main_region_Operating_react(const sc::integer transitioned_before)
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
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Unlocked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Unlocked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (turn_raised)
				{
					exseq_main_region_Operating_operating_Unlocked();
					ifaceOperationCallback->inc();
					enact_main_region_Operating_operating_Turning();
					react_main_region_Operating_operating_Turning_r1__choice_0();
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

		sc::integer SwitchStatechart::main_region_Operating_operating_Locked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Locked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (unlock_raised)
				{
					exseq_main_region_Operating_operating_Locked();
					enseq_main_region_Operating_operating_Unlocked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
				else
				{
					if ((leftResponse_raised) || (rightResponse_raised))
					{
						exseq_main_region_Operating();
						enseq_main_region_Failed_default();
						react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turning. */
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
				else
				{
					if (leftResponse_raised)
					{
						exseq_main_region_Operating_operating_Turning();
						ifaceOperationCallback->dec();
						react_main_region_Operating_operating__choice_0();
						transitioned_after = 0;
					}
					else
					{
						if (rightResponse_raised)
						{
							exseq_main_region_Operating_operating_Turning();
							ifaceOperationCallback->dec();
							react_main_region_Operating_operating__choice_0();
							transitioned_after = 0;
						}
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_r1_Turn_Right_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn Right. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Right();
					enseq_main_region_Operating_operating_Turning_r1_Pending_default();
					main_region_Operating_operating_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_operating_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_r1_Turn_Left_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn Left. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Operating_operating_Turning_r1_Turn_Left();
					enseq_main_region_Operating_operating_Turning_r1_Pending_default();
					main_region_Operating_operating_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_operating_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_r1_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_operating_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (clear_raised)
				{
					exseq_main_region_Failed();
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

		void SwitchStatechart::clearInEvents()
		{
			inquire_raised = false;
			leftResponse_raised = false;
			rightResponse_raised = false;
			response_raised = false;
			clear_raised = false;
			queued_raised = false;
			failed_raised = false;
			unlock_raised = false;
			turn_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
		}

		void SwitchStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start :
				{
					main_region_Wait_for_Start_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Init :
				{
					main_region_Init_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked :
				{
					main_region_Operating_operating_Unlocked_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked :
				{
					main_region_Operating_operating_Locked_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Right :
				{
					main_region_Operating_operating_Turning_r1_Turn_Right_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Turn_Left :
				{
					main_region_Operating_operating_Turning_r1_Turn_Left_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_r1_Pending :
				{
					main_region_Operating_operating_Turning_r1_Pending_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void SwitchStatechart::runCycle()
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
			while (((((((((((inquire_raised) || (leftResponse_raised)) || (rightResponse_raised)) || (response_raised)) || (clear_raised)) || (queued_raised)) || (failed_raised)) || (unlock_raised)) || (turn_raised)) || (timeEvents[0])) || (timeEvents[1]));
			isExecuting = false;
		}

		void SwitchStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart SwitchStatechart */
			enseq_main_region_default();
			do
			{
				stateConfVectorChanged = false;
				microStep();
			}
			while (stateConfVectorChanged);
			isExecuting = false;
		}

		void SwitchStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart SwitchStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SwitchStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
