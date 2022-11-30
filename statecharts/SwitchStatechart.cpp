/* Copyright (C) Steffen A. Mork */

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
			inquire_raised(false),
			leftResponse_raised(false),
			rightResponse_raised(false),
			response_raised(false),
			clear_raised(false),
			failed_raised(false)
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
			case mrw::statechart::SwitchStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}


			case mrw::statechart::SwitchStatechart::Event::_te0_main_region_Init_:
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


		void mrw::statechart::SwitchStatechart::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::SwitchStatechart::EventInstance(mrw::statechart::SwitchStatechart::Event::failed));
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
					return  (stateConfVector[scvi_main_region_Operating] == mrw::statechart::SwitchStatechart::State::main_region_Operating);
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
		}

		/* Entry action for state 'Operating'. */
		void SwitchStatechart::enact_main_region_Operating()
		{
			/* Entry action for state 'Operating'. */
			emit inquired();
		}

		/* Exit action for state 'Init'. */
		void SwitchStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 0);
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
			enact_main_region_Operating();
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Operating;
		}

		/* 'default' enter sequence for state Failed */
		void SwitchStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SwitchStatechart::State::main_region_Failed;
		}

		/* 'default' enter sequence for region main region */
		void SwitchStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating :
				{
					exseq_main_region_Operating();
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

		/* Default react sequence for initial entry  */
		void SwitchStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
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
					emit entering();
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
			failed_raised = false;
			timeEvents[0] = false;
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
			case mrw::statechart::SwitchStatechart::State::main_region_Operating :
				{
					main_region_Operating_react(-1);
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
				dispatchEvent(getNextEvent());
			}
			while (((((((inquire_raised) || (leftResponse_raised)) || (rightResponse_raised)) || (response_raised)) || (clear_raised)) || (failed_raised)) || (timeEvents[0]));
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
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SwitchStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
