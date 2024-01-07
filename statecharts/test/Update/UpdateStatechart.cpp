/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */

#include "UpdateStatechart.h"

/*! \file
Implementation of the state machine 'UpdateStatechart'
*/

namespace mrw
{
	namespace statechart
	{

		const sc::integer UpdateStatechart::timeout = 250;
		const sc::integer UpdateStatechart::delay_boot = 3000;
		const sc::integer UpdateStatechart::delay_reset = 1200;
		const sc::integer UpdateStatechart::delay_flash_request = 200;
		const sc::integer UpdateStatechart::delay_flash_page = 60;
		const sc::integer UpdateStatechart::retry = 10;



		UpdateStatechart::UpdateStatechart() :
			count(0),
			error(0),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			connected_raised(false),
			complete_raised(false),
			mismatch_raised(false),
			failed_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		UpdateStatechart::~UpdateStatechart()
		{
		}



		mrw::statechart::UpdateStatechart::EventInstance * UpdateStatechart::getNextEvent()
		{
			mrw::statechart::UpdateStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void UpdateStatechart::dispatchEvent(mrw::statechart::UpdateStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::UpdateStatechart::Event::connected:
				{
					connected_raised = true;
					break;
				}
			case mrw::statechart::UpdateStatechart::Event::complete:
				{
					complete_raised = true;
					break;
				}
			case mrw::statechart::UpdateStatechart::Event::mismatch:
				{
					mismatch_raised = true;
					break;
				}
			case mrw::statechart::UpdateStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}


			case mrw::statechart::UpdateStatechart::Event::_te0_main_region_Ping_:
			case mrw::statechart::UpdateStatechart::Event::_te1_main_region_Reset_:
			case mrw::statechart::UpdateStatechart::Event::_te2_main_region_Flash_Request_:
			case mrw::statechart::UpdateStatechart::Event::_te3_main_region_Flash_Complete_Page_:
			case mrw::statechart::UpdateStatechart::Event::_te4_main_region_Flash_Rest_:
			case mrw::statechart::UpdateStatechart::Event::_te5_main_region_Flash_Check_:
			case mrw::statechart::UpdateStatechart::Event::_te6_main_region_Leave_Bootloader_:
			case mrw::statechart::UpdateStatechart::Event::_te7_main_region_Wait_for_Connect_:
			case mrw::statechart::UpdateStatechart::Event::_te8_main_region_Test_Hardware_Mismatch_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::UpdateStatechart::Event::_te0_main_region_Ping_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::UpdateStatechart::raiseConnected()
		{
			incomingEventQueue.push_back(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::connected));
			runCycle();
		}


		void mrw::statechart::UpdateStatechart::raiseComplete()
		{
			incomingEventQueue.push_back(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::complete));
			runCycle();
		}


		void mrw::statechart::UpdateStatechart::raiseMismatch()
		{
			incomingEventQueue.push_back(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::mismatch));
			runCycle();
		}


		void mrw::statechart::UpdateStatechart::raiseFailed()
		{
			incomingEventQueue.push_back(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::failed));
			runCycle();
		}



		bool UpdateStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::UpdateStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool UpdateStatechart::isFinal() const
		{
			return false;
		}

		bool UpdateStatechart::check() const
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


		void UpdateStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * UpdateStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer UpdateStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void UpdateStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::UpdateStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::UpdateStatechart::Event::_te0_main_region_Ping_))));
				runCycle();
			}
		}


		bool UpdateStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::UpdateStatechart::State::main_region_Ping :
				{
					return  (stateConfVector[scvi_main_region_Ping] == mrw::statechart::UpdateStatechart::State::main_region_Ping);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Reset :
				{
					return  (stateConfVector[scvi_main_region_Reset] == mrw::statechart::UpdateStatechart::State::main_region_Reset);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Request :
				{
					return  (stateConfVector[scvi_main_region_Flash_Request] == mrw::statechart::UpdateStatechart::State::main_region_Flash_Request);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page :
				{
					return  (stateConfVector[scvi_main_region_Flash_Complete_Page] == mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest :
				{
					return  (stateConfVector[scvi_main_region_Flash_Rest] == mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Check :
				{
					return  (stateConfVector[scvi_main_region_Flash_Check] == mrw::statechart::UpdateStatechart::State::main_region_Flash_Check);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader :
				{
					return  (stateConfVector[scvi_main_region_Leave_Bootloader] == mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Booted :
				{
					return  (stateConfVector[scvi_main_region_Booted] == mrw::statechart::UpdateStatechart::State::main_region_Booted);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Connect] == mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch :
				{
					return  (stateConfVector[scvi_main_region_Test_Hardware_Mismatch] == mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::UpdateStatechart::State::main_region_Failed);
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

		sc::integer UpdateStatechart::getTimeout()
		{
			return timeout;
		}

		sc::integer UpdateStatechart::getDelay_boot()
		{
			return delay_boot;
		}

		sc::integer UpdateStatechart::getDelay_reset()
		{
			return delay_reset;
		}

		sc::integer UpdateStatechart::getDelay_flash_request()
		{
			return delay_flash_request;
		}

		sc::integer UpdateStatechart::getDelay_flash_page()
		{
			return delay_flash_page;
		}

		sc::integer UpdateStatechart::getCount() const
		{
			return count;
		}

		sc::integer UpdateStatechart::getError() const
		{
			return error;
		}

		sc::integer UpdateStatechart::getRetry()
		{
			return retry;
		}

		void UpdateStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Ping'. */
		void UpdateStatechart::enact_main_region_Ping()
		{
			/* Entry action for state 'Ping'. */
			timerService->setTimer(this, 0, UpdateStatechart::timeout, false);
			ifaceOperationCallback->ping();
		}

		/* Entry action for state 'Reset'. */
		void UpdateStatechart::enact_main_region_Reset()
		{
			/* Entry action for state 'Reset'. */
			timerService->setTimer(this, 1, UpdateStatechart::delay_reset, false);
			ifaceOperationCallback->boot();
		}

		/* Entry action for state 'Flash Request'. */
		void UpdateStatechart::enact_main_region_Flash_Request()
		{
			/* Entry action for state 'Flash Request'. */
			timerService->setTimer(this, 2, UpdateStatechart::delay_flash_request, false);
			ifaceOperationCallback->init(1);
			ifaceOperationCallback->flashRequest();
		}

		/* Entry action for state 'Flash Complete Page'. */
		void UpdateStatechart::enact_main_region_Flash_Complete_Page()
		{
			/* Entry action for state 'Flash Complete Page'. */
			timerService->setTimer(this, 3, UpdateStatechart::delay_flash_page, false);
			ifaceOperationCallback->flashCompletePage();
		}

		/* Entry action for state 'Flash Rest'. */
		void UpdateStatechart::enact_main_region_Flash_Rest()
		{
			/* Entry action for state 'Flash Rest'. */
			timerService->setTimer(this, 4, UpdateStatechart::delay_flash_page, false);
			ifaceOperationCallback->flashRestPage();
		}

		/* Entry action for state 'Flash Check'. */
		void UpdateStatechart::enact_main_region_Flash_Check()
		{
			/* Entry action for state 'Flash Check'. */
			timerService->setTimer(this, 5, UpdateStatechart::delay_boot, false);
			ifaceOperationCallback->init(3);
			ifaceOperationCallback->flashCheck();
		}

		/* Entry action for state 'Leave Bootloader'. */
		void UpdateStatechart::enact_main_region_Leave_Bootloader()
		{
			/* Entry action for state 'Leave Bootloader'. */
			timerService->setTimer(this, 6, UpdateStatechart::delay_boot, false);
			ifaceOperationCallback->boot();
		}

		/* Entry action for state 'Booted'. */
		void UpdateStatechart::enact_main_region_Booted()
		{
			/* Entry action for state 'Booted'. */
			ifaceOperationCallback->quit();
		}

		/* Entry action for state 'Wait for Connect'. */
		void UpdateStatechart::enact_main_region_Wait_for_Connect()
		{
			/* Entry action for state 'Wait for Connect'. */
			timerService->setTimer(this, 7, UpdateStatechart::timeout, false);
		}

		/* Entry action for state 'Test Hardware Mismatch'. */
		void UpdateStatechart::enact_main_region_Test_Hardware_Mismatch()
		{
			/* Entry action for state 'Test Hardware Mismatch'. */
			timerService->setTimer(this, 8, UpdateStatechart::delay_flash_request, false);
			ifaceOperationCallback->flashRequest();
		}

		/* Entry action for state 'Failed'. */
		void UpdateStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail(error);
		}

		/* Exit action for state 'Ping'. */
		void UpdateStatechart::exact_main_region_Ping()
		{
			/* Exit action for state 'Ping'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Reset'. */
		void UpdateStatechart::exact_main_region_Reset()
		{
			/* Exit action for state 'Reset'. */
			timerService->unsetTimer(this, 1);
		}

		/* Exit action for state 'Flash Request'. */
		void UpdateStatechart::exact_main_region_Flash_Request()
		{
			/* Exit action for state 'Flash Request'. */
			timerService->unsetTimer(this, 2);
		}

		/* Exit action for state 'Flash Complete Page'. */
		void UpdateStatechart::exact_main_region_Flash_Complete_Page()
		{
			/* Exit action for state 'Flash Complete Page'. */
			timerService->unsetTimer(this, 3);
		}

		/* Exit action for state 'Flash Rest'. */
		void UpdateStatechart::exact_main_region_Flash_Rest()
		{
			/* Exit action for state 'Flash Rest'. */
			timerService->unsetTimer(this, 4);
		}

		/* Exit action for state 'Flash Check'. */
		void UpdateStatechart::exact_main_region_Flash_Check()
		{
			/* Exit action for state 'Flash Check'. */
			timerService->unsetTimer(this, 5);
		}

		/* Exit action for state 'Leave Bootloader'. */
		void UpdateStatechart::exact_main_region_Leave_Bootloader()
		{
			/* Exit action for state 'Leave Bootloader'. */
			timerService->unsetTimer(this, 6);
		}

		/* Exit action for state 'Wait for Connect'. */
		void UpdateStatechart::exact_main_region_Wait_for_Connect()
		{
			/* Exit action for state 'Wait for Connect'. */
			timerService->unsetTimer(this, 7);
		}

		/* Exit action for state 'Test Hardware Mismatch'. */
		void UpdateStatechart::exact_main_region_Test_Hardware_Mismatch()
		{
			/* Exit action for state 'Test Hardware Mismatch'. */
			timerService->unsetTimer(this, 8);
		}

		/* 'default' enter sequence for state Ping */
		void UpdateStatechart::enseq_main_region_Ping_default()
		{
			/* 'default' enter sequence for state Ping */
			enact_main_region_Ping();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Ping;
		}

		/* 'default' enter sequence for state Reset */
		void UpdateStatechart::enseq_main_region_Reset_default()
		{
			/* 'default' enter sequence for state Reset */
			enact_main_region_Reset();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Reset;
		}

		/* 'default' enter sequence for state Flash Request */
		void UpdateStatechart::enseq_main_region_Flash_Request_default()
		{
			/* 'default' enter sequence for state Flash Request */
			enact_main_region_Flash_Request();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Flash_Request;
		}

		/* 'default' enter sequence for state Flash Complete Page */
		void UpdateStatechart::enseq_main_region_Flash_Complete_Page_default()
		{
			/* 'default' enter sequence for state Flash Complete Page */
			enact_main_region_Flash_Complete_Page();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page;
		}

		/* 'default' enter sequence for state Flash Rest */
		void UpdateStatechart::enseq_main_region_Flash_Rest_default()
		{
			/* 'default' enter sequence for state Flash Rest */
			enact_main_region_Flash_Rest();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest;
		}

		/* 'default' enter sequence for state Flash Check */
		void UpdateStatechart::enseq_main_region_Flash_Check_default()
		{
			/* 'default' enter sequence for state Flash Check */
			enact_main_region_Flash_Check();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Flash_Check;
		}

		/* 'default' enter sequence for state Leave Bootloader */
		void UpdateStatechart::enseq_main_region_Leave_Bootloader_default()
		{
			/* 'default' enter sequence for state Leave Bootloader */
			enact_main_region_Leave_Bootloader();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader;
		}

		/* 'default' enter sequence for state Booted */
		void UpdateStatechart::enseq_main_region_Booted_default()
		{
			/* 'default' enter sequence for state Booted */
			enact_main_region_Booted();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Booted;
		}

		/* 'default' enter sequence for state Wait for Connect */
		void UpdateStatechart::enseq_main_region_Wait_for_Connect_default()
		{
			/* 'default' enter sequence for state Wait for Connect */
			enact_main_region_Wait_for_Connect();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect;
		}

		/* 'default' enter sequence for state Test Hardware Mismatch */
		void UpdateStatechart::enseq_main_region_Test_Hardware_Mismatch_default()
		{
			/* 'default' enter sequence for state Test Hardware Mismatch */
			enact_main_region_Test_Hardware_Mismatch();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch;
		}

		/* 'default' enter sequence for state Failed */
		void UpdateStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::main_region_Failed;
		}

		/* 'default' enter sequence for region main region */
		void UpdateStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Ping */
		void UpdateStatechart::exseq_main_region_Ping()
		{
			/* Default exit sequence for state Ping */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Ping();
		}

		/* Default exit sequence for state Reset */
		void UpdateStatechart::exseq_main_region_Reset()
		{
			/* Default exit sequence for state Reset */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Reset();
		}

		/* Default exit sequence for state Flash Request */
		void UpdateStatechart::exseq_main_region_Flash_Request()
		{
			/* Default exit sequence for state Flash Request */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Flash_Request();
		}

		/* Default exit sequence for state Flash Complete Page */
		void UpdateStatechart::exseq_main_region_Flash_Complete_Page()
		{
			/* Default exit sequence for state Flash Complete Page */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Flash_Complete_Page();
		}

		/* Default exit sequence for state Flash Rest */
		void UpdateStatechart::exseq_main_region_Flash_Rest()
		{
			/* Default exit sequence for state Flash Rest */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Flash_Rest();
		}

		/* Default exit sequence for state Flash Check */
		void UpdateStatechart::exseq_main_region_Flash_Check()
		{
			/* Default exit sequence for state Flash Check */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Flash_Check();
		}

		/* Default exit sequence for state Leave Bootloader */
		void UpdateStatechart::exseq_main_region_Leave_Bootloader()
		{
			/* Default exit sequence for state Leave Bootloader */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Leave_Bootloader();
		}

		/* Default exit sequence for state Booted */
		void UpdateStatechart::exseq_main_region_Booted()
		{
			/* Default exit sequence for state Booted */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Wait for Connect */
		void UpdateStatechart::exseq_main_region_Wait_for_Connect()
		{
			/* Default exit sequence for state Wait for Connect */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Wait_for_Connect();
		}

		/* Default exit sequence for state Test Hardware Mismatch */
		void UpdateStatechart::exseq_main_region_Test_Hardware_Mismatch()
		{
			/* Default exit sequence for state Test Hardware Mismatch */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			exact_main_region_Test_Hardware_Mismatch();
		}

		/* Default exit sequence for state Failed */
		void UpdateStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void UpdateStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.UpdateStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::UpdateStatechart::State::main_region_Ping :
				{
					exseq_main_region_Ping();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Reset :
				{
					exseq_main_region_Reset();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Request :
				{
					exseq_main_region_Flash_Request();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page :
				{
					exseq_main_region_Flash_Complete_Page();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest :
				{
					exseq_main_region_Flash_Rest();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Check :
				{
					exseq_main_region_Flash_Check();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader :
				{
					exseq_main_region_Leave_Bootloader();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Booted :
				{
					exseq_main_region_Booted();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect :
				{
					exseq_main_region_Wait_for_Connect();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch :
				{
					exseq_main_region_Test_Hardware_Mismatch();
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void UpdateStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if ((count) < (UpdateStatechart::retry))
			{
				count++;
				enseq_main_region_Flash_Request_default();
			}
			else
			{
				error = 6;
				enseq_main_region_Leave_Bootloader_default();
			}
		}

		/* The reactions of state null. */
		void UpdateStatechart::react_main_region__choice_1()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasPages())
			{
				enseq_main_region_Flash_Complete_Page_default();
			}
			else
			{
				enseq_main_region_Flash_Rest_default();
			}
		}

		/* The reactions of state null. */
		void UpdateStatechart::react_main_region__choice_2()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasController())
			{
				count = 0;
				enseq_main_region_Flash_Request_default();
			}
			else
			{
				error = 1;
				enseq_main_region_Leave_Bootloader_default();
			}
		}

		/* Default react sequence for initial entry  */
		void UpdateStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Connect_default();
		}

		sc::integer UpdateStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer UpdateStatechart::main_region_Ping_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Ping. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[0])
				{
					exseq_main_region_Ping();
					timeEvents[0] = false;
					react_main_region__choice_2();
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

		sc::integer UpdateStatechart::main_region_Reset_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Reset. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Reset();
					timeEvents[1] = false;
					enseq_main_region_Ping_default();
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

		sc::integer UpdateStatechart::main_region_Flash_Request_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flash Request. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[2])
				{
					exseq_main_region_Flash_Request();
					timeEvents[2] = false;
					react_main_region__choice_0();
					transitioned_after = 0;
				}
				else
				{
					if (complete_raised)
					{
						exseq_main_region_Flash_Request();
						enseq_main_region_Test_Hardware_Mismatch_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (mismatch_raised)
						{
							exseq_main_region_Flash_Request();
							error = 8;
							enseq_main_region_Leave_Bootloader_default();
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

		sc::integer UpdateStatechart::main_region_Flash_Complete_Page_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flash Complete Page. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[3])
				{
					exseq_main_region_Flash_Complete_Page();
					timeEvents[3] = false;
					react_main_region__choice_1();
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

		sc::integer UpdateStatechart::main_region_Flash_Rest_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flash Rest. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[4])
				{
					exseq_main_region_Flash_Rest();
					timeEvents[4] = false;
					enseq_main_region_Flash_Check_default();
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

		sc::integer UpdateStatechart::main_region_Flash_Check_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flash Check. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (failed_raised)
				{
					exseq_main_region_Flash_Check();
					error = 3;
					enseq_main_region_Leave_Bootloader_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[5])
					{
						exseq_main_region_Flash_Check();
						error = 4;
						timeEvents[5] = false;
						enseq_main_region_Leave_Bootloader_default();
						react(0);
						transitioned_after = 0;
					}
					else
					{
						if (complete_raised)
						{
							exseq_main_region_Flash_Check();
							enseq_main_region_Booted_default();
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

		sc::integer UpdateStatechart::main_region_Leave_Bootloader_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Leave Bootloader. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[6])
				{
					exseq_main_region_Leave_Bootloader();
					timeEvents[6] = false;
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

		sc::integer UpdateStatechart::main_region_Booted_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Booted. */
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

		sc::integer UpdateStatechart::main_region_Wait_for_Connect_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait for Connect. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (connected_raised)
				{
					exseq_main_region_Wait_for_Connect();
					enseq_main_region_Reset_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[7])
					{
						exseq_main_region_Wait_for_Connect();
						error = 7;
						timeEvents[7] = false;
						enseq_main_region_Leave_Bootloader_default();
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

		sc::integer UpdateStatechart::main_region_Test_Hardware_Mismatch_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Test Hardware Mismatch. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (mismatch_raised)
				{
					exseq_main_region_Test_Hardware_Mismatch();
					error = 8;
					enseq_main_region_Leave_Bootloader_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[8])
					{
						exseq_main_region_Test_Hardware_Mismatch();
						timeEvents[8] = false;
						react_main_region__choice_1();
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

		sc::integer UpdateStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
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

		void UpdateStatechart::clearInEvents()
		{
			connected_raised = false;
			complete_raised = false;
			mismatch_raised = false;
			failed_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
			timeEvents[3] = false;
			timeEvents[4] = false;
			timeEvents[5] = false;
			timeEvents[6] = false;
			timeEvents[7] = false;
			timeEvents[8] = false;
		}

		void UpdateStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::UpdateStatechart::State::main_region_Ping :
				{
					main_region_Ping_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Reset :
				{
					main_region_Reset_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Request :
				{
					main_region_Flash_Request_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page :
				{
					main_region_Flash_Complete_Page_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest :
				{
					main_region_Flash_Rest_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Flash_Check :
				{
					main_region_Flash_Check_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader :
				{
					main_region_Leave_Bootloader_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Booted :
				{
					main_region_Booted_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect :
				{
					main_region_Wait_for_Connect_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch :
				{
					main_region_Test_Hardware_Mismatch_react(-1);
					break;
				}
			case mrw::statechart::UpdateStatechart::State::main_region_Failed :
				{
					main_region_Failed_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void UpdateStatechart::runCycle()
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
			while (((((((((((((connected_raised) || (complete_raised)) || (mismatch_raised)) || (failed_raised)) || (timeEvents[0])) || (timeEvents[1])) || (timeEvents[2])) || (timeEvents[3])) || (timeEvents[4])) || (timeEvents[5])) || (timeEvents[6])) || (timeEvents[7])) || (timeEvents[8]));
			isExecuting = false;
		}

		void UpdateStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart UpdateStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void UpdateStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart UpdateStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void UpdateStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
