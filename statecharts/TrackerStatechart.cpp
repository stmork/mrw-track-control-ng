/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */

#include "TrackerStatechart.h"

/*! \file
Implementation of the state machine 'TrackerStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		TrackerStatechart::TrackerStatechart(QObject * parent) noexcept :
			received_raised(false),
			completed_raised(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::TrackerStatechart::State::NO_STATE;
			}

			clearInEvents();
			clearInternalEvents();
		}

		TrackerStatechart::~TrackerStatechart()
		{
		}



		std::unique_ptr<mrw::statechart::TrackerStatechart::EventInstance> TrackerStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::TrackerStatechart::EventInstance> nextEvent = 0;

			if (!internalEventQueue.empty())
			{
				nextEvent = std::move(internalEventQueue.front());
				internalEventQueue.pop_front();
			}
			else if (!incomingEventQueue.empty())
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

		bool TrackerStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::TrackerStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::TrackerStatechart::Event::received:
				{
					received_raised = true;
					break;
				}
			case mrw::statechart::TrackerStatechart::Event::Internal_completed:
				{
					completed_raised = true;
					break;
				}


			case mrw::statechart::TrackerStatechart::Event::_te0_main_region_Preparing_:
			case mrw::statechart::TrackerStatechart::Event::_te1_main_region_Driving_Tracking_First_:
			case mrw::statechart::TrackerStatechart::Event::_te2_main_region_Driving_Tracking_Occupy_:
			case mrw::statechart::TrackerStatechart::Event::_te3_main_region_Driving_Tracking_Free_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::TrackerStatechart::Event::_te0_main_region_Preparing_)] = true;
					break;
				}
			default:
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'received' that is defined in the default interface scope. */
		void mrw::statechart::TrackerStatechart::received()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::TrackerStatechart::EventInstance>(new mrw::statechart::TrackerStatechart::EventInstance(mrw::statechart::TrackerStatechart::Event::received)))
			;
			runCycle();
		}



		bool TrackerStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::TrackerStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool TrackerStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool TrackerStatechart::check() const noexcept
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


		void TrackerStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> TrackerStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer TrackerStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void TrackerStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::TrackerStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::TrackerStatechart::Event::_te0_main_region_Preparing_)))));
				runCycle();
			}
		}


		bool TrackerStatechart::isStateActive(State state) const noexcept
		{
			switch (state)
			{
			case mrw::statechart::TrackerStatechart::State::main_region_Preparing :
				{
					return  (stateConfVector[scvi_main_region_Preparing] == mrw::statechart::TrackerStatechart::State::main_region_Preparing);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving :
				{
					return  (stateConfVector[scvi_main_region_Driving] >= mrw::statechart::TrackerStatechart::State::main_region_Driving && stateConfVector[scvi_main_region_Driving] <= mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First :
				{
					return  (stateConfVector[scvi_main_region_Driving_Tracking_First] == mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy :
				{
					return  (stateConfVector[scvi_main_region_Driving_Tracking_Occupy] == mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free :
				{
					return  (stateConfVector[scvi_main_region_Driving_Tracking_Free] == mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Idle :
				{
					return  (stateConfVector[scvi_main_region_Idle] == mrw::statechart::TrackerStatechart::State::main_region_Idle);
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

		sc::integer TrackerStatechart::getStart() noexcept
		{
			return start
				;
		}

		sc::integer TrackerStatechart::getStep() noexcept
		{
			return step
				;
		}

		void TrackerStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Preparing'. */
		void TrackerStatechart::enact_main_region_Preparing()
		{
			/* Entry action for state 'Preparing'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (TrackerStatechart::start)), false);
		}

		/* Entry action for state 'First'. */
		void TrackerStatechart::enact_main_region_Driving_Tracking_First()
		{
			/* Entry action for state 'First'. */
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (TrackerStatechart::step)), false);
			ifaceOperationCallback->first();
		}

		/* Entry action for state 'Occupy'. */
		void TrackerStatechart::enact_main_region_Driving_Tracking_Occupy()
		{
			/* Entry action for state 'Occupy'. */
			timerService->setTimer(shared_from_this(), 2, (static_cast<sc::time> (TrackerStatechart::step)), false);
			ifaceOperationCallback->occupy();
		}

		/* Entry action for state 'Free'. */
		void TrackerStatechart::enact_main_region_Driving_Tracking_Free()
		{
			/* Entry action for state 'Free'. */
			timerService->setTimer(shared_from_this(), 3, (static_cast<sc::time> (TrackerStatechart::step)), false);
			ifaceOperationCallback->free();
			internalEventQueue.push_back(std::unique_ptr<mrw::statechart::TrackerStatechart::EventInstance>(new mrw::statechart::TrackerStatechart::EventInstance(mrw::statechart::TrackerStatechart::Event::Internal_completed)))
			;
		}

		/* Entry action for state 'Idle'. */
		void TrackerStatechart::enact_main_region_Idle()
		{
			/* Entry action for state 'Idle'. */
			ifaceOperationCallback->clear();
		}

		/* Exit action for state 'Preparing'. */
		void TrackerStatechart::exact_main_region_Preparing()
		{
			/* Exit action for state 'Preparing'. */
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* Exit action for state 'First'. */
		void TrackerStatechart::exact_main_region_Driving_Tracking_First()
		{
			/* Exit action for state 'First'. */
			timerService->unsetTimer(shared_from_this(), 1);
		}

		/* Exit action for state 'Occupy'. */
		void TrackerStatechart::exact_main_region_Driving_Tracking_Occupy()
		{
			/* Exit action for state 'Occupy'. */
			timerService->unsetTimer(shared_from_this(), 2);
		}

		/* Exit action for state 'Free'. */
		void TrackerStatechart::exact_main_region_Driving_Tracking_Free()
		{
			/* Exit action for state 'Free'. */
			timerService->unsetTimer(shared_from_this(), 3);
		}

		/* 'default' enter sequence for state Preparing */
		void TrackerStatechart::enseq_main_region_Preparing_default()
		{
			/* 'default' enter sequence for state Preparing */
			enact_main_region_Preparing();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Preparing;
		}

		/* 'default' enter sequence for state Driving */
		void TrackerStatechart::enseq_main_region_Driving_default()
		{
			/* 'default' enter sequence for state Driving */
			enseq_main_region_Driving_Tracking_default();
		}

		/* 'default' enter sequence for state First */
		void TrackerStatechart::enseq_main_region_Driving_Tracking_First_default()
		{
			/* 'default' enter sequence for state First */
			enact_main_region_Driving_Tracking_First();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First;
		}

		/* 'default' enter sequence for state Occupy */
		void TrackerStatechart::enseq_main_region_Driving_Tracking_Occupy_default()
		{
			/* 'default' enter sequence for state Occupy */
			enact_main_region_Driving_Tracking_Occupy();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy;
		}

		/* 'default' enter sequence for state Free */
		void TrackerStatechart::enseq_main_region_Driving_Tracking_Free_default()
		{
			/* 'default' enter sequence for state Free */
			enact_main_region_Driving_Tracking_Free();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free;
		}

		/* 'default' enter sequence for state Idle */
		void TrackerStatechart::enseq_main_region_Idle_default()
		{
			/* 'default' enter sequence for state Idle */
			enact_main_region_Idle();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Idle;
		}

		/* 'default' enter sequence for region main region */
		void TrackerStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region Tracking */
		void TrackerStatechart::enseq_main_region_Driving_Tracking_default()
		{
			/* 'default' enter sequence for region Tracking */
			react_main_region_Driving_Tracking__entry_Default();
		}

		/* Default exit sequence for state Preparing */
		void TrackerStatechart::exseq_main_region_Preparing()
		{
			/* Default exit sequence for state Preparing */
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::NO_STATE;
			exact_main_region_Preparing();
		}

		/* Default exit sequence for state Driving */
		void TrackerStatechart::exseq_main_region_Driving()
		{
			/* Default exit sequence for state Driving */
			exseq_main_region_Driving_Tracking();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state First */
		void TrackerStatechart::exseq_main_region_Driving_Tracking_First()
		{
			/* Default exit sequence for state First */
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving;
			exact_main_region_Driving_Tracking_First();
		}

		/* Default exit sequence for state Occupy */
		void TrackerStatechart::exseq_main_region_Driving_Tracking_Occupy()
		{
			/* Default exit sequence for state Occupy */
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving;
			exact_main_region_Driving_Tracking_Occupy();
		}

		/* Default exit sequence for state Free */
		void TrackerStatechart::exseq_main_region_Driving_Tracking_Free()
		{
			/* Default exit sequence for state Free */
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::main_region_Driving;
			exact_main_region_Driving_Tracking_Free();
		}

		/* Default exit sequence for state Idle */
		void TrackerStatechart::exseq_main_region_Idle()
		{
			/* Default exit sequence for state Idle */
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void TrackerStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.TrackerStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::TrackerStatechart::State::main_region_Preparing :
				{
					exseq_main_region_Preparing();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving :
				{
					exseq_main_region_Driving();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First :
				{
					exseq_main_region_Driving_Tracking_First();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy :
				{
					exseq_main_region_Driving_Tracking_Occupy();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free :
				{
					exseq_main_region_Driving_Tracking_Free();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Idle :
				{
					exseq_main_region_Idle();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Tracking */
		void TrackerStatechart::exseq_main_region_Driving_Tracking()
		{
			/* Default exit sequence for region Tracking */
			/* Handle exit of all possible states (of mrw.statechart.TrackerStatechart.main_region.Driving.Tracking) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First :
				{
					exseq_main_region_Driving_Tracking_First();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy :
				{
					exseq_main_region_Driving_Tracking_Occupy();
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free :
				{
					exseq_main_region_Driving_Tracking_Free();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void TrackerStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->valid())
			{
				enseq_main_region_Driving_default();
			}
			else
			{
				enseq_main_region_Idle_default();
			}
		}

		/* Default react sequence for initial entry  */
		void TrackerStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Idle_default();
		}

		/* Default react sequence for initial entry  */
		void TrackerStatechart::react_main_region_Driving_Tracking__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Driving_Tracking_First_default();
		}

		sc::integer TrackerStatechart::main_region_Preparing_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Preparing. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (received_raised)
				{
					exseq_main_region_Preparing();
					enseq_main_region_Preparing_default();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Preparing();
						timeEvents[0] = false;
						react_main_region__choice_0();
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

		sc::integer TrackerStatechart::main_region_Driving_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Driving. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((completed_raised)) && ((ifaceOperationCallback->last())))
				{
					exseq_main_region_Driving();
					enseq_main_region_Idle_default();
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

		sc::integer TrackerStatechart::main_region_Driving_Tracking_First_react(const sc::integer transitioned_before)
		{
			/* The reactions of state First. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Driving_Tracking_First();
					timeEvents[1] = false;
					enseq_main_region_Driving_Tracking_Occupy_default();
					main_region_Driving_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Driving_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer TrackerStatechart::main_region_Driving_Tracking_Occupy_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Occupy. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[2])
				{
					exseq_main_region_Driving_Tracking_Occupy();
					timeEvents[2] = false;
					enseq_main_region_Driving_Tracking_Free_default();
					main_region_Driving_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Driving_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer TrackerStatechart::main_region_Driving_Tracking_Free_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Free. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[3])
				{
					exseq_main_region_Driving_Tracking_Free();
					timeEvents[3] = false;
					enseq_main_region_Driving_Tracking_Occupy_default();
					main_region_Driving_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Driving_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer TrackerStatechart::main_region_Idle_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Idle. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (received_raised)
				{
					exseq_main_region_Idle();
					enseq_main_region_Preparing_default();
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

		void TrackerStatechart::clearInEvents() noexcept
		{
			received_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
			timeEvents[3] = false;
		}

		void TrackerStatechart::clearInternalEvents() noexcept
		{
			completed_raised = false;
		}

		void TrackerStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::TrackerStatechart::State::main_region_Preparing :
				{
					main_region_Preparing_react(-1);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First :
				{
					main_region_Driving_Tracking_First_react(-1);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy :
				{
					main_region_Driving_Tracking_Occupy_react(-1);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free :
				{
					main_region_Driving_Tracking_Free_react(-1);
					break;
				}
			case mrw::statechart::TrackerStatechart::State::main_region_Idle :
				{
					main_region_Idle_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void TrackerStatechart::runCycle()
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
				clearInternalEvents();
			}
			while (dispatchEvent(getNextEvent()));
			isExecuting = false;
		}

		void TrackerStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart TrackerStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void TrackerStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart TrackerStatechart */
			exseq_main_region();
			stateConfVector[0] = mrw::statechart::TrackerStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void TrackerStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
