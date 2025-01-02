/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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



		UpdateStatechart::UpdateStatechart(QObject * parent) noexcept :
			count(0),
			error(0),
			connected_raised(false),
			complete_raised(false),
			mismatch_raised(false),
			failed_raised(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
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



		std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance> UpdateStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance> nextEvent = 0;

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

		bool UpdateStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
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
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'connected' that is defined in the default interface scope. */
		void mrw::statechart::UpdateStatechart::connected()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance>(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::connected)))
			;
			runCycle();
		}


		/*! Slot for the in event 'complete' that is defined in the default interface scope. */
		void mrw::statechart::UpdateStatechart::complete()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance>(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::complete)))
			;
			runCycle();
		}


		/*! Slot for the in event 'mismatch' that is defined in the default interface scope. */
		void mrw::statechart::UpdateStatechart::mismatch()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance>(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::mismatch)))
			;
			runCycle();
		}


		/*! Slot for the in event 'failed' that is defined in the default interface scope. */
		void mrw::statechart::UpdateStatechart::failed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::UpdateStatechart::EventInstance>(new mrw::statechart::UpdateStatechart::EventInstance(mrw::statechart::UpdateStatechart::Event::failed)))
			;
			runCycle();
		}



		bool UpdateStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::UpdateStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool UpdateStatechart::isFinal() const noexcept
		{
			return false;
		}

		bool UpdateStatechart::check() const noexcept
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


		void UpdateStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> UpdateStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer UpdateStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void UpdateStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::UpdateStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::UpdateStatechart::Event::_te0_main_region_Ping_)))));
				runCycle();
			}
		}


		bool UpdateStatechart::isStateActive(State state) const noexcept
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

		sc::integer UpdateStatechart::getTimeout() noexcept
		{
			return timeout
				;
		}

		sc::integer UpdateStatechart::getDelay_boot() noexcept
		{
			return delay_boot
				;
		}

		sc::integer UpdateStatechart::getDelay_reset() noexcept
		{
			return delay_reset
				;
		}

		sc::integer UpdateStatechart::getDelay_flash_request() noexcept
		{
			return delay_flash_request
				;
		}

		sc::integer UpdateStatechart::getDelay_flash_page() noexcept
		{
			return delay_flash_page
				;
		}

		sc::integer UpdateStatechart::getCount() const noexcept
		{
			return count
				;
		}

		void UpdateStatechart::setCount(sc::integer count_) noexcept
		{
			this->count = count_;
		}
		sc::integer UpdateStatechart::getError() const noexcept
		{
			return error
				;
		}

		void UpdateStatechart::setError(sc::integer error_) noexcept
		{
			this->error = error_;
		}
		sc::integer UpdateStatechart::getRetry() noexcept
		{
			return retry
				;
		}

		void UpdateStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Ping'. */
		void UpdateStatechart::enact_main_region_Ping()
		{
			/* Entry action for state 'Ping'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (UpdateStatechart::timeout)), false);
			ifaceOperationCallback->ping();
		}

		/* Entry action for state 'Reset'. */
		void UpdateStatechart::enact_main_region_Reset()
		{
			/* Entry action for state 'Reset'. */
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (UpdateStatechart::delay_reset)), false);
			ifaceOperationCallback->boot();
		}

		/* Entry action for state 'Flash Request'. */
		void UpdateStatechart::enact_main_region_Flash_Request()
		{
			/* Entry action for state 'Flash Request'. */
			timerService->setTimer(shared_from_this(), 2, (static_cast<sc::time> (UpdateStatechart::delay_flash_request)), false);
			ifaceOperationCallback->init(1);
			ifaceOperationCallback->flashRequest();
		}

		/* Entry action for state 'Flash Complete Page'. */
		void UpdateStatechart::enact_main_region_Flash_Complete_Page()
		{
			/* Entry action for state 'Flash Complete Page'. */
			timerService->setTimer(shared_from_this(), 3, (static_cast<sc::time> (UpdateStatechart::delay_flash_page)), false);
			ifaceOperationCallback->flashCompletePage();
		}

		/* Entry action for state 'Flash Rest'. */
		void UpdateStatechart::enact_main_region_Flash_Rest()
		{
			/* Entry action for state 'Flash Rest'. */
			timerService->setTimer(shared_from_this(), 4, (static_cast<sc::time> (UpdateStatechart::delay_flash_page)), false);
			ifaceOperationCallback->flashRestPage();
		}

		/* Entry action for state 'Flash Check'. */
		void UpdateStatechart::enact_main_region_Flash_Check()
		{
			/* Entry action for state 'Flash Check'. */
			timerService->setTimer(shared_from_this(), 5, (static_cast<sc::time> (UpdateStatechart::delay_boot)), false);
			ifaceOperationCallback->init(3);
			ifaceOperationCallback->flashCheck();
		}

		/* Entry action for state 'Leave Bootloader'. */
		void UpdateStatechart::enact_main_region_Leave_Bootloader()
		{
			/* Entry action for state 'Leave Bootloader'. */
			timerService->setTimer(shared_from_this(), 6, (static_cast<sc::time> (UpdateStatechart::delay_boot)), false);
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
			timerService->setTimer(shared_from_this(), 7, (static_cast<sc::time> (UpdateStatechart::timeout)), false);
		}

		/* Entry action for state 'Test Hardware Mismatch'. */
		void UpdateStatechart::enact_main_region_Test_Hardware_Mismatch()
		{
			/* Entry action for state 'Test Hardware Mismatch'. */
			timerService->setTimer(shared_from_this(), 8, (static_cast<sc::time> (UpdateStatechart::delay_flash_request)), false);
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
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* Exit action for state 'Reset'. */
		void UpdateStatechart::exact_main_region_Reset()
		{
			/* Exit action for state 'Reset'. */
			timerService->unsetTimer(shared_from_this(), 1);
		}

		/* Exit action for state 'Flash Request'. */
		void UpdateStatechart::exact_main_region_Flash_Request()
		{
			/* Exit action for state 'Flash Request'. */
			timerService->unsetTimer(shared_from_this(), 2);
		}

		/* Exit action for state 'Flash Complete Page'. */
		void UpdateStatechart::exact_main_region_Flash_Complete_Page()
		{
			/* Exit action for state 'Flash Complete Page'. */
			timerService->unsetTimer(shared_from_this(), 3);
		}

		/* Exit action for state 'Flash Rest'. */
		void UpdateStatechart::exact_main_region_Flash_Rest()
		{
			/* Exit action for state 'Flash Rest'. */
			timerService->unsetTimer(shared_from_this(), 4);
		}

		/* Exit action for state 'Flash Check'. */
		void UpdateStatechart::exact_main_region_Flash_Check()
		{
			/* Exit action for state 'Flash Check'. */
			timerService->unsetTimer(shared_from_this(), 5);
		}

		/* Exit action for state 'Leave Bootloader'. */
		void UpdateStatechart::exact_main_region_Leave_Bootloader()
		{
			/* Exit action for state 'Leave Bootloader'. */
			timerService->unsetTimer(shared_from_this(), 6);
		}

		/* Exit action for state 'Wait for Connect'. */
		void UpdateStatechart::exact_main_region_Wait_for_Connect()
		{
			/* Exit action for state 'Wait for Connect'. */
			timerService->unsetTimer(shared_from_this(), 7);
		}

		/* Exit action for state 'Test Hardware Mismatch'. */
		void UpdateStatechart::exact_main_region_Test_Hardware_Mismatch()
		{
			/* Exit action for state 'Test Hardware Mismatch'. */
			timerService->unsetTimer(shared_from_this(), 8);
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
				setError(6);
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
				setCount(0);
				enseq_main_region_Flash_Request_default();
			}
			else
			{
				setError(1);
				enseq_main_region_Leave_Bootloader_default();
			}
		}

		/* Default react sequence for initial entry  */
		void UpdateStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Connect_default();
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
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
						transitioned_after = 0;
					}
					else
					{
						if (mismatch_raised)
						{
							exseq_main_region_Flash_Request();
							setError(8);
							enseq_main_region_Leave_Bootloader_default();
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
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

		sc::integer UpdateStatechart::main_region_Flash_Check_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flash Check. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (failed_raised)
				{
					exseq_main_region_Flash_Check();
					setError(3);
					enseq_main_region_Leave_Bootloader_default();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[5])
					{
						exseq_main_region_Flash_Check();
						setError(4);
						timeEvents[5] = false;
						enseq_main_region_Leave_Bootloader_default();
						transitioned_after = 0;
					}
					else
					{
						if (complete_raised)
						{
							exseq_main_region_Flash_Check();
							enseq_main_region_Booted_default();
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
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[7])
					{
						exseq_main_region_Wait_for_Connect();
						setError(7);
						timeEvents[7] = false;
						enseq_main_region_Leave_Bootloader_default();
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

		sc::integer UpdateStatechart::main_region_Test_Hardware_Mismatch_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Test Hardware Mismatch. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (mismatch_raised)
				{
					exseq_main_region_Test_Hardware_Mismatch();
					setError(8);
					enseq_main_region_Leave_Bootloader_default();
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
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = transitioned_before;
			}
			return transitioned_after;
		}

		void UpdateStatechart::clearInEvents() noexcept
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
			}
			while (dispatchEvent(getNextEvent()));
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
			stateConfVector[0] = mrw::statechart::UpdateStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void UpdateStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
