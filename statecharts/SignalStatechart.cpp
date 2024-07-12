/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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



		SignalStatechart::SignalStatechart(QObject * parent) noexcept :
			symbol(SignalStatechart::STOP),
			turn_raised(false),
			turn_value
			(0),
			queued_raised(false),
			ok_raised(false),
			fail_raised(false),
			clear_raised(false),
			modified(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
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



		std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance> SignalStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance> nextEvent = 0;

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

		bool SignalStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SignalStatechart::Event::turn:
				{
					std::unique_ptr<mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer>> e = cast_event_pointer_type<mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer> >(std::move(event));

					if (e != 0)
					{
						turn_value
							= e->value;
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
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'turn' that is defined in the default interface scope. */
		void mrw::statechart::SignalStatechart::turn(sc::integer turn_)
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer>>( new mrw::statechart::SignalStatechart::EventInstanceWithValue<sc::integer>(mrw::statechart::SignalStatechart::Event::turn, turn_)))
			;
			runCycle();
		}


		/*! Slot for the in event 'queued' that is defined in the default interface scope. */
		void mrw::statechart::SignalStatechart::queued()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance>(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::queued)))
			;
			runCycle();
		}


		/*! Slot for the in event 'ok' that is defined in the default interface scope. */
		void mrw::statechart::SignalStatechart::ok()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance>(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::ok)))
			;
			runCycle();
		}


		/*! Slot for the in event 'fail' that is defined in the default interface scope. */
		void mrw::statechart::SignalStatechart::fail()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance>(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::fail)))
			;
			runCycle();
		}


		/*! Slot for the in event 'clear' that is defined in the default interface scope. */
		void mrw::statechart::SignalStatechart::clear()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::SignalStatechart::EventInstance>(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::clear)))
			;
			runCycle();
		}



		bool SignalStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SignalStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool SignalStatechart::check() const noexcept
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


		void SignalStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> SignalStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer SignalStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void SignalStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::SignalStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Turning_)))));
				runCycle();
			}
		}


		bool SignalStatechart::isStateActive(State state) const noexcept
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
					return  (stateConfVector[scvi_main_region_Turning] >= mrw::statechart::SignalStatechart::State::main_region_Turning && stateConfVector[scvi_main_region_Turning] <= mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					return  (stateConfVector[scvi_main_region_Turning_Turn_processing_Pending] == mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send :
				{
					return  (stateConfVector[scvi_main_region_Turning_Turn_processing_Send] == mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send);
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

		sc::integer SignalStatechart::getTimeout() noexcept
		{
			return timeout
				;
		}

		sc::integer SignalStatechart::getSymbol() const noexcept
		{
			return symbol
				;
		}

		void SignalStatechart::setSymbol(sc::integer symbol_) noexcept
		{
			this->symbol = symbol_;
		}
		sc::integer SignalStatechart::getOFF() noexcept
		{
			return OFF
				;
		}

		sc::integer SignalStatechart::getSTOP() noexcept
		{
			return STOP
				;
		}

		sc::integer SignalStatechart::getGO() noexcept
		{
			return GO
				;
		}

		void SignalStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}
		void SignalStatechart::setModified(bool modified_) noexcept
		{
			this->modified = modified_;
		}

// implementations of all internal functions
		/* Entry action for state 'Turning'. */
		void SignalStatechart::enact_main_region_Turning()
		{
			/* Entry action for state 'Turning'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (SignalStatechart::timeout)), false);
			setModified(ifaceOperationCallback->prepare());
		}

		/* Entry action for state 'Send'. */
		void SignalStatechart::enact_main_region_Turning_Turn_processing_Send()
		{
			/* Entry action for state 'Send'. */
			ifaceOperationCallback->send();
		}

		/* Entry action for state 'Fail'. */
		void SignalStatechart::enact_main_region_Fail()
		{
			/* Entry action for state 'Fail'. */
			setSymbol(SignalStatechart::OFF);
			ifaceOperationCallback->prepare();
			ifaceOperationCallback->send();
			emit failed();
		}

		/* Exit action for state 'Turning'. */
		void SignalStatechart::exact_main_region_Turning()
		{
			/* Exit action for state 'Turning'. */
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* 'default' enter sequence for state Idle */
		void SignalStatechart::enseq_main_region_Idle_default()
		{
			/* 'default' enter sequence for state Idle */
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

		/* 'default' enter sequence for state Send */
		void SignalStatechart::enseq_main_region_Turning_Turn_processing_Send_default()
		{
			/* 'default' enter sequence for state Send */
			enact_main_region_Turning_Turn_processing_Send();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send;
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
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			exact_main_region_Turning();
		}

		/* Default exit sequence for state Pending */
		void SignalStatechart::exseq_main_region_Turning_Turn_processing_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Turning;
		}

		/* Default exit sequence for state Send */
		void SignalStatechart::exseq_main_region_Turning_Turn_processing_Send()
		{
			/* Default exit sequence for state Send */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Turning;
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
			case mrw::statechart::SignalStatechart::State::main_region_Turning :
				{
					exseq_main_region_Turning();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending :
				{
					exseq_main_region_Turning_Turn_processing_Pending();
					exact_main_region_Turning();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send :
				{
					exseq_main_region_Turning_Turn_processing_Send();
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
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send :
				{
					exseq_main_region_Turning_Turn_processing_Send();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Turning_Turn_processing__choice_0()
		{
			/* The reactions of state null. */
			if (modified)
			{
				enseq_main_region_Turning_Turn_processing_Send_default();
			}
			else
			{
				exseq_main_region_Turning();
				emit completed();
				enseq_main_region_Idle_default();
				react(0);
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasSignal())
			{
				setSymbol(turn_value);
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
			setSymbol(SignalStatechart::STOP);
			enseq_main_region_Idle_default();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Turning_Turn_processing__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Turning_Turn_processing__choice_0();
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
				else
				{
					if (((fail_raised)) && ((ifaceOperationCallback->hasSignal())))
					{
						exseq_main_region_Idle();
						enseq_main_region_Fail_default();
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

		sc::integer SignalStatechart::main_region_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turning. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (ok_raised)
				{
					exseq_main_region_Turning();
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Turning_Turn_processing_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Turning_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Turning_Turn_processing_Send_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Send. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Turning_Turn_processing_Send();
					enseq_main_region_Turning_Turn_processing_Pending_default();
					main_region_Turning_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		void SignalStatechart::clearInEvents() noexcept
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
			case mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send :
				{
					main_region_Turning_Turn_processing_Send_react(-1);
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
			}
			while (dispatchEvent(getNextEvent()));
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
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SignalStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
