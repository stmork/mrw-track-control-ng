/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#include "RouteStatechart.h"

/*! \file
Implementation of the state machine 'RouteStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		RouteStatechart::RouteStatechart(QObject * parent) :
			QObject(parent),
			timeout(2000),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			completed(false),
			doCompletion(false),
			isExecuting(false),
			stateConfVectorChanged(false),
			turned_raised(false),
			failed_raised(false),
			disable_raised(false),
			extended_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::RouteStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		RouteStatechart::~RouteStatechart()
		{
		}



		mrw::statechart::RouteStatechart::EventInstance * RouteStatechart::getNextEvent()
		{
			mrw::statechart::RouteStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void RouteStatechart::dispatchEvent(mrw::statechart::RouteStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::RouteStatechart::Event::turned:
				{
					turned_raised = true;
					break;
				}
			case mrw::statechart::RouteStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}
			case mrw::statechart::RouteStatechart::Event::disable:
				{
					disable_raised = true;
					break;
				}
			case mrw::statechart::RouteStatechart::Event::extended:
				{
					extended_raised = true;
					break;
				}


			case mrw::statechart::RouteStatechart::Event::_te0_main_region_Switch_Turning_:
			case mrw::statechart::RouteStatechart::Event::_te1_main_region_Section_Activation_:
			case mrw::statechart::RouteStatechart::Event::_te2_main_region_Signal_Turning_:
			case mrw::statechart::RouteStatechart::Event::_te3_main_region_Active_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::RouteStatechart::Event::_te0_main_region_Switch_Turning_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::RouteStatechart::turned()
		{
			incomingEventQueue.push_back(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::turned));
			runCycle();
		}


		void mrw::statechart::RouteStatechart::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::failed));
			runCycle();
		}


		void mrw::statechart::RouteStatechart::disable()
		{
			incomingEventQueue.push_back(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::disable));
			runCycle();
		}


		void mrw::statechart::RouteStatechart::extended()
		{
			incomingEventQueue.push_back(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::extended));
			runCycle();
		}



		bool RouteStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool RouteStatechart::isFinal() const
		{
			return false;
		}

		bool RouteStatechart::check() const
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


		void RouteStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * RouteStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer RouteStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void RouteStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::RouteStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::RouteStatechart::Event::_te0_main_region_Switch_Turning_))));
				runCycle();
			}
		}


		bool RouteStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::RouteStatechart::State::main_region_Switch_Turning :
				{
					return  (stateConfVector[scvi_main_region_Switch_Turning] == mrw::statechart::RouteStatechart::State::main_region_Switch_Turning);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Section_Activation :
				{
					return  (stateConfVector[scvi_main_region_Section_Activation] == mrw::statechart::RouteStatechart::State::main_region_Section_Activation);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Signal_Turning :
				{
					return  (stateConfVector[scvi_main_region_Signal_Turning] == mrw::statechart::RouteStatechart::State::main_region_Signal_Turning);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active :
				{
					return  (stateConfVector[scvi_main_region_Active] == mrw::statechart::RouteStatechart::State::main_region_Active);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Disable :
				{
					return  (stateConfVector[scvi_main_region_Disable] == mrw::statechart::RouteStatechart::State::main_region_Disable);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Start :
				{
					return  (stateConfVector[scvi_main_region_Start] == mrw::statechart::RouteStatechart::State::main_region_Start);
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

		sc::integer RouteStatechart::getTimeout() const
		{
			return timeout;
		}

		void RouteStatechart::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		void RouteStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Switch Turning'. */
		void RouteStatechart::enact_main_region_Switch_Turning()
		{
			/* Entry action for state 'Switch Turning'. */
			timerService->setTimer(this, 0, timeout, false);
			ifaceOperationCallback->turnSwitches();
		}

		/* Entry action for state 'Section Activation'. */
		void RouteStatechart::enact_main_region_Section_Activation()
		{
			/* Entry action for state 'Section Activation'. */
			timerService->setTimer(this, 1, timeout, false);
			ifaceOperationCallback->activateSections();
		}

		/* Entry action for state 'Signal Turning'. */
		void RouteStatechart::enact_main_region_Signal_Turning()
		{
			/* Entry action for state 'Signal Turning'. */
			timerService->setTimer(this, 2, timeout, false);
			ifaceOperationCallback->turnSignals();
		}

		void RouteStatechart::enact_main_region_Active()
		{
			/* Entry action for state 'Active'. */
			timerService->setTimer(this, 3, timeout, false);
			completed = true;
		}

		/* Entry action for state 'Disable'. */
		void RouteStatechart::enact_main_region_Disable()
		{
			/* Entry action for state 'Disable'. */
			ifaceOperationCallback->deactivateSections();
		}

		/* Exit action for state 'Switch Turning'. */
		void RouteStatechart::exact_main_region_Switch_Turning()
		{
			/* Exit action for state 'Switch Turning'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Section Activation'. */
		void RouteStatechart::exact_main_region_Section_Activation()
		{
			/* Exit action for state 'Section Activation'. */
			timerService->unsetTimer(this, 1);
		}

		/* Exit action for state 'Signal Turning'. */
		void RouteStatechart::exact_main_region_Signal_Turning()
		{
			/* Exit action for state 'Signal Turning'. */
			timerService->unsetTimer(this, 2);
		}

		/* Exit action for state 'Active'. */
		void RouteStatechart::exact_main_region_Active()
		{
			/* Exit action for state 'Active'. */
			timerService->unsetTimer(this, 3);
		}

		/* 'default' enter sequence for state Switch Turning */
		void RouteStatechart::enseq_main_region_Switch_Turning_default()
		{
			/* 'default' enter sequence for state Switch Turning */
			enact_main_region_Switch_Turning();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Switch_Turning;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Section Activation */
		void RouteStatechart::enseq_main_region_Section_Activation_default()
		{
			/* 'default' enter sequence for state Section Activation */
			enact_main_region_Section_Activation();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Section_Activation;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Signal Turning */
		void RouteStatechart::enseq_main_region_Signal_Turning_default()
		{
			/* 'default' enter sequence for state Signal Turning */
			enact_main_region_Signal_Turning();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Signal_Turning;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Active */
		void RouteStatechart::enseq_main_region_Active_default()
		{
			/* 'default' enter sequence for state Active */
			enact_main_region_Active();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Disable */
		void RouteStatechart::enseq_main_region_Disable_default()
		{
			/* 'default' enter sequence for state Disable */
			enact_main_region_Disable();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Disable;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Start */
		void RouteStatechart::enseq_main_region_Start_default()
		{
			/* 'default' enter sequence for state Start */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Start;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for region main region */
		void RouteStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Switch Turning */
		void RouteStatechart::exseq_main_region_Switch_Turning()
		{
			/* Default exit sequence for state Switch Turning */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Switch_Turning();
		}

		/* Default exit sequence for state Section Activation */
		void RouteStatechart::exseq_main_region_Section_Activation()
		{
			/* Default exit sequence for state Section Activation */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Section_Activation();
		}

		/* Default exit sequence for state Signal Turning */
		void RouteStatechart::exseq_main_region_Signal_Turning()
		{
			/* Default exit sequence for state Signal Turning */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Signal_Turning();
		}

		/* Default exit sequence for state Active */
		void RouteStatechart::exseq_main_region_Active()
		{
			/* Default exit sequence for state Active */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Active();
		}

		/* Default exit sequence for state Disable */
		void RouteStatechart::exseq_main_region_Disable()
		{
			/* Default exit sequence for state Disable */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Start */
		void RouteStatechart::exseq_main_region_Start()
		{
			/* Default exit sequence for state Start */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void RouteStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.RouteStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::RouteStatechart::State::main_region_Switch_Turning :
				{
					exseq_main_region_Switch_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Section_Activation :
				{
					exseq_main_region_Section_Activation();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Signal_Turning :
				{
					exseq_main_region_Signal_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active :
				{
					exseq_main_region_Active();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Disable :
				{
					exseq_main_region_Disable();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Start :
				{
					exseq_main_region_Start();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default react sequence for initial entry  */
		void RouteStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Start_default();
		}

		sc::integer RouteStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer RouteStatechart::main_region_Switch_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Switch Turning. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if (turned_raised)
					{
						exseq_main_region_Switch_Turning();
						enseq_main_region_Section_Activation_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (timeEvents[0])
						{
							exseq_main_region_Switch_Turning();
							timeEvents[0] = false;
							enseq_main_region_Disable_default();
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
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Section_Activation_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Section Activation. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if (turned_raised)
					{
						exseq_main_region_Section_Activation();
						enseq_main_region_Signal_Turning_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (timeEvents[1])
						{
							exseq_main_region_Section_Activation();
							timeEvents[1] = false;
							enseq_main_region_Disable_default();
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
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Signal_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Signal Turning. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if (turned_raised)
					{
						exseq_main_region_Signal_Turning();
						enseq_main_region_Active_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (timeEvents[2])
						{
							exseq_main_region_Signal_Turning();
							timeEvents[2] = false;
							enseq_main_region_Disable_default();
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
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Active. */
			sc::integer transitioned_after = transitioned_before;
			if (doCompletion)
			{
				/* Default exit sequence for state Active */
				stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
				exact_main_region_Active();
				/* 'default' enter sequence for state Switch Turning */
				enact_main_region_Switch_Turning();
				stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Switch_Turning;
				stateConfVectorChanged = true;
				react(0);
			}
			else
			{
				if ((transitioned_after) < (0))
				{
					if (timeEvents[3])
					{
						exseq_main_region_Active();
						timeEvents[3] = false;
						enseq_main_region_Disable_default();
						react(0);
						transitioned_after = 0;
					}
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Disable_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Disable. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Start_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Start. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if (extended_raised)
					{
						exseq_main_region_Start();
						enseq_main_region_Switch_Turning_default();
						react(0);
						transitioned_after = 0;
					}
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		void RouteStatechart::clearInEvents()
		{
			turned_raised = false;
			failed_raised = false;
			disable_raised = false;
			extended_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
			timeEvents[3] = false;
		}

		void RouteStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::RouteStatechart::State::main_region_Switch_Turning :
				{
					main_region_Switch_Turning_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Section_Activation :
				{
					main_region_Section_Activation_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Signal_Turning :
				{
					main_region_Signal_Turning_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active :
				{
					main_region_Active_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Disable :
				{
					main_region_Disable_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Start :
				{
					main_region_Start_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void RouteStatechart::runCycle()
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
				doCompletion = false;
				do
				{
					if (completed)
					{
						doCompletion = true;
					}
					completed = false;
					do
					{
						stateConfVectorChanged = false;
						microStep();
					}
					while (stateConfVectorChanged);
					doCompletion = false;
				}
				while (completed);
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while ((((((((turned_raised) || (failed_raised)) || (disable_raised)) || (extended_raised)) || (timeEvents[0])) || (timeEvents[1])) || (timeEvents[2])) || (timeEvents[3]));
			isExecuting = false;
		}

		void RouteStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart RouteStatechart */
			enseq_main_region_default();
			do
			{
				stateConfVectorChanged = false;
				doCompletion = false;
				do
				{
					if (completed)
					{
						doCompletion = true;
					}
					completed = false;
					microStep();
					doCompletion = false;
				}
				while (completed);
			}
			while (stateConfVectorChanged);
			isExecuting = false;
		}

		void RouteStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart RouteStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void RouteStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
