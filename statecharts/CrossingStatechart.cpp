/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//
* */

#include "CrossingStatechart.h"

/*! \file
Implementation of the state machine 'CrossingStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		CrossingStatechart::CrossingStatechart(QObject * parent) noexcept :
			action_raised(false),
			clear_raised(false),
			start_raised(false),
			response_raised(false),
			failed_raised(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
		{
			std::fill(std::begin(stateConfVector), std::end(stateConfVector), mrw::statechart::CrossingStatechart::State::NO_STATE);
			clearInEvents();
		}

		CrossingStatechart::~CrossingStatechart()
		{
			if (!timerService)
			{
				return;
			}
			timerService->unsetTimerRaw(this, 0);
			timerService->unsetTimerRaw(this, 1);
			timerService->unsetTimerRaw(this, 2);
		}



		std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance> CrossingStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance> nextEvent = nullptr;

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

		bool CrossingStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::CrossingStatechart::Event::action:
				{
					action_raised = true;
					break;
				}
			case mrw::statechart::CrossingStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::CrossingStatechart::Event::start:
				{
					start_raised = true;
					break;
				}
			case mrw::statechart::CrossingStatechart::Event::response:
				{
					response_raised = true;
					break;
				}
			case mrw::statechart::CrossingStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}

			case mrw::statechart::CrossingStatechart::Event::_te0_main_region_Init_:
			case mrw::statechart::CrossingStatechart::Event::_te1_main_region_Operating_Processing_Pending_:
			case mrw::statechart::CrossingStatechart::Event::_te2_main_region_Operating_Processing_Pending_Crossing_processing_Delay_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::CrossingStatechart::Event::_te0_main_region_Init_)] = true;
					break;
				}
			default:
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'action' that is defined in the default interface scope. */
		void mrw::statechart::CrossingStatechart::action()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance>(new mrw::statechart::CrossingStatechart::EventInstance(mrw::statechart::CrossingStatechart::Event::action)));
			runCycle();
		}


		/*! Slot for the in event 'clear' that is defined in the default interface scope. */
		void mrw::statechart::CrossingStatechart::clear()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance>(new mrw::statechart::CrossingStatechart::EventInstance(mrw::statechart::CrossingStatechart::Event::clear)));
			runCycle();
		}


		/*! Slot for the in event 'start' that is defined in the default interface scope. */
		void mrw::statechart::CrossingStatechart::start()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance>(new mrw::statechart::CrossingStatechart::EventInstance(mrw::statechart::CrossingStatechart::Event::start)));
			runCycle();
		}


		/*! Slot for the in event 'response' that is defined in the default interface scope. */
		void mrw::statechart::CrossingStatechart::response()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance>(new mrw::statechart::CrossingStatechart::EventInstance(mrw::statechart::CrossingStatechart::Event::response)));
			runCycle();
		}


		/*! Slot for the in event 'failed' that is defined in the default interface scope. */
		void mrw::statechart::CrossingStatechart::failed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::CrossingStatechart::EventInstance>(new mrw::statechart::CrossingStatechart::EventInstance(mrw::statechart::CrossingStatechart::Event::failed)));
			runCycle();
		}



		bool CrossingStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::CrossingStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool CrossingStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool CrossingStatechart::check() const noexcept
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


		void CrossingStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> CrossingStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer CrossingStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void CrossingStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::CrossingStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::CrossingStatechart::Event::_te0_main_region_Init_)))));
				runCycle();
			}
		}


		bool CrossingStatechart::isStateActive(State state) const noexcept
		{
			switch (state)
			{
			case mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start :
				{
					return  (stateConfVector[scvi_main_region_Wait_For_Start] == mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::CrossingStatechart::State::main_region_Failed);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] == mrw::statechart::CrossingStatechart::State::main_region_Init);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::CrossingStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Unlocked] == mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked] == mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending] >= mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending && stateConfVector[scvi_main_region_Operating_Processing_Pending] <= mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Crossing_processing_Closing] == mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Crossing_processing_Opening] == mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Pending_Crossing_processing_Delay] == mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay);
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

		sc::integer CrossingStatechart::getTimeout() noexcept
		{
			return timeout;
		}

		sc::integer CrossingStatechart::getDelay() noexcept
		{
			return delay;
		}

		void CrossingStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Failed'. */
		void CrossingStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail();
		}

		/* Entry action for state 'Init'. */
		void CrossingStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (CrossingStatechart::timeout)), false);
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
			ifaceOperationCallback->open();
		}

		/* Entry action for state 'Unlocked'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Unlocked()
		{
			/* Entry action for state 'Unlocked'. */
			ifaceOperationCallback->lock(false);
		}

		/* Entry action for state 'Locked'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Locked()
		{
			/* Entry action for state 'Locked'. */
			ifaceOperationCallback->lock(true);
		}

		/* Entry action for state 'Pending'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Pending()
		{
			/* Entry action for state 'Pending'. */
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (CrossingStatechart::timeout)), false);
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
		}

		/* Entry action for state 'Closing'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Pending_Crossing_processing_Closing()
		{
			/* Entry action for state 'Closing'. */
			ifaceOperationCallback->close();
		}

		/* Entry action for state 'Opening'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Pending_Crossing_processing_Opening()
		{
			/* Entry action for state 'Opening'. */
			ifaceOperationCallback->open();
		}

		/* Entry action for state 'Delay'. */
		void CrossingStatechart::enact_main_region_Operating_Processing_Pending_Crossing_processing_Delay()
		{
			/* Entry action for state 'Delay'. */
			timerService->setTimer(shared_from_this(), 2, (static_cast<sc::time> (CrossingStatechart::delay)), false);
		}

		/* Exit action for state 'Init'. */
		void CrossingStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(shared_from_this(), 0);
			ifaceOperationCallback->dec();
		}

		/* Exit action for state 'Pending'. */
		void CrossingStatechart::exact_main_region_Operating_Processing_Pending()
		{
			/* Exit action for state 'Pending'. */
			timerService->unsetTimer(shared_from_this(), 1);
			ifaceOperationCallback->dec();
			ifaceOperationCallback->unregister();
		}

		/* Exit action for state 'Delay'. */
		void CrossingStatechart::exact_main_region_Operating_Processing_Pending_Crossing_processing_Delay()
		{
			/* Exit action for state 'Delay'. */
			timerService->unsetTimer(shared_from_this(), 2);
		}

		/* 'default' enter sequence for state Wait For Start */
		void CrossingStatechart::enseq_main_region_Wait_For_Start_default()
		{
			/* 'default' enter sequence for state Wait For Start */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start;
		}

		/* 'default' enter sequence for state Failed */
		void CrossingStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Failed;
		}

		/* 'default' enter sequence for state Init */
		void CrossingStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Init;
		}

		/* 'default' enter sequence for state Operating */
		void CrossingStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enseq_main_region_Operating_Processing_default();
		}

		/* 'default' enter sequence for state Unlocked */
		void CrossingStatechart::enseq_main_region_Operating_Processing_Unlocked_default()
		{
			/* 'default' enter sequence for state Unlocked */
			enact_main_region_Operating_Processing_Unlocked();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked;
		}

		/* 'default' enter sequence for state Locked */
		void CrossingStatechart::enseq_main_region_Operating_Processing_Locked_default()
		{
			/* 'default' enter sequence for state Locked */
			enact_main_region_Operating_Processing_Locked();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked;
		}

		/* 'default' enter sequence for state Closing */
		void CrossingStatechart::enseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing_default()
		{
			/* 'default' enter sequence for state Closing */
			enact_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing;
		}

		/* 'default' enter sequence for state Opening */
		void CrossingStatechart::enseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening_default()
		{
			/* 'default' enter sequence for state Opening */
			enact_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening;
		}

		/* 'default' enter sequence for state Delay */
		void CrossingStatechart::enseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay_default()
		{
			/* 'default' enter sequence for state Delay */
			enact_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay;
		}

		/* 'default' enter sequence for region main region */
		void CrossingStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region Processing */
		void CrossingStatechart::enseq_main_region_Operating_Processing_default()
		{
			/* 'default' enter sequence for region Processing */
			react_main_region_Operating_Processing__entry_Default();
		}

		/* Default exit sequence for state Wait For Start */
		void CrossingStatechart::exseq_main_region_Wait_For_Start()
		{
			/* Default exit sequence for state Wait For Start */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Failed */
		void CrossingStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Init */
		void CrossingStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::NO_STATE;
			exact_main_region_Init();
		}

		/* Default exit sequence for state Operating */
		void CrossingStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			exseq_main_region_Operating_Processing();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Unlocked */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Unlocked()
		{
			/* Default exit sequence for state Unlocked */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating;
		}

		/* Default exit sequence for state Locked */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Locked()
		{
			/* Default exit sequence for state Locked */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating;
		}

		/* Default exit sequence for state Pending */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Pending()
		{
			/* Default exit sequence for state Pending */
			exseq_main_region_Operating_Processing_Pending_Crossing_processing();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating;
			exact_main_region_Operating_Processing_Pending();
		}

		/* Default exit sequence for state Closing */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing()
		{
			/* Default exit sequence for state Closing */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending;
		}

		/* Default exit sequence for state Opening */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening()
		{
			/* Default exit sequence for state Opening */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending;
		}

		/* Default exit sequence for state Delay */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay()
		{
			/* Default exit sequence for state Delay */
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending;
			exact_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
		}

		/* Default exit sequence for region main region */
		void CrossingStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.CrossingStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start :
				{
					exseq_main_region_Wait_For_Start();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Init :
				{
					exseq_main_region_Init();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating :
				{
					exseq_main_region_Operating();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					exseq_main_region_Operating_Processing_Unlocked();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked :
				{
					exseq_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending :
				{
					exseq_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Processing */
		void CrossingStatechart::exseq_main_region_Operating_Processing()
		{
			/* Default exit sequence for region Processing */
			/* Handle exit of all possible states (of mrw.statechart.CrossingStatechart.main_region.Operating.Processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					exseq_main_region_Operating_Processing_Unlocked();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked :
				{
					exseq_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending :
				{
					exseq_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
					exact_main_region_Operating_Processing_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Crossing processing */
		void CrossingStatechart::exseq_main_region_Operating_Processing_Pending_Crossing_processing()
		{
			/* Default exit sequence for region Crossing processing */
			/* Handle exit of all possible states (of mrw.statechart.CrossingStatechart.main_region.Operating.Processing.Pending.Crossing_processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay :
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void CrossingStatechart::react_main_region_Operating_Processing_Pending_Crossing_processing__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->used())
			{
				enseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing_default();
			}
			else
			{
				exseq_main_region_Operating_Processing_Pending();
				enseq_main_region_Operating_Processing_Unlocked_default();
				main_region_Operating_react(0);
			}
		}

		/* Default react sequence for initial entry  */
		void CrossingStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_For_Start_default();
		}

		/* Default react sequence for initial entry  */
		void CrossingStatechart::react_main_region_Operating_Processing__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Operating_Processing_Unlocked_default();
		}

		sc::integer CrossingStatechart::main_region_Wait_For_Start_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait For Start. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (start_raised)
				{
					exseq_main_region_Wait_For_Start();
					enseq_main_region_Init_default();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (clear_raised)
				{
					exseq_main_region_Failed();
					enseq_main_region_Wait_For_Start_default();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Init();
					enseq_main_region_Operating_default();
					transitioned_after = 0;
				}
				else
				{
					if (failed_raised)
					{
						exseq_main_region_Init();
						enseq_main_region_Failed_default();
						transitioned_after = 0;
					}
					else
					{
						if (timeEvents[0])
						{
							exseq_main_region_Init();
							timeEvents[0] = false;
							enseq_main_region_Failed_default();
							transitioned_after = 0;
						}
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (failed_raised)
				{
					exseq_main_region_Operating();
					enseq_main_region_Failed_default();
					transitioned_after = 0;
				}
				else
				{
					if (start_raised)
					{
						exseq_main_region_Operating();
						enseq_main_region_Init_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Unlocked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((action_raised)) && ((ifaceOperationCallback->used())))
				{
					exseq_main_region_Operating_Processing_Unlocked();
					enact_main_region_Operating_Processing_Pending();
					enseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing_default();
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

		sc::integer CrossingStatechart::main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Locked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((action_raised)) && ((!(ifaceOperationCallback->used()))))
				{
					exseq_main_region_Operating_Processing_Locked();
					enact_main_region_Operating_Processing_Pending();
					enseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening_default();
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

		sc::integer CrossingStatechart::main_region_Operating_Processing_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Operating();
					timeEvents[1] = false;
					enseq_main_region_Failed_default();
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

		sc::integer CrossingStatechart::main_region_Operating_Processing_Pending_Crossing_processing_Closing_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Closing. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
					enseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay_default();
					main_region_Operating_Processing_Pending_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Operating_Processing_Pending_Crossing_processing_Opening_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Opening. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
					react_main_region_Operating_Processing_Pending_Crossing_processing__choice_0();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer CrossingStatechart::main_region_Operating_Processing_Pending_Crossing_processing_Delay_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Delay. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[2])
				{
					exseq_main_region_Operating_Processing_Pending();
					timeEvents[2] = false;
					enseq_main_region_Operating_Processing_Locked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Operating_Processing_Pending_react(transitioned_before);
			}
			return transitioned_after;
		}

		void CrossingStatechart::clearInEvents() noexcept
		{
			action_raised = false;
			clear_raised = false;
			start_raised = false;
			response_raised = false;
			failed_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
		}

		void CrossingStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start :
				{
					main_region_Wait_For_Start_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Init :
				{
					main_region_Init_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					main_region_Operating_Processing_Unlocked_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked :
				{
					main_region_Operating_Processing_Locked_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing :
				{
					main_region_Operating_Processing_Pending_Crossing_processing_Closing_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening :
				{
					main_region_Operating_Processing_Pending_Crossing_processing_Opening_react(-1);
					break;
				}
			case mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Delay :
				{
					main_region_Operating_Processing_Pending_Crossing_processing_Delay_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void CrossingStatechart::runCycle()
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

		void CrossingStatechart::enter()
		{
			/* Activates the state machine. */
			{
			};
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart CrossingStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void CrossingStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart CrossingStatechart */
			exseq_main_region();
			stateConfVector[0] = mrw::statechart::CrossingStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void CrossingStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
