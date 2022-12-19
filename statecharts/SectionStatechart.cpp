/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#include "SectionStatechart.h"

/*! \file
Implementation of the state machine 'SectionStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		SectionStatechart::SectionStatechart(QObject * parent) :
			QObject(parent),
			timeout(2000),
			auto_off(true),
			auto_unlock(true),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false),
			stateConfVectorPosition(0),
			stateConfVectorChanged(false),
			enable_raised(false),
			enable_value(false),
			disable_raised(false),
			clear_raised(false),
			start_raised(false),
			relaisResponse_raised(false),
			stateResponse_raised(false),
			stateResponse_value(false),
			failed_raised(false)
		{
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::SectionStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		SectionStatechart::~SectionStatechart()
		{
		}



		mrw::statechart::SectionStatechart::EventInstance * SectionStatechart::getNextEvent()
		{
			mrw::statechart::SectionStatechart::EventInstance * nextEvent = 0;

			if (!incomingEventQueue.empty())
			{
				nextEvent = incomingEventQueue.front();
				incomingEventQueue.pop_front();
			}

			return nextEvent;

		}


		void SectionStatechart::dispatchEvent(mrw::statechart::SectionStatechart::EventInstance * event)
		{
			if (event == nullptr)
			{
				return;
			}

			switch (event->eventId)
			{
			case mrw::statechart::SectionStatechart::Event::enable:
				{
					mrw::statechart::SectionStatechart::EventInstanceWithValue<bool> * e = static_cast<mrw::statechart::SectionStatechart::EventInstanceWithValue<bool>*>(event);
					if (e != 0)
					{
						enable_value = e->value;
						enable_raised = true;
					}
					break;
				}
			case mrw::statechart::SectionStatechart::Event::disable:
				{
					disable_raised = true;
					break;
				}
			case mrw::statechart::SectionStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::SectionStatechart::Event::start:
				{
					start_raised = true;
					break;
				}
			case mrw::statechart::SectionStatechart::Event::relaisResponse:
				{
					relaisResponse_raised = true;
					break;
				}
			case mrw::statechart::SectionStatechart::Event::stateResponse:
				{
					mrw::statechart::SectionStatechart::EventInstanceWithValue<bool> * e = static_cast<mrw::statechart::SectionStatechart::EventInstanceWithValue<bool>*>(event);
					if (e != 0)
					{
						stateResponse_value = e->value;
						stateResponse_raised = true;
					}
					break;
				}
			case mrw::statechart::SectionStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}


			case mrw::statechart::SectionStatechart::Event::_te0_main_region_Init_:
			case mrw::statechart::SectionStatechart::Event::_te1_main_region_Operating_Processing_Locked_Route_active_Waiting_:
			case mrw::statechart::SectionStatechart::Event::_te2_main_region_Operating_Processing_Enabling_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::SectionStatechart::Event::_te0_main_region_Init_)] = true;
					break;
				}
			default:
				/* do nothing */
				break;
			}
			delete event;
		}


		void mrw::statechart::SectionStatechart::enable(bool enable_)
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstanceWithValue<bool>(mrw::statechart::SectionStatechart::Event::enable, enable_));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::disable()
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstance(mrw::statechart::SectionStatechart::Event::disable));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::clear()
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstance(mrw::statechart::SectionStatechart::Event::clear));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::start()
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstance(mrw::statechart::SectionStatechart::Event::start));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::relaisResponse()
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstance(mrw::statechart::SectionStatechart::Event::relaisResponse));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::stateResponse(bool stateResponse_)
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstanceWithValue<bool>(mrw::statechart::SectionStatechart::Event::stateResponse, stateResponse_));
			runCycle();
		}


		void mrw::statechart::SectionStatechart::failed()
		{
			incomingEventQueue.push_back(new mrw::statechart::SectionStatechart::EventInstance(mrw::statechart::SectionStatechart::Event::failed));
			runCycle();
		}



		bool SectionStatechart::isActive() const
		{
			return stateConfVector[0] != mrw::statechart::SectionStatechart::State::NO_STATE || stateConfVector[1] != mrw::statechart::SectionStatechart::State::NO_STATE;
		}

		/*
		 * Always returns 'false' since this state machine can never become final.
		 */
		bool SectionStatechart::isFinal() const
		{
			return false;
		}

		bool SectionStatechart::check() const
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


		void SectionStatechart::setTimerService(sc::timer::TimerServiceInterface * timerService_)
		{
			this->timerService = timerService_;
		}

		sc::timer::TimerServiceInterface * SectionStatechart::getTimerService()
		{
			return timerService;
		}

		sc::integer SectionStatechart::getNumberOfParallelTimeEvents()
		{
			return parallelTimeEventsCount;
		}

		void SectionStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(new EventInstance(static_cast<mrw::statechart::SectionStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::SectionStatechart::Event::_te0_main_region_Init_))));
				runCycle();
			}
		}


		bool SectionStatechart::isStateActive(State state) const
		{
			switch (state)
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init :
				{
					return  (stateConfVector[scvi_main_region_Init] >= mrw::statechart::SectionStatechart::State::main_region_Init && stateConfVector[scvi_main_region_Init] <= mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_Process_Requesting] >= mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting && stateConfVector[scvi_main_region_Init_Init_Process_Requesting] <= mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_Process_Requesting_relais_Relay] == mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_Process_Requesting_relais_Wait] == mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_Process_Requesting_state_Occupation] == mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait :
				{
					return  (stateConfVector[scvi_main_region_Init_Init_Process_Requesting_state_Wait] == mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating :
				{
					return  (stateConfVector[scvi_main_region_Operating] >= mrw::statechart::SectionStatechart::State::main_region_Operating && stateConfVector[scvi_main_region_Operating] <= mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Unlocked] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked] >= mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked && stateConfVector[scvi_main_region_Operating_Processing_Locked] <= mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Enabled] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Passed] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Waiting] >= mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting && stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Waiting] <= mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Locked_Route_active_Disabled] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling :
				{
					return  (stateConfVector[scvi_main_region_Operating_Processing_Enabling] == mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Failed :
				{
					return  (stateConfVector[scvi_main_region_Failed] == mrw::statechart::SectionStatechart::State::main_region_Failed);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start :
				{
					return  (stateConfVector[scvi_main_region_Wait_for_Start] == mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start);
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

		sc::integer SectionStatechart::getTimeout() const
		{
			return timeout;
		}

		void SectionStatechart::setTimeout(sc::integer timeout_)
		{
			this->timeout = timeout_;
		}

		bool SectionStatechart::getAuto_off() const
		{
			return auto_off;
		}

		void SectionStatechart::setAuto_off(bool auto_off_)
		{
			this->auto_off = auto_off_;
		}

		bool SectionStatechart::getAuto_unlock() const
		{
			return auto_unlock;
		}

		void SectionStatechart::setAuto_unlock(bool auto_unlock_)
		{
			this->auto_unlock = auto_unlock_;
		}

		void SectionStatechart::setOperationCallback(OperationCallback * operationCallback)
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Init'. */
		void SectionStatechart::enact_main_region_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(this, 0, timeout, false);
			emit entered();
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Relay'. */
		void SectionStatechart::enact_main_region_Init_Init_Process_Requesting_relais_Relay()
		{
			/* Entry action for state 'Relay'. */
			ifaceOperationCallback->off();
		}

		/* Entry action for state 'Occupation'. */
		void SectionStatechart::enact_main_region_Init_Init_Process_Requesting_state_Occupation()
		{
			/* Entry action for state 'Occupation'. */
			ifaceOperationCallback->request();
		}

		/* Entry action for state 'Unlocked'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Unlocked()
		{
			/* Entry action for state 'Unlocked'. */
			ifaceOperationCallback->free();
			ifaceOperationCallback->lock(false);
		}

		/* Entry action for state 'Locked'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked()
		{
			/* Entry action for state 'Locked'. */
			ifaceOperationCallback->lock(true);
		}

		/* Entry action for state 'Passed'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked_Route_active_Passed()
		{
			/* Entry action for state 'Passed'. */
			ifaceOperationCallback->passed();
			emit tryUnblock();
		}

		/* Entry action for state 'Waiting'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked_Route_active_Waiting()
		{
			/* Entry action for state 'Waiting'. */
			timerService->setTimer(this, 1, timeout, false);
			ifaceOperationCallback->inc();
		}

		/* Entry action for state 'Left'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left()
		{
			/* Entry action for state 'Left'. */
			ifaceOperationCallback->off();
		}

		/* Entry action for state 'Disabling'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling()
		{
			/* Entry action for state 'Disabling'. */
			ifaceOperationCallback->off();
		}

		/* Entry action for state 'Enabling'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling()
		{
			/* Entry action for state 'Enabling'. */
			ifaceOperationCallback->inc();
			ifaceOperationCallback->on();
		}

		/* Entry action for state 'Enabling'. */
		void SectionStatechart::enact_main_region_Operating_Processing_Enabling()
		{
			/* Entry action for state 'Enabling'. */
			timerService->setTimer(this, 2, timeout, false);
			ifaceOperationCallback->inc();
			ifaceOperationCallback->pending();
			ifaceOperationCallback->on();
		}

		/* Entry action for state 'Failed'. */
		void SectionStatechart::enact_main_region_Failed()
		{
			/* Entry action for state 'Failed'. */
			ifaceOperationCallback->fail();
		}

		/* Exit action for state 'Init'. */
		void SectionStatechart::exact_main_region_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(this, 0);
		}

		/* Exit action for state 'Locked'. */
		void SectionStatechart::exact_main_region_Operating_Processing_Locked()
		{
			/* Exit action for state 'Locked'. */
			emit unregister();
		}

		/* Exit action for state 'Waiting'. */
		void SectionStatechart::exact_main_region_Operating_Processing_Locked_Route_active_Waiting()
		{
			/* Exit action for state 'Waiting'. */
			timerService->unsetTimer(this, 1);
		}

		/* Exit action for state 'Enabling'. */
		void SectionStatechart::exact_main_region_Operating_Processing_Enabling()
		{
			/* Exit action for state 'Enabling'. */
			timerService->unsetTimer(this, 2);
		}

		/* 'default' enter sequence for state Init */
		void SectionStatechart::enseq_main_region_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Init();
			enseq_main_region_Init_Init_Process_default();
		}

		/* 'default' enter sequence for state Relay */
		void SectionStatechart::enseq_main_region_Init_Init_Process_Requesting_relais_Relay_default()
		{
			/* 'default' enter sequence for state Relay */
			enact_main_region_Init_Init_Process_Requesting_relais_Relay();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Wait */
		void SectionStatechart::enseq_main_region_Init_Init_Process_Requesting_relais_Wait_default()
		{
			/* 'default' enter sequence for state Wait */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Occupation */
		void SectionStatechart::enseq_main_region_Init_Init_Process_Requesting_state_Occupation_default()
		{
			/* 'default' enter sequence for state Occupation */
			enact_main_region_Init_Init_Process_Requesting_state_Occupation();
			stateConfVector[1] = mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation;
			stateConfVectorPosition = 1;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Wait */
		void SectionStatechart::enseq_main_region_Init_Init_Process_Requesting_state_Wait_default()
		{
			/* 'default' enter sequence for state Wait */
			stateConfVector[1] = mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait;
			stateConfVectorPosition = 1;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Operating */
		void SectionStatechart::enseq_main_region_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enseq_main_region_Operating_Processing_default();
		}

		/* 'default' enter sequence for state Unlocked */
		void SectionStatechart::enseq_main_region_Operating_Processing_Unlocked_default()
		{
			/* 'default' enter sequence for state Unlocked */
			enact_main_region_Operating_Processing_Unlocked();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Enabled */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Enabled_default()
		{
			/* 'default' enter sequence for state Enabled */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Passed */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Passed_default()
		{
			/* 'default' enter sequence for state Passed */
			enact_main_region_Operating_Processing_Locked_Route_active_Passed();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Left */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_default()
		{
			/* 'default' enter sequence for state Left */
			enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Disabling */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_default()
		{
			/* 'default' enter sequence for state Disabling */
			enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Enabling */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_default()
		{
			/* 'default' enter sequence for state Enabling */
			enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Disabled */
		void SectionStatechart::enseq_main_region_Operating_Processing_Locked_Route_active_Disabled_default()
		{
			/* 'default' enter sequence for state Disabled */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Enabling */
		void SectionStatechart::enseq_main_region_Operating_Processing_Enabling_default()
		{
			/* 'default' enter sequence for state Enabling */
			enact_main_region_Operating_Processing_Enabling();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Failed */
		void SectionStatechart::enseq_main_region_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Failed();
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Failed;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for state Wait for Start */
		void SectionStatechart::enseq_main_region_Wait_for_Start_default()
		{
			/* 'default' enter sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start;
			stateConfVectorPosition = 0;
			stateConfVectorChanged = true;
		}

		/* 'default' enter sequence for region main region */
		void SectionStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* 'default' enter sequence for region Init Process */
		void SectionStatechart::enseq_main_region_Init_Init_Process_default()
		{
			/* 'default' enter sequence for region Init Process */
			react_main_region_Init_Init_Process__entry_Default();
		}

		/* 'default' enter sequence for region Processing */
		void SectionStatechart::enseq_main_region_Operating_Processing_default()
		{
			/* 'default' enter sequence for region Processing */
			react_main_region_Operating_Processing__entry_Default();
		}

		/* Default exit sequence for state Init */
		void SectionStatechart::exseq_main_region_Init()
		{
			/* Default exit sequence for state Init */
			exseq_main_region_Init_Init_Process();
			exact_main_region_Init();
		}

		/* Default exit sequence for state Requesting */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting()
		{
			/* Default exit sequence for state Requesting */
			exseq_main_region_Init_Init_Process_Requesting_relais();
			exseq_main_region_Init_Init_Process_Requesting_state();
		}

		/* Default exit sequence for state Relay */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_relais_Relay()
		{
			/* Default exit sequence for state Relay */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Wait */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_relais_Wait()
		{
			/* Default exit sequence for state Wait */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Occupation */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_state_Occupation()
		{
			/* Default exit sequence for state Occupation */
			stateConfVector[1] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 1;
		}

		/* Default exit sequence for state Wait */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_state_Wait()
		{
			/* Default exit sequence for state Wait */
			stateConfVector[1] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 1;
		}

		/* Default exit sequence for state Operating */
		void SectionStatechart::exseq_main_region_Operating()
		{
			/* Default exit sequence for state Operating */
			exseq_main_region_Operating_Processing();
		}

		/* Default exit sequence for state Unlocked */
		void SectionStatechart::exseq_main_region_Operating_Processing_Unlocked()
		{
			/* Default exit sequence for state Unlocked */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Locked */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked()
		{
			/* Default exit sequence for state Locked */
			exseq_main_region_Operating_Processing_Locked_Route_active();
			exact_main_region_Operating_Processing_Locked();
		}

		/* Default exit sequence for state Enabled */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Enabled()
		{
			/* Default exit sequence for state Enabled */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Passed */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Passed()
		{
			/* Default exit sequence for state Passed */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Waiting */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Waiting()
		{
			/* Default exit sequence for state Waiting */
			exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing();
			exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
		}

		/* Default exit sequence for state Left */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left()
		{
			/* Default exit sequence for state Left */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Disabling */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling()
		{
			/* Default exit sequence for state Disabling */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Enabling */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling()
		{
			/* Default exit sequence for state Enabling */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Disabled */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Disabled()
		{
			/* Default exit sequence for state Disabled */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Enabling */
		void SectionStatechart::exseq_main_region_Operating_Processing_Enabling()
		{
			/* Default exit sequence for state Enabling */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
			exact_main_region_Operating_Processing_Enabling();
		}

		/* Default exit sequence for state Failed */
		void SectionStatechart::exseq_main_region_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for state Wait for Start */
		void SectionStatechart::exseq_main_region_Wait_for_Start()
		{
			/* Default exit sequence for state Wait for Start */
			stateConfVector[0] = mrw::statechart::SectionStatechart::State::NO_STATE;
			stateConfVectorPosition = 0;
		}

		/* Default exit sequence for region main region */
		void SectionStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Wait();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					exseq_main_region_Operating_Processing_Unlocked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Passed();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Disabled();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Enabling();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Failed :
				{
					exseq_main_region_Failed();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start :
				{
					exseq_main_region_Wait_for_Start();
					break;
				}
			default:
				/* do nothing */
				break;
			}
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
					exact_main_region_Init();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Wait();
					exact_main_region_Init();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Init Process */
		void SectionStatechart::exseq_main_region_Init_Init_Process()
		{
			/* Default exit sequence for region Init Process */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Init.Init_Process) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Wait();
					break;
				}
			default:
				/* do nothing */
				break;
			}
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Init.Init_Process) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Wait();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region relais */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_relais()
		{
			/* Default exit sequence for region relais */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Init.Init_Process.Requesting.relais) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Wait();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region state */
		void SectionStatechart::exseq_main_region_Init_Init_Process_Requesting_state()
		{
			/* Default exit sequence for region state */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Init.Init_Process.Requesting.state) at position 1... */
			switch (stateConfVector[ 1 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait :
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Wait();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Processing */
		void SectionStatechart::exseq_main_region_Operating_Processing()
		{
			/* Default exit sequence for region Processing */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Operating.Processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					exseq_main_region_Operating_Processing_Unlocked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Passed();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Disabled();
					exact_main_region_Operating_Processing_Locked();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Enabling();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Route active */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active()
		{
			/* Default exit sequence for region Route active */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Operating.Processing.Locked.Route_active) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Passed();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
					exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Disabled();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region Relais processing */
		void SectionStatechart::exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing()
		{
			/* Default exit sequence for region Relais processing */
			/* Handle exit of all possible states (of mrw.statechart.SectionStatechart.main_region.Operating.Processing.Locked.Route_active.Waiting.Relais_processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void SectionStatechart::react_main_region_Operating_Processing_Locked_Route_active__choice_0()
		{
			/* The reactions of state null. */
			if (auto_unlock)
			{
				exseq_main_region_Operating_Processing_Locked();
				enseq_main_region_Operating_Processing_Unlocked_default();
				main_region_Operating_react(0);
			}
			else
			{
				enseq_main_region_Operating_Processing_Locked_Route_active_Passed_default();
			}
		}

		/* The reactions of state null. */
		void SectionStatechart::react_main_region_Operating_Processing__choice_0()
		{
			/* The reactions of state null. */
			if (enable_value)
			{
				enseq_main_region_Operating_Processing_Enabling_default();
			}
			else
			{
				enact_main_region_Operating_Processing_Locked();
				enseq_main_region_Operating_Processing_Locked_Route_active_Disabled_default();
			}
		}

		/* Default react sequence for initial entry  */
		void SectionStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Wait_for_Start_default();
		}

		/* Default react sequence for initial entry  */
		void SectionStatechart::react_main_region_Init_Init_Process__entry_Default()
		{
			/* Default react sequence for initial entry  */
			react_main_region_Init_Init_Process__sync1();
		}

		/* Default react sequence for initial entry  */
		void SectionStatechart::react_main_region_Operating_Processing__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Operating_Processing_Unlocked_default();
		}

		/* The reactions of state null. */
		void SectionStatechart::react_main_region_Init_Init_Process__sync0()
		{
			/* The reactions of state null. */
			exseq_main_region_Init();
			ifaceOperationCallback->dec();
			emit started();
			enseq_main_region_Operating_default();
			react(0);
		}

		/* The reactions of state null. */
		void SectionStatechart::react_main_region_Init_Init_Process__sync1()
		{
			/* The reactions of state null. */
			enseq_main_region_Init_Init_Process_Requesting_relais_Relay_default();
			enseq_main_region_Init_Init_Process_Requesting_state_Occupation_default();
		}

		sc::integer SectionStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer SectionStatechart::main_region_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[0])
				{
					exseq_main_region_Init();
					timeEvents[0] = false;
					enseq_main_region_Failed_default();
					react(0);
					transitioned_after = 1;
				}
				else
				{
					if (failed_raised)
					{
						exseq_main_region_Init();
						enseq_main_region_Failed_default();
						react(0);
						transitioned_after = 1;
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

		sc::integer SectionStatechart::main_region_Init_Init_Process_Requesting_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Requesting. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Init_Init_Process_Requesting_relais_Relay_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Relay. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (relaisResponse_raised)
				{
					exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
					enseq_main_region_Init_Init_Process_Requesting_relais_Wait_default();
					transitioned_after = 0;
				}
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Init_Init_Process_Requesting_relais_Wait_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait))
				{
					exseq_main_region_Init_Init_Process_Requesting();
					react_main_region_Init_Init_Process__sync0();
					transitioned_after = 0;
				}
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Init_Init_Process_Requesting_state_Occupation_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Occupation. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (1))
			{
				if (stateResponse_raised)
				{
					exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
					enseq_main_region_Init_Init_Process_Requesting_state_Wait_default();
					main_region_Init_Init_Process_Requesting_react(0);
					transitioned_after = 1;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_Process_Requesting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Init_Init_Process_Requesting_state_Wait_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (1))
			{
				if (isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait))
				{
					exseq_main_region_Init_Init_Process_Requesting();
					react_main_region_Init_Init_Process__sync0();
					transitioned_after = 1;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Init_Init_Process_Requesting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_react(const sc::integer transitioned_before)
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
				else
				{
					if (start_raised)
					{
						exseq_main_region_Operating();
						enseq_main_region_Init_default();
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

		sc::integer SectionStatechart::main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Unlocked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (enable_raised)
				{
					exseq_main_region_Operating_Processing_Unlocked();
					react_main_region_Operating_Processing__choice_0();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Locked. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Enabled_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Enabled. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((stateResponse_raised)) && (((!stateResponse_value) && (auto_off))))
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
					enact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_default();
					main_region_Operating_Processing_Locked_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (disable_raised)
					{
						exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
						enact_main_region_Operating_Processing_Locked_Route_active_Waiting();
						enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_default();
						main_region_Operating_Processing_Locked_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Passed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Passed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (disable_raised)
				{
					exseq_main_region_Operating_Processing_Locked();
					enseq_main_region_Operating_Processing_Unlocked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Waiting_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Waiting. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Operating();
					timeEvents[1] = false;
					enseq_main_region_Failed_default();
					react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Left. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (relaisResponse_raised)
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting();
					ifaceOperationCallback->dec();
					emit left();
					react_main_region_Operating_Processing_Locked_Route_active__choice_0();
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_Route_active_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Disabling. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (relaisResponse_raised)
				{
					exseq_main_region_Operating_Processing_Locked();
					ifaceOperationCallback->dec();
					enseq_main_region_Operating_Processing_Unlocked_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_Route_active_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Enabling. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (relaisResponse_raised)
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Waiting();
					ifaceOperationCallback->dec();
					enseq_main_region_Operating_Processing_Locked_Route_active_Enabled_default();
					main_region_Operating_Processing_Locked_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_Route_active_Waiting_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Locked_Route_active_Disabled_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Disabled. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((enable_raised)) && ((enable_value)))
				{
					exseq_main_region_Operating_Processing_Locked_Route_active_Disabled();
					enact_main_region_Operating_Processing_Locked_Route_active_Waiting();
					enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_default();
					main_region_Operating_Processing_Locked_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (disable_raised)
					{
						exseq_main_region_Operating_Processing_Locked();
						enseq_main_region_Operating_Processing_Unlocked_default();
						main_region_Operating_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_Processing_Locked_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Operating_Processing_Enabling_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Enabling. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (relaisResponse_raised)
				{
					exseq_main_region_Operating_Processing_Enabling();
					ifaceOperationCallback->dec();
					enact_main_region_Operating_Processing_Locked();
					enseq_main_region_Operating_Processing_Locked_Route_active_Enabled_default();
					main_region_Operating_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[2])
					{
						exseq_main_region_Operating();
						timeEvents[2] = false;
						enseq_main_region_Failed_default();
						react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken then execute local reactions */
			if ((transitioned_after) == (transitioned_before))
			{
				transitioned_after = main_region_Operating_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer SectionStatechart::main_region_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (clear_raised)
				{
					exseq_main_region_Failed();
					enseq_main_region_Wait_for_Start_default();
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

		sc::integer SectionStatechart::main_region_Wait_for_Start_react(const sc::integer transitioned_before)
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

		void SectionStatechart::clearInEvents()
		{
			enable_raised = false;
			disable_raised = false;
			clear_raised = false;
			start_raised = false;
			relaisResponse_raised = false;
			stateResponse_raised = false;
			failed_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
		}

		void SectionStatechart::microStep()
		{
			sc::integer transitioned = -1;
			stateConfVectorPosition = 0;
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay :
				{
					transitioned = main_region_Init_Init_Process_Requesting_relais_Relay_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait :
				{
					transitioned = main_region_Init_Init_Process_Requesting_relais_Wait_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked :
				{
					transitioned = main_region_Operating_Processing_Unlocked_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Enabled_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Passed_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled :
				{
					transitioned = main_region_Operating_Processing_Locked_Route_active_Disabled_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Enabling :
				{
					transitioned = main_region_Operating_Processing_Enabling_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Failed :
				{
					transitioned = main_region_Failed_react(transitioned);
					break;
				}
			case mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start :
				{
					transitioned = main_region_Wait_for_Start_react(transitioned);
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
				case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation :
					{
						main_region_Init_Init_Process_Requesting_state_Occupation_react(transitioned);
						break;
					}
				case mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait :
					{
						main_region_Init_Init_Process_Requesting_state_Wait_react(transitioned);
						break;
					}
				default:
					/* do nothing */
					break;
				}
			}
		}

		void SectionStatechart::runCycle()
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
			while ((((((((((enable_raised) || (disable_raised)) || (clear_raised)) || (start_raised)) || (relaisResponse_raised)) || (stateResponse_raised)) || (failed_raised)) || (timeEvents[0])) || (timeEvents[1])) || (timeEvents[2]));
			isExecuting = false;
		}

		void SectionStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart SectionStatechart */
			enseq_main_region_default();
			do
			{
				stateConfVectorChanged = false;
				microStep();
			}
			while (stateConfVectorChanged);
			isExecuting = false;
		}

		void SectionStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart SectionStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void SectionStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
