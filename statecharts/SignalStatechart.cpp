/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

		const sc::integer SignalStatechart::STOP = 0;
		const sc::integer SignalStatechart::GO = 1;
		const sc::integer SignalStatechart::OFF = -1;



		SignalStatechart::SignalStatechart(QObject * parent) :
			QObject(parent),
			timeout(2000),
			symbolMain(SignalStatechart::STOP),
			symbolDistant(SignalStatechart::STOP),
			symbolShunt(SignalStatechart::STOP),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			stateConfVectorChanged(false),
			start_raised(false),
			queued_raised(false),
			response_raised(false),
			fail_raised(false),
			clear_raised(false)
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



		mrw::statechart::SignalStatechart::EventInstance * SignalStatechart::getNextEvent()
		{
			mrw::statechart::SignalStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void SignalStatechart::dispatchEvent(mrw::statechart::SignalStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SignalStatechart::Event::start:
				{
					start_raised = true;
					break;
				}
			case mrw::statechart::SignalStatechart::Event::queued:
				{
					queued_raised = true;
					break;
				}
			case mrw::statechart::SignalStatechart::Event::response:
				{
					response_raised = true;
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


			case mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::SignalStatechart::start()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::start));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::queued()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::queued));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::response()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::response));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::fail()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::fail));
			runCycle();
		}


		void mrw::statechart::SignalStatechart::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::clear));
			runCycle();
		}



		bool SignalStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SignalStatechart::isFinal() const
		{
			return false;
		}

		bool SignalStatechart::check() const
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


		void SignalStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * SignalStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer SignalStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void SignalStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SignalStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_))));
				runCycle();
			}
		}


		bool SignalStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] >= mrw::statechart::SignalStatechart::State::main_region_Init && stateConfVector[scvi_main_region_Init] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Main_signal] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal && stateConfVector[scvi_main_region_Init_Init_process_Main_signal] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Main_signal_Turn_main_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Main_signal_Turn_main_Pending] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Distant_signal] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal && stateConfVector[scvi_main_region_Init_Init_process_Distant_signal] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Shunt_signal] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal && stateConfVector[scvi_main_region_Init_Init_process_Shunt_signal] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Start] == mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] == mrw::statechart::SignalStatechart::State::main_region_Operating);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::SignalStatechart::State::main_region_Failed);
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

		sc::integer SignalStatechart::getTimeout() const
		{
			return timeout;
		}

		void SignalStatechart::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		sc::integer SignalStatechart::getSTOP()
		{
			return STOP;
		}

		sc::integer SignalStatechart::getGO()
		{
			return GO;
		}

		sc::integer SignalStatechart::getOFF()
		{
			return OFF;
		}

		sc::integer SignalStatechart::getSymbolMain() const
		{
			return symbolMain;
		}

		void SignalStatechart::setSymbolMain(sc::integer symbolMain_)
		{
			this->symbolMain = symbolMain_;
		}

		sc::integer SignalStatechart::getSymbolDistant() const
		{
			return symbolDistant;
		}

		void SignalStatechart::setSymbolDistant(sc::integer symbolDistant_)
		{
			this->symbolDistant = symbolDistant_;
		}

		sc::integer SignalStatechart::getSymbolShunt() const
		{
			return symbolShunt;
		}

		void SignalStatechart::setSymbolShunt(sc::integer symbolShunt_)
		{
			this->symbolShunt = symbolShunt_;
		}

		void SignalStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Init'. */
		void SignalStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 0, timeout, false);
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Main_signal_Turn_main_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnMainSignal(symbolMain);
		}

		/* Entry action for state 'Distant signal'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Distant_signal()
		{
			/* Entry action for state 'Distant signal'. */
			if (ifaceOperationCallback->hasMain())
			{
				symbolDistant = SignalStatechart::OFF;
			}
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnDistantSignal(symbolDistant);
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnShuntSignal(symbolShunt);
		}

		/* Exit action for state 'Init'. */
		void SignalStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 0);
		}

		/* 'default' enter sequence for state Init */
		void SignalStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			enseq_main_region_Init_Init_process_default();
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Pending */
		void SignalStatechart::enseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Pending */
		void SignalStatechart::enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Pending */
		void SignalStatechart::enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_default()
		{
			/* 'default' enter sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Wait for Start */
		void SignalStatechart::enseq_main_region_Wait_for_Start_default()
		{
			/* 'default' enter sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Operating */
		void SignalStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Operating;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Failed */
		void SignalStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Failed;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for region main region */
		void SignalStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region Init process */
		void SignalStatechart::enseq_main_region_Init_Init_process_default()
		{
			/* 'default' enter sequence for region Init process */
			react_main_region_Init_Init_process__entry_Default();
		}

		/* Default exit sequence for state Init */
		void SignalStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			exseq_main_region_Init_Init_process();
			exact_main_region_Init();
		}

		/* Default exit sequence for state Main signal */
		void SignalStatechart::exseq_main_region_Init_Init_process_Main_signal()
		{
			/* Default exit sequence for state Main signal */
			exseq_main_region_Init_Init_process_Main_signal_Turn_main();
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Pending */
		void SignalStatechart::exseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Distant signal */
		void SignalStatechart::exseq_main_region_Init_Init_process_Distant_signal()
		{
			/* Default exit sequence for state Distant signal */
			exseq_main_region_Init_Init_process_Distant_signal_Turn_distant();
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Pending */
		void SignalStatechart::exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Pending */
		void SignalStatechart::exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending()
		{
			/* Default exit sequence for state Pending */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Wait for Start */
		void SignalStatechart::exseq_main_region_Wait_for_Start()
		{
			/* Default exit sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Operating */
		void SignalStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Failed */
		void SignalStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void SignalStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn :
				{
					exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending :
				{
					exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start :
				{
					exseq_main_region_Wait_for_Start();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Operating :
				{
					exseq_main_region_Operating();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Init process */
		void SignalStatechart::exseq_main_region_Init_Init_process()
		{
			/* Default exit sequence for region Init process */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn :
				{
					exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending :
				{
					exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Turn main */
		void SignalStatechart::exseq_main_region_Init_Init_process_Main_signal_Turn_main()
		{
			/* Default exit sequence for region Turn main */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process.Main_signal.Turn_main) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending :
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Turn distant */
		void SignalStatechart::exseq_main_region_Init_Init_process_Distant_signal_Turn_distant()
		{
			/* Default exit sequence for region Turn distant */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process.Distant_signal.Turn_distant) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending :
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasMain())
			{
				enseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn_default();
			}
			else
			{
				react_main_region_Init_Init_process__choice_1();
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process__choice_1()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasDistant())
			{
				enact_main_region_Init_Init_process_Distant_signal();
				enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_default();
			}
			else
			{
				react_main_region_Init_Init_process__choice_2();
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process__choice_2()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasShunt())
			{
				enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_default();
			}
			else
			{
				exseq_main_region_Init();
				ifaceOperationCallback->dec();
				enseq_main_region_Operating_default();
				react(0);
			}
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Init_Init_process__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_process__choice_0();
		}

		sc::integer SignalStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer SignalStatechart::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (fail_raised)
				{
					exseq_main_region_Init();
					enseq_main_region_Failed_default();
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
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Main_signal_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Main signal. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Init_Init_process_Main_signal();
					react_main_region_Init_Init_process__choice_1();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Main_signal_Turn_main_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
					enseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending_default();
					main_region_Init_Init_process_Main_signal_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Main_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Main_signal_Turn_main_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Main_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Distant_signal_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Distant signal. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Init_Init_process_Distant_signal();
					react_main_region_Init_Init_process__choice_2();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
					enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_default();
					main_region_Init_Init_process_Distant_signal_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Distant_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Distant_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Shunt_signal_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Shunt signal. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (response_raised)
				{
					exseq_main_region_Init();
					ifaceOperationCallback->dec();
					enseq_main_region_Operating_default();
					react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (queued_raised)
				{
					exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
					enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_default();
					main_region_Init_Init_process_Shunt_signal_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Shunt_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Pending. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_process_Shunt_signal_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before)
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

		sc::integer SignalStatechart::main_region_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
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

		sc::integer SignalStatechart::main_region_Failed_react(const sc::integer transitioned_before)
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

		void SignalStatechart::clearInEvents()
		{
			start_raised = false;
			queued_raised = false;
			response_raised = false;
			fail_raised = false;
			clear_raised = false;
			timeEvents[0] = false;
		}

		void SignalStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Turn :
				{
					main_region_Init_Init_process_Main_signal_Turn_main_Turn_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Main_signal_Turn_main_Pending :
				{
					main_region_Init_Init_process_Main_signal_Turn_main_Pending_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Turn :
				{
					main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Distant_signal_Turn_distant_Pending :
				{
					main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn :
				{
					main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending :
				{
					main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start :
				{
					main_region_Wait_for_Start_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Operating :
				{
					main_region_Operating_react(-1);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
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
				do
				{
					stateConfVectorChanged = false;
					microStep();
				}
				while (stateConfVectorChanged);
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while ((((((start_raised) || (queued_raised)) || (response_raised)) || (fail_raised)) || (clear_raised)) || (timeEvents[0]));
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
			do
			{
				stateConfVectorChanged = false;
				microStep();
			}
			while (stateConfVectorChanged);
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
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SignalStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
