/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */

#include "ConfigStatechart.h"

/*! \file
Implementation of the state machine 'ConfigStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		ConfigStatechart::ConfigStatechart(QObject * parent) noexcept :
			idx(0),
			max(0),
			connected_raised(false),
			completed_raised(false),
			written(0),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::ConfigStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		ConfigStatechart::~ConfigStatechart()
		{
		}



		std::unique_ptr<mrw::statechart::ConfigStatechart::EventInstance> ConfigStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::ConfigStatechart::EventInstance> nextEvent = 0;

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

		bool ConfigStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::ConfigStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::ConfigStatechart::Event::connected:
				{
					connected_raised = true;
					break;
				}
			case mrw::statechart::ConfigStatechart::Event::completed:
				{
					completed_raised = true;
					break;
				}


			case mrw::statechart::ConfigStatechart::Event::_te0_main_region_Wait_for_Connect_:
			case mrw::statechart::ConfigStatechart::Event::_te1_main_region_Configure_:
			case mrw::statechart::ConfigStatechart::Event::_te2_main_region_Wait_for_Boot_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::ConfigStatechart::Event::_te0_main_region_Wait_for_Connect_)] = true;
					break;
				}
			default:
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'connected' that is defined in the default interface scope. */
		void mrw::statechart::ConfigStatechart::connected()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::ConfigStatechart::EventInstance>(new mrw::statechart::ConfigStatechart::EventInstance(mrw::statechart::ConfigStatechart::Event::connected)))
			;
			runCycle();
		}


		/*! Slot for the in event 'completed' that is defined in the default interface scope. */
		void mrw::statechart::ConfigStatechart::completed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::ConfigStatechart::EventInstance>(new mrw::statechart::ConfigStatechart::EventInstance(mrw::statechart::ConfigStatechart::Event::completed)))
			;
			runCycle();
		}



		bool ConfigStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::ConfigStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool ConfigStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool ConfigStatechart::check() const noexcept
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


		void ConfigStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> ConfigStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer ConfigStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void ConfigStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::ConfigStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::ConfigStatechart::Event::_te0_main_region_Wait_for_Connect_)))));
				runCycle();
			}
		}


		bool ConfigStatechart::isStateActive(State state) const noexcept
		{
			switch (state)
			{
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Connect] == mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Configure :
				{
					return  (stateConfVector[scvi_main_region_Configure] == mrw::statechart::ConfigStatechart::State::main_region_Configure);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Boot] == mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::ConfigStatechart::State::main_region_Failed);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Booted :
				{
					return  (stateConfVector[scvi_main_region_Booted] == mrw::statechart::ConfigStatechart::State::main_region_Booted);
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

		sc::integer ConfigStatechart::getTimeout() noexcept
		{
			return timeout
				;
		}

		sc::integer ConfigStatechart::getWritetime() noexcept
		{
			return writetime
				;
		}

		sc::integer ConfigStatechart::getFlashtime() noexcept
		{
			return flashtime
				;
		}

		sc::integer ConfigStatechart::getResettime() noexcept
		{
			return resettime
				;
		}

		sc::integer ConfigStatechart::getIdx() const noexcept
		{
			return idx
				;
		}

		void ConfigStatechart::setIdx(sc::integer idx_) noexcept
		{
			this->idx = idx_;
		}
		sc::integer ConfigStatechart::getMax() const noexcept
		{
			return max
				;
		}

		void ConfigStatechart::setMax(sc::integer max_) noexcept
		{
			this->max = max_;
		}
		void ConfigStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}
		void ConfigStatechart::setWritten(sc::integer written_) noexcept
		{
			this->written = written_;
		}

// implementations of all internal functions
		/* Entry action for state 'Wait for Connect'. */
		void ConfigStatechart::enact_main_region_Wait_for_Connect()
		{
			/* Entry action for state 'Wait for Connect'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (ConfigStatechart::timeout)), false);
			setIdx(0);
			setMax(0);
		}

		/* Entry action for state 'Configure'. */
		void ConfigStatechart::enact_main_region_Configure()
		{
			/* Entry action for state 'Configure'. */
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (ConfigStatechart::writetime)), false);
			setWritten(ifaceOperationCallback->configure(idx));
			setMax((written) > (max) ? written : max);
		}

		/* Entry action for state 'Wait for Boot'. */
		void ConfigStatechart::enact_main_region_Wait_for_Boot()
		{
			/* Entry action for state 'Wait for Boot'. */
			timerService->setTimer(shared_from_this(), 2, (((static_cast<sc::time> (ConfigStatechart::flashtime)) * max) + ConfigStatechart::resettime), false);
			ifaceOperationCallback->booting();
		}

		/* Entry action for state 'Failed'. */
		void ConfigStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail();
		}

		/* Entry action for state 'Booted'. */
		void ConfigStatechart::enact_main_region_Booted()
		{
			/* Entry action for state 'Booted'. */
			ifaceOperationCallback->quit();
		}

		/* Exit action for state 'Wait for Connect'. */
		void ConfigStatechart::exact_main_region_Wait_for_Connect()
		{
			/* Exit action for state 'Wait for Connect'. */
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* Exit action for state 'Configure'. */
		void ConfigStatechart::exact_main_region_Configure()
		{
			/* Exit action for state 'Configure'. */
			timerService->unsetTimer(shared_from_this(), 1);
			idx++;
		}

		/* Exit action for state 'Wait for Boot'. */
		void ConfigStatechart::exact_main_region_Wait_for_Boot()
		{
			/* Exit action for state 'Wait for Boot'. */
			timerService->unsetTimer(shared_from_this(), 2);
		}

		/* 'default' enter sequence for state Wait for Connect */
		void ConfigStatechart::enseq_main_region_Wait_for_Connect_default()
		{
			/* 'default' enter sequence for state Wait for Connect */
			enact_main_region_Wait_for_Connect();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect;
		}

		/* 'default' enter sequence for state Configure */
		void ConfigStatechart::enseq_main_region_Configure_default()
		{
			/* 'default' enter sequence for state Configure */
			enact_main_region_Configure();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::main_region_Configure;
		}

		/* 'default' enter sequence for state Wait for Boot */
		void ConfigStatechart::enseq_main_region_Wait_for_Boot_default()
		{
			/* 'default' enter sequence for state Wait for Boot */
			enact_main_region_Wait_for_Boot();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot;
		}

		/* 'default' enter sequence for state Failed */
		void ConfigStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::main_region_Failed;
		}

		/* 'default' enter sequence for state Booted */
		void ConfigStatechart::enseq_main_region_Booted_default()
		{
			/* 'default' enter sequence for state Booted */
			enact_main_region_Booted();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::main_region_Booted;
		}

		/* 'default' enter sequence for region main region */
		void ConfigStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Wait for Connect */
		void ConfigStatechart::exseq_main_region_Wait_for_Connect()
		{
			/* Default exit sequence for state Wait for Connect */
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
			exact_main_region_Wait_for_Connect();
		}

		/* Default exit sequence for state Configure */
		void ConfigStatechart::exseq_main_region_Configure()
		{
			/* Default exit sequence for state Configure */
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
			exact_main_region_Configure();
		}

		/* Default exit sequence for state Wait for Boot */
		void ConfigStatechart::exseq_main_region_Wait_for_Boot()
		{
			/* Default exit sequence for state Wait for Boot */
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
			exact_main_region_Wait_for_Boot();
		}

		/* Default exit sequence for state Failed */
		void ConfigStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Booted */
		void ConfigStatechart::exseq_main_region_Booted()
		{
			/* Default exit sequence for state Booted */
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void ConfigStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.ConfigStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect :
				{
					exseq_main_region_Wait_for_Connect();
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Configure :
				{
					exseq_main_region_Configure();
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot :
				{
					exseq_main_region_Wait_for_Boot();
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Booted :
				{
					exseq_main_region_Booted();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void ConfigStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasMore(idx))
			{
				enseq_main_region_Configure_default();
			}
			else
			{
				enseq_main_region_Wait_for_Boot_default();
			}
		}

		/* Default react sequence for initial entry  */
		void ConfigStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Connect_default();
		}

		sc::integer ConfigStatechart::main_region_Wait_for_Connect_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait for Connect. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (connected_raised)
				{
					exseq_main_region_Wait_for_Connect();
					react_main_region__choice_0();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Wait_for_Connect();
						timeEvents[0] = false;
						enseq_main_region_Failed_default();
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

		sc::integer ConfigStatechart::main_region_Configure_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Configure. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Configure();
					timeEvents[1] = false;
					react_main_region__choice_0();
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

		sc::integer ConfigStatechart::main_region_Wait_for_Boot_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait for Boot. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[2])
				{
					exseq_main_region_Wait_for_Boot();
					timeEvents[2] = false;
					enseq_main_region_Failed_default();
					transitioned_after = 0;
				}
				else
				{
					if (completed_raised)
					{
						exseq_main_region_Wait_for_Boot();
						enseq_main_region_Booted_default();
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

		void ConfigStatechart::clearInEvents() noexcept
		{
			connected_raised = false;
			completed_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
		}

		void ConfigStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect :
				{
					main_region_Wait_for_Connect_react(-1);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Configure :
				{
					main_region_Configure_react(-1);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot :
				{
					main_region_Wait_for_Boot_react(-1);
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Failed :
				{
					break;
				}
			case mrw::statechart::ConfigStatechart::State::main_region_Booted :
				{
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void ConfigStatechart::runCycle()
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

		void ConfigStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart ConfigStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void ConfigStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart ConfigStatechart */
			exseq_main_region();
			stateConfVector[0] = mrw::statechart::ConfigStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void ConfigStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
