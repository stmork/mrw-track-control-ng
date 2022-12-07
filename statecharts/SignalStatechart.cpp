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
			completed(false),
			doCompletion(false),
			isExecuting(false),
			stateConfVectorPosition(0),
			start_raised(false),
			response_raised(false)
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
			case mrw::statechart::SignalStatechart::Event::response:
				{
					response_raised = true;
					break;
				}


			case mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_Init_process_Waiting_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_Init_process_Waiting_)] = true;
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


		void mrw::statechart::SignalStatechart::response()
		{
			incomingEventQueue.push_back(new mrw::statechart::SignalStatechart::EventInstance(mrw::statechart::SignalStatechart::Event::response));
			runCycle();
		}



		bool SignalStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SignalStatechart::State::NO_STATE || stateConfVector[1] != mrw::statechart::SignalStatechart::State::NO_STATE || stateConfVector[2] != mrw::statechart::SignalStatechart::State::NO_STATE;
		}

		bool SignalStatechart::isFinal() const
		{
			return (stateConfVector[0] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_) && (stateConfVector[1] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_) && (stateConfVector[2] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_);
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
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SignalStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SignalStatechart::Event::_te0_main_region_Init_Init_process_Waiting_))));
				runCycle();
			}
		}


		bool SignalStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] >= mrw::statechart::SignalStatechart::State::main_region_Init && stateConfVector[scvi_main_region_Init] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting && stateConfVector[scvi_main_region_Init_Init_process_Waiting] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_main_Turn] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn && stateConfVector[scvi_main_region_Init_Init_process_Waiting_main_Turn] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_ :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_distant_Turn] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn && stateConfVector[scvi_main_region_Init_Init_process_Waiting_distant_Turn] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_ :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_shunt_Turn] >= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn && stateConfVector[scvi_main_region_Init_Init_process_Waiting_shunt_Turn] <= mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_ :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_] == mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_);
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
		/* Entry action for state 'Waiting'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Waiting()
		{
			/* Entry action for state 'Waiting'. */
			timerService->setTimer(this, 0, timeout, false);
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnMainSignal(symbolMain);
		}

		/* Entry action for state 'Turn'. */
		void SignalStatechart::enact_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnDistantSignal(symbolDistant);
		}

		void SignalStatechart::enact_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn()
		{
			/* Entry action for state 'Turn'. */
			ifaceOperationCallback->turnShuntSignal(symbolShunt);
			completed = true;
		}

		/* Exit action for state 'Waiting'. */
		void SignalStatechart::exact_main_region_Init_Init_process_Waiting()
		{
			/* Exit action for state 'Waiting'. */
			timerService->unsetTimer(this, 0);
		}

		/* 'default' enter sequence for state Init */
		void SignalStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enseq_main_region_Init_Init_process_default();
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_main_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_default();
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn();
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn;
			stateConfVectorPosition = 0;
		}

		/* Default enter sequence for final state */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_;
			stateConfVectorPosition = 0;
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_distant_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_default();
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn();
			stateConfVector[1] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn;
			stateConfVectorPosition = 1;
		}

		/* Default enter sequence for final state */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[1] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_;
			stateConfVectorPosition = 1;
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_shunt_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_default();
		}

		/* 'default' enter sequence for state Turn */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn_default()
		{
			/* 'default' enter sequence for state Turn */
			enact_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn();
			stateConfVector[2] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn;
			stateConfVectorPosition = 2;
		}

		/* Default enter sequence for final state */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[2] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_;
			stateConfVectorPosition = 2;
		}

		/* 'default' enter sequence for state Wait for Start */
		void SignalStatechart::enseq_main_region_Wait_for_Start_default()
		{
			/* 'default' enter sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start;
			stateConfVectorPosition = 0;
		}

		/* 'default' enter sequence for state Operating */
		void SignalStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Operating;
			stateConfVectorPosition = 0;
		}

		/* 'default' enter sequence for state Failed */
		void SignalStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::main_region_Failed;
			stateConfVectorPosition = 0;
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

		/* 'default' enter sequence for region Turn main */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_default()
		{
			/* 'default' enter sequence for region Turn main */
			react_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__entry_Default();
		}

		/* 'default' enter sequence for region Turn distant */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_default()
		{
			/* 'default' enter sequence for region Turn distant */
			react_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__entry_Default();
		}

		/* 'default' enter sequence for region Turn shunt */
		void SignalStatechart::enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_default()
		{
			/* 'default' enter sequence for region Turn shunt */
			react_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__entry_Default();
		}

		/* Default exit sequence for state Init */
		void SignalStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			exseq_main_region_Init_Init_process();
		}

		/* Default exit sequence for state Waiting */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting()
		{
			/* Default exit sequence for state Waiting */
			exseq_main_region_Init_Init_process_Waiting_main();
			exseq_main_region_Init_Init_process_Waiting_distant();
			exseq_main_region_Init_Init_process_Waiting_shunt();
			exact_main_region_Init_Init_process_Waiting();
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for final state. */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[1] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 1;
		}

		/* Default exit sequence for final state. */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[1] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 1;
		}

		/* Default exit sequence for state Turn */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn()
		{
			/* Default exit sequence for state Turn */
			stateConfVector[2] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 2;
		}

		/* Default exit sequence for final state. */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[2] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 2;
		}

		/* Default exit sequence for state Wait for Start */
		void SignalStatechart::exseq_main_region_Wait_for_Start()
		{
			/* Default exit sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Operating */
		void SignalStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Failed */
		void SignalStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SignalStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for region main region */
		void SignalStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_();
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
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region) at position 2... */
			switch (stateConfVector[ 2 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn();
					exact_main_region_Init_Init_process_Waiting();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_();
					exact_main_region_Init_Init_process_Waiting();
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
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process) at position 2... */
			switch (stateConfVector[ 2 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn();
					exact_main_region_Init_Init_process_Waiting();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_();
					exact_main_region_Init_Init_process_Waiting();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region main */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_main()
		{
			/* Default exit sequence for region main */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process.Waiting.main) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region distant */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_distant()
		{
			/* Default exit sequence for region distant */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process.Waiting.distant) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region shunt */
		void SignalStatechart::exseq_main_region_Init_Init_process_Waiting_shunt()
		{
			/* Default exit sequence for region shunt */
			/* Handle exit of all possible states (of mrw.statechart.SignalStatechart.main_region.Init.Init_process.Waiting.shunt) at position 2... */
			switch (stateConfVector[ 2 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn();
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_ :
				{
					exseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasMain())
			{
				enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn_default();
			}
			else
			{
				enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final__default();
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasDistant())
			{
				enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn_default();
			}
			else
			{
				enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final__default();
			}
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasShunt())
			{
				enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn_default();
			}
			else
			{
				enseq_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final__default();
			}
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__choice_0();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__choice_0();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__choice_0();
		}

		/* Default react sequence for initial entry  */
		void SignalStatechart::react_main_region_Init_Init_process__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_process__sync0();
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process__sync0()
		{
			/* The reactions of state null. */
			enact_main_region_Init_Init_process_Waiting();
			enseq_main_region_Init_Init_process_Waiting_main_Turn_default();
			enseq_main_region_Init_Init_process_Waiting_distant_Turn_default();
			enseq_main_region_Init_Init_process_Waiting_shunt_Turn_default();
		}

		/* The reactions of state null. */
		void SignalStatechart::react_main_region_Init_Init_process__sync1()
		{
			/* The reactions of state null. */
			exseq_main_region_Init();
			enseq_main_region_Operating_default();
			react(0);
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

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Waiting. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
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
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_main_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if ((isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn)) && (isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn)))
					{
						exseq_main_region_Init_Init_process_Waiting();
						react_main_region_Init_Init_process__sync1();
						transitioned_after = 0;
					}
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
					if (response_raised)
					{
						exseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn();
						enseq_main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final__default();
						transitioned_after = 0;
					}
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_main_Turn_react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final__react(const sc::integer transitioned_before)
		{
			/* The reactions of state null. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (0))
				{
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_main_Turn_react(transitioned_before);
				}
			}
			else
			{
				transitioned_after = main_region_Init_Init_process_Waiting_main_Turn_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_distant_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (1))
				{
					if ((isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn)) && (isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn)))
					{
						exseq_main_region_Init_Init_process_Waiting();
						react_main_region_Init_Init_process__sync1();
						transitioned_after = 1;
					}
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (1))
				{
					if (response_raised)
					{
						exseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn();
						enseq_main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final__default();
						transitioned_after = 1;
					}
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_distant_Turn_react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final__react(const sc::integer transitioned_before)
		{
			/* The reactions of state null. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (1))
				{
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_distant_Turn_react(transitioned_before);
				}
			}
			else
			{
				transitioned_after = main_region_Init_Init_process_Waiting_distant_Turn_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_shunt_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (2))
				{
					if ((isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn)) && (isStateActive(mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn)))
					{
						exseq_main_region_Init_Init_process_Waiting();
						react_main_region_Init_Init_process__sync1();
						transitioned_after = 2;
					}
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Turn. */
			sc::integer transitioned_after = transitioned_before;
			if (doCompletion)
			{
				/* Default exit sequence for state Turn */
				stateConfVector[2] = mrw::statechart::SignalStatechart::State::NO_STATE;
				stateConfVectorPosition = 2;
				/* Default enter sequence for final state */
				stateConfVector[2] = mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_;
				stateConfVectorPosition = 2;
			}
			else
			{
				if ((transitioned_after) < (2))
				{
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_shunt_Turn_react(transitioned_before);
				}
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final__react(const sc::integer transitioned_before)
		{
			/* The reactions of state null. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
				if ((transitioned_after) < (2))
				{
				}
				/* If no transition was taken then execute local reactions */
				if ((transitioned_after) == (transitioned_before))
				{
					transitioned_after = main_region_Init_Init_process_Waiting_shunt_Turn_react(transitioned_before);
				}
			}
			else
			{
				transitioned_after = main_region_Init_Init_process_Waiting_shunt_Turn_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait for Start. */
			sc::integer transitioned_after = transitioned_before;
			if (!doCompletion)
			{
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
			}
			return transitioned_after;
		}

		sc::integer SignalStatechart::main_region_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
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

		sc::integer SignalStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
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

		void SignalStatechart::clearInEvents()
		{
			start_raised = false;
			response_raised = false;
			timeEvents[0] = false;
		}

		void SignalStatechart::microStep()
		{
			sc::integer transitioned = -1;
			stateConfVectorPosition = 0;
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn :
				{
					transitioned = main_region_Init_Init_process_Waiting_main_Turn_Turn_main_Turn_react(transitioned);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final_ :
				{
					transitioned = main_region_Init_Init_process_Waiting_main_Turn_Turn_main__final__react(transitioned);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Wait_for_Start :
				{
					transitioned = main_region_Wait_for_Start_react(transitioned);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Operating :
				{
					transitioned = main_region_Operating_react(transitioned);
					break;
				}
			case mrw::statechart::SignalStatechart::State::main_region_Failed :
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
				case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn :
					{
						transitioned = main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant_Turn_react(transitioned);
						break;
					}
				case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final_ :
					{
						transitioned = main_region_Init_Init_process_Waiting_distant_Turn_Turn_distant__final__react(transitioned);
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
				case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn :
					{
						main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt_Turn_react(transitioned);
						break;
					}
				case mrw::statechart::SignalStatechart::State::main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final_ :
					{
						main_region_Init_Init_process_Waiting_shunt_Turn_Turn_shunt__final__react(transitioned);
						break;
					}
				default:
					/* do nothing */
					break;
				}
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
				clearInEvents();
				dispatchEvent(getNextEvent());
			}
			while (((start_raised) || (response_raised)) || (timeEvents[0]));
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
