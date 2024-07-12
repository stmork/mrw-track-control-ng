/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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



		SwitchStatechart::SwitchStatechart(QObject * parent) noexcept :
			isManual(false),
			clear_raised(false),
			start_raised(false),
			leftResponse_raised(false),
			rightResponse_raised(false),
			response_raised(false),
			queued_raised(false),
			failed_raised(false),
			unlock_raised(false),
			turn_raised(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
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



		std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance> SwitchStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance> nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = std::move(incomingEventQueue.front());
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		template<typename EWV, typename EV>
		std::unique_ptr<EWV> cast_event_pointer_type (std::unique_ptr<EV> && event)
		{
			return std::unique_ptr<EWV> {static_cast<EWV *>(event.release())};
		}

		bool SwitchStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SwitchStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::SwitchStatechart::Event::start:
				{
					start_raised = true;
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
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'clear' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::clear()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::clear)))
			;
			runCycle();
		}


		/*! Slot for the in event 'start' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::start()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::start)))
			;
			runCycle();
		}


		/*! Slot for the in event 'leftResponse' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::leftResponse()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::leftResponse)))
			;
			runCycle();
		}


		/*! Slot for the in event 'rightResponse' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::rightResponse()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::rightResponse)))
			;
			runCycle();
		}


		/*! Slot for the in event 'response' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::response()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::response)))
			;
			runCycle();
		}


		/*! Slot for the in event 'queued' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::queued()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::queued)))
			;
			runCycle();
		}


		/*! Slot for the in event 'failed' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::failed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::failed)))
			;
			runCycle();
		}


		/*! Slot for the in event 'unlock' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::unlock()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::unlock)))
			;
			runCycle();
		}


		/*! Slot for the in event 'turn' that is defined in the default interface scope. */
		void mrw::statechart::SwitchStatechart::turn()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SwitchStatechart::EventInstance>(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::turn)))
			;
			runCycle();
		}



		bool SwitchStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SwitchStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool SwitchStatechart::check() const noexcept
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


		void SwitchStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> SwitchStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer SwitchStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void SwitchStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::SwitchStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SwitchStatechart::Event::_te0_main_region_Init_)))));
				runCycle();
			}
		}


		bool SwitchStatechart::isStateActive(State state) const noexcept
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
					return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::SwitchStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending);
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
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning] >= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning && stateConfVector[scvi_main_region_Operating_operating_Turning] <= mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Right] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Left] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending :
				{
					return  (stateConfVector[scvi_main_region_Operating_operating_Turning_Turning_process_Pending] == mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending);
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

		bool SwitchStatechart::getIsManual() const noexcept
		{
			return isManual
				;
		}

		void SwitchStatechart::setIsManual(bool isManual_) noexcept
		{
			this->isManual = isManual_;
		}
		sc::integer SwitchStatechart::getTimeout() noexcept
		{
			return timeout
				;
		}

		void SwitchStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Init'. */
		void SwitchStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (SwitchStatechart::timeout)), false);
			emit entered();
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
			ifaceOperationCallback->request();
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
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (SwitchStatechart::timeout)), false);
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
		}

		/* Entry action for state 'Turn Right'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Turning_Turning_process_Turn_Right()
		{
			/* Entry action for state 'Turn Right'. */
			ifaceOperationCallback->right();
		}

		/* Entry action for state 'Turn Left'. */
		void SwitchStatechart::enact_main_region_Operating_operating_Turning_Turning_process_Turn_Left()
		{
			/* Entry action for state 'Turn Left'. */
			ifaceOperationCallback->left();
		}

		/* Entry action for state 'Failed'. */
		void SwitchStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail();
			emit stop();
		}

		/* Exit action for state 'Init'. */
		void SwitchStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(shared_from_this(), 0);
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Turning'. */
		void SwitchStatechart::exact_main_region_Operating_operating_Turning()
		{
			/* Exit action for state 'Turning'. */
			timerService->unsetTimer(shared_from_this(), 1);
			ifaceOperationCallback->dec();
		}

		/* 'default' enter sequence for state Wait for Start */
		void SwitchStatechart::enseq_main_region_Wait_for_Start_default()
		{
			/* 'default' enter sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start;
		}

		/* 'default' enter sequence for state Init */
		void SwitchStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Init;
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
		}

		/* 'default' enter sequence for state Locked */
		void SwitchStatechart::enseq_main_region_Operating_operating_Locked_default()
		{
			/* 'default' enter sequence for state Locked */
			enact_main_region_Operating_operating_Locked();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked;
		}

		/* 'mrw.statechart.SwitchStatechart.main_region.Operating.operating.Turning' enter sequence for state Turning */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_mrw_statechart_SwitchStatechart_main_region_Operating_operating_Turning()
		{
			/* 'mrw.statechart.SwitchStatechart.main_region.Operating.operating.Turning' enter sequence for state Turning */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning;
		}

		/* 'default' enter sequence for state Turn Right */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right_default()
		{
			/* 'default' enter sequence for state Turn Right */
			enact_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right;
		}

		/* 'default' enter sequence for state Turn Left */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left_default()
		{
			/* 'default' enter sequence for state Turn Left */
			enact_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left;
		}

		/* 'default' enter sequence for state Pending */
		void SwitchStatechart::enseq_main_region_Operating_operating_Turning_Turning_process_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending;
		}

		/* 'default' enter sequence for state Failed */
		void SwitchStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Failed;
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
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Unlocked */
		void SwitchStatechart::exseq_main_region_Operating_operating_Unlocked()
		{
			/* Default exit sequence for state Unlocked */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating;
		}

		/* Default exit sequence for state Locked */
		void SwitchStatechart::exseq_main_region_Operating_operating_Locked()
		{
			/* Default exit sequence for state Locked */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating;
		}

		/* Default exit sequence for state Turning */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning()
		{
			/* Default exit sequence for state Turning */
			exseq_main_region_Operating_operating_Turning_Turning_process();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating;
			exact_main_region_Operating_operating_Turning();
		}

		/* Default exit sequence for state Turn Right */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right()
		{
			/* Default exit sequence for state Turn Right */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning;
		}

		/* Default exit sequence for state Turn Left */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left()
		{
			/* Default exit sequence for state Turn Left */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning;
		}

		/* Default exit sequence for state Pending */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_Turning_process_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning;
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating :
				{
					exseq_main_region_Operating();
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning :
				{
					exseq_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Pending();
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning :
				{
					exseq_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Pending();
					exact_main_region_Operating_operating_Turning();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Turning process */
		void SwitchStatechart::exseq_main_region_Operating_operating_Turning_Turning_process()
		{
			/* Default exit sequence for region Turning process */
			/* Handle exit of all possible states (of mrw.statechart.SwitchStatechart.main_region.Operating.operating.Turning.Turning_process) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending :
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Pending();
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
			if (((ifaceOperationCallback->isFree()) || (isManual)) || ((!(ifaceOperationCallback->isReserved()))))
			{
				enseq_main_region_Operating_operating_Unlocked_default();
			}
			else
			{
				enseq_main_region_Operating_operating_Locked_default();
			}
		}

		/* The reactions of state null. */
		void SwitchStatechart::react_main_region_Operating_operating_Turning_Turning_process__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->doTurnLeft())
			{
				enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left_default();
			}
			else
			{
				enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right_default();
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
				if (start_raised)
				{
					exseq_main_region_Wait_for_Start();
					enseq_main_region_Init_default();
					react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
					emit started();
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
					enseq_main_region_Operating_operating_Turning_mrw_statechart_SwitchStatechart_main_region_Operating_operating_Turning();
					enact_main_region_Operating_operating_Turning();
					react_main_region_Operating_operating_Turning_Turning_process__choice_0();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
					if (response_raised)
					{
						exseq_main_region_Operating();
						enseq_main_region_Failed_default();
						react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
						react_main_region_Operating_operating__choice_0();
						transitioned_after = 0;
					}
					else
					{
						if (rightResponse_raised)
						{
							exseq_main_region_Operating_operating_Turning();
							react_main_region_Operating_operating__choice_0();
							transitioned_after = 0;
						}
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_Turning_process_Turn_Right_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn Right. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
					enseq_main_region_Operating_operating_Turning_Turning_process_Pending_default();
					main_region_Operating_operating_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_operating_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_Turning_process_Turn_Left_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn Left. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
					enseq_main_region_Operating_operating_Turning_Turning_process_Pending_default();
					main_region_Operating_operating_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_operating_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SwitchStatechart::main_region_Operating_operating_Turning_Turning_process_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
					enseq_main_region_Wait_for_Start_default();
					react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		void SwitchStatechart::clearInEvents() noexcept
		{
			clear_raised = false;
			start_raised = false;
			leftResponse_raised = false;
			rightResponse_raised = false;
			response_raised = false;
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right :
				{
					main_region_Operating_operating_Turning_Turning_process_Turn_Right_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left :
				{
					main_region_Operating_operating_Turning_Turning_process_Turn_Left_react(-1);
					break;
				}
			case mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending :
				{
					main_region_Operating_operating_Turning_Turning_process_Pending_react(-1);
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
				microStep();
				clearInEvents();
			}
			while (dispatchEvent(getNextEvent()));
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
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SwitchStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
