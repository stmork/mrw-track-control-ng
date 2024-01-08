/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */

#include "OperatingModeStatechart.h"

/*! \file
Implementation of the state machine 'OperatingModeStatechart'
*/

namespace mrw
{
	namespace statechart
	{



		OperatingModeStatechart::OperatingModeStatechart(QObject * parent) noexcept :
			clear_raised(false),
			started_raised(false),
			failed_raised(false),
			edit_raised(false),
			operate_raised(false),
			manual_raised(false),
			manual_value
			(false),
			init_raised(false),
			finalize_raised(false),
			completed_raised(false),
			timerService(nullptr),
			ifaceCan(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
		{
			this->ifaceCan.parent = this;
			for (sc::ushort state_vec_pos = 0; state_vec_pos < maxOrthogonalStates; ++state_vec_pos)
			{
				stateConfVector[state_vec_pos] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			}

			clearInEvents();
		}

		OperatingModeStatechart::~OperatingModeStatechart()
		{
		}

		OperatingModeStatechart::Can::Can(OperatingModeStatechart * parent_) noexcept :
			parent(parent_),
			ifaceCanOperationCallback(nullptr)
		{
		}



		std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance> OperatingModeStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance> nextEvent = 0;

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

		bool OperatingModeStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::OperatingModeStatechart::Event::clear:
				{
					clear_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::started:
				{
					started_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::failed:
				{
					failed_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::edit:
				{
					edit_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::operate:
				{
					operate_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::manual:
				{
					std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstanceWithValue<bool>> e = cast_event_pointer_type<mrw::statechart::OperatingModeStatechart::EventInstanceWithValue<bool> >(std::move(event));

					if (e != 0)
					{
						manual_value
							= e->value;
						manual_raised = true;
					}
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::init:
				{
					init_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::finalize:
				{
					finalize_raised = true;
					break;
				}
			case mrw::statechart::OperatingModeStatechart::Event::completed:
				{
					completed_raised = true;
					break;
				}

			case mrw::statechart::OperatingModeStatechart::Event::Can_connected:
				{
					ifaceCan.connected_raised = true;
					break;
				}

			case mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Running_operating_Prepare_Bus_:
			case mrw::statechart::OperatingModeStatechart::Event::_te1_main_region_Running_operating_Init_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Running_operating_Prepare_Bus_)] = true;
					break;
				}
			default:
				//pointer got out of scope
				return false;
			}
			//pointer got out of scope
			return true;
		}


		/*! Slot for the in event 'clear' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::clear()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::clear)))
			;
			runCycle();
		}


		/*! Slot for the in event 'started' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::started()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::started)))
			;
			runCycle();
		}


		/*! Slot for the in event 'failed' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::failed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::failed)))
			;
			runCycle();
		}


		/*! Slot for the in event 'edit' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::edit()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::edit)))
			;
			runCycle();
		}


		/*! Slot for the in event 'operate' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::operate()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::operate)))
			;
			runCycle();
		}


		/*! Slot for the in event 'manual' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::manual(bool manual_)
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstanceWithValue<bool>>( new mrw::statechart::OperatingModeStatechart::EventInstanceWithValue<bool>(mrw::statechart::OperatingModeStatechart::Event::manual, manual_)))
			;
			runCycle();
		}


		/*! Slot for the in event 'init' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::init()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::init)))
			;
			runCycle();
		}


		/*! Slot for the in event 'finalize' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::finalize()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::finalize)))
			;
			runCycle();
		}


		/*! Slot for the in event 'completed' that is defined in the default interface scope. */
		void mrw::statechart::OperatingModeStatechart::completed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::completed)))
			;
			runCycle();
		}


		/*! Slot for the in event 'connected' that is defined in the interface scope 'can'. */
		void mrw::statechart::OperatingModeStatechart::can_connected()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::OperatingModeStatechart::EventInstance>(new mrw::statechart::OperatingModeStatechart::EventInstance(mrw::statechart::OperatingModeStatechart::Event::Can_connected)))
			;
			runCycle();
		}



		bool OperatingModeStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		bool OperatingModeStatechart::isFinal() const noexcept
		{
			return (stateConfVector[0] == mrw::statechart::OperatingModeStatechart::State::main_region__final_);
		}

		bool OperatingModeStatechart::check() const noexcept
		{
			if (timerService == nullptr)
			{
				return false;
			}
			if (this->ifaceOperationCallback == nullptr)
			{
				return false;
			}
			if (this->ifaceCan.ifaceCanOperationCallback == nullptr)
			{
				return false;
			}
			return true;
		}


		void OperatingModeStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> OperatingModeStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer OperatingModeStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void OperatingModeStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::OperatingModeStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::OperatingModeStatechart::Event::_te0_main_region_Running_operating_Prepare_Bus_)))));
				runCycle();
			}
		}


		bool OperatingModeStatechart::isStateActive(State state) const noexcept
		{
			switch (state)
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					return  (stateConfVector[scvi_main_region_Exit] == mrw::statechart::OperatingModeStatechart::State::main_region_Exit);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					return  (stateConfVector[scvi_main_region__final_] == mrw::statechart::OperatingModeStatechart::State::main_region__final_);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running :
				{
					return  (stateConfVector[scvi_main_region_Running] >= mrw::statechart::OperatingModeStatechart::State::main_region_Running && stateConfVector[scvi_main_region_Running] <= mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Failed] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Prepare_Bus] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Init] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Operating] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Editing] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable :
				{
					return  (stateConfVector[scvi_main_region_Running_operating_Disable] == mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Manual :
				{
					return  (stateConfVector[scvi_main_region_Manual] == mrw::statechart::OperatingModeStatechart::State::main_region_Manual);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Wait :
				{
					return  (stateConfVector[scvi_main_region_Wait] == mrw::statechart::OperatingModeStatechart::State::main_region_Wait);
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

		sc::integer OperatingModeStatechart::getTimeout() noexcept
		{
			return timeout
				;
		}

		void OperatingModeStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}
		OperatingModeStatechart::Can & OperatingModeStatechart::can() noexcept
		{
			return ifaceCan;
		}
		sc::integer OperatingModeStatechart::Can::getTimeout() noexcept
		{
			return timeout
				;
		}

		void OperatingModeStatechart::Can::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceCanOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Exit'. */
		void OperatingModeStatechart::enact_main_region_Exit()
		{
			/* Entry action for state 'Exit'. */
			emit quitting(true);
			ifaceOperationCallback->disableRoutes();
		}

		/* Entry action for state 'Failed'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Failed()
		{
			/* Entry action for state 'Failed'. */
			emit failing();
		}

		/* Entry action for state 'Prepare Bus'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Prepare_Bus()
		{
			/* Entry action for state 'Prepare Bus'. */
			timerService->setTimer(shared_from_this(), 0, ((sc::time) OperatingModeStatechart::Can::timeout), false);
			ifaceCan.ifaceCanOperationCallback->connectBus();
		}

		/* Entry action for state 'Init'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Init()
		{
			/* Entry action for state 'Init'. */
			timerService->setTimer(shared_from_this(), 1, ((sc::time) OperatingModeStatechart::timeout), false);
			ifaceOperationCallback->resetTransaction();
			emit start();
		}

		/* Entry action for state 'Operating'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Operating()
		{
			/* Entry action for state 'Operating'. */
			emit operating(true);
		}

		/* Entry action for state 'Editing'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Editing()
		{
			/* Entry action for state 'Editing'. */
			emit editing(true);
		}

		/* Entry action for state 'Disable'. */
		void OperatingModeStatechart::enact_main_region_Running_operating_Disable()
		{
			/* Entry action for state 'Disable'. */
			ifaceOperationCallback->disableRoutes();
		}

		/* Entry action for state 'Manual'. */
		void OperatingModeStatechart::enact_main_region_Manual()
		{
			/* Entry action for state 'Manual'. */
			emit playing(true);
			ifaceOperationCallback->activateManual(true);
		}

		/* Entry action for state 'Wait'. */
		void OperatingModeStatechart::enact_main_region_Wait()
		{
			/* Entry action for state 'Wait'. */
			ifaceOperationCallback->activateManual(false);
		}

		/* Exit action for state 'Exit'. */
		void OperatingModeStatechart::exact_main_region_Exit()
		{
			/* Exit action for state 'Exit'. */
			emit quitting(false);
		}

		/* Exit action for state 'Prepare Bus'. */
		void OperatingModeStatechart::exact_main_region_Running_operating_Prepare_Bus()
		{
			/* Exit action for state 'Prepare Bus'. */
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* Exit action for state 'Init'. */
		void OperatingModeStatechart::exact_main_region_Running_operating_Init()
		{
			/* Exit action for state 'Init'. */
			timerService->unsetTimer(shared_from_this(), 1);
		}

		/* Exit action for state 'Operating'. */
		void OperatingModeStatechart::exact_main_region_Running_operating_Operating()
		{
			/* Exit action for state 'Operating'. */
			emit operating(false);
		}

		/* Exit action for state 'Editing'. */
		void OperatingModeStatechart::exact_main_region_Running_operating_Editing()
		{
			/* Exit action for state 'Editing'. */
			emit editing(false);
		}

		/* Exit action for state 'Manual'. */
		void OperatingModeStatechart::exact_main_region_Manual()
		{
			/* Exit action for state 'Manual'. */
			emit playing(false);
		}

		/* 'default' enter sequence for state Exit */
		void OperatingModeStatechart::enseq_main_region_Exit_default()
		{
			/* 'default' enter sequence for state Exit */
			enact_main_region_Exit();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Exit;
		}

		/* Default enter sequence for final state */
		void OperatingModeStatechart::enseq_main_region__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region__final_;
		}

		/* 'default' enter sequence for state Failed */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Failed_default()
		{
			/* 'default' enter sequence for state Failed */
			enact_main_region_Running_operating_Failed();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed;
		}

		/* 'default' enter sequence for state Prepare Bus */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Prepare_Bus_default()
		{
			/* 'default' enter sequence for state Prepare Bus */
			enact_main_region_Running_operating_Prepare_Bus();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus;
		}

		/* 'default' enter sequence for state Init */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Init_default()
		{
			/* 'default' enter sequence for state Init */
			enact_main_region_Running_operating_Init();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init;
		}

		/* 'default' enter sequence for state Operating */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Operating_default()
		{
			/* 'default' enter sequence for state Operating */
			enact_main_region_Running_operating_Operating();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating;
		}

		/* 'default' enter sequence for state Editing */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Editing_default()
		{
			/* 'default' enter sequence for state Editing */
			enact_main_region_Running_operating_Editing();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing;
		}

		/* 'default' enter sequence for state Disable */
		void OperatingModeStatechart::enseq_main_region_Running_operating_Disable_default()
		{
			/* 'default' enter sequence for state Disable */
			enact_main_region_Running_operating_Disable();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable;
		}

		/* 'default' enter sequence for state Manual */
		void OperatingModeStatechart::enseq_main_region_Manual_default()
		{
			/* 'default' enter sequence for state Manual */
			enact_main_region_Manual();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Manual;
		}

		/* 'default' enter sequence for state Wait */
		void OperatingModeStatechart::enseq_main_region_Wait_default()
		{
			/* 'default' enter sequence for state Wait */
			enact_main_region_Wait();
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::main_region_Wait;
		}

		/* 'default' enter sequence for region main region */
		void OperatingModeStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Exit */
		void OperatingModeStatechart::exseq_main_region_Exit()
		{
			/* Default exit sequence for state Exit */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Exit();
		}

		/* Default exit sequence for final state. */
		void OperatingModeStatechart::exseq_main_region__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Running */
		void OperatingModeStatechart::exseq_main_region_Running()
		{
			/* Default exit sequence for state Running */
			exseq_main_region_Running_operating();
		}

		/* Default exit sequence for state Failed */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Failed()
		{
			/* Default exit sequence for state Failed */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Prepare Bus */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Prepare_Bus()
		{
			/* Default exit sequence for state Prepare Bus */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Running_operating_Prepare_Bus();
		}

		/* Default exit sequence for state Init */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Init()
		{
			/* Default exit sequence for state Init */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Running_operating_Init();
		}

		/* Default exit sequence for state Operating */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Operating()
		{
			/* Default exit sequence for state Operating */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Running_operating_Operating();
		}

		/* Default exit sequence for state Editing */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Editing()
		{
			/* Default exit sequence for state Editing */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Running_operating_Editing();
		}

		/* Default exit sequence for state Disable */
		void OperatingModeStatechart::exseq_main_region_Running_operating_Disable()
		{
			/* Default exit sequence for state Disable */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Manual */
		void OperatingModeStatechart::exseq_main_region_Manual()
		{
			/* Default exit sequence for state Manual */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
			exact_main_region_Manual();
		}

		/* Default exit sequence for state Wait */
		void OperatingModeStatechart::exseq_main_region_Wait()
		{
			/* Default exit sequence for state Wait */
			stateConfVector[0] = mrw::statechart::OperatingModeStatechart::State::NO_STATE;
		}

		/* Default exit sequence for region main region */
		void OperatingModeStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.OperatingModeStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					exseq_main_region_Exit();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					exseq_main_region__final_();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed :
				{
					exseq_main_region_Running_operating_Failed();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus :
				{
					exseq_main_region_Running_operating_Prepare_Bus();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init :
				{
					exseq_main_region_Running_operating_Init();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating :
				{
					exseq_main_region_Running_operating_Operating();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing :
				{
					exseq_main_region_Running_operating_Editing();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable :
				{
					exseq_main_region_Running_operating_Disable();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Manual :
				{
					exseq_main_region_Manual();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Wait :
				{
					exseq_main_region_Wait();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region operating */
		void OperatingModeStatechart::exseq_main_region_Running_operating()
		{
			/* Default exit sequence for region operating */
			/* Handle exit of all possible states (of mrw.statechart.OperatingModeStatechart.main_region.Running.operating) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed :
				{
					exseq_main_region_Running_operating_Failed();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus :
				{
					exseq_main_region_Running_operating_Prepare_Bus();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init :
				{
					exseq_main_region_Running_operating_Init();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating :
				{
					exseq_main_region_Running_operating_Operating();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing :
				{
					exseq_main_region_Running_operating_Editing();
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable :
				{
					exseq_main_region_Running_operating_Disable();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void OperatingModeStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasActiveRoutes())
			{
				enseq_main_region_Exit_default();
			}
			else
			{
				emit quit();
				enseq_main_region__final__default();
			}
		}

		/* The reactions of state null. */
		void OperatingModeStatechart::react_main_region_Running_operating__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->hasActiveRoutes())
			{
				enseq_main_region_Running_operating_Disable_default();
			}
			else
			{
				enseq_main_region_Running_operating_Init_default();
			}
		}

		/* Default react sequence for initial entry  */
		void OperatingModeStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Running_operating_Prepare_Bus_default();
		}

		sc::integer OperatingModeStatechart::react(const sc::integer transitioned_before)
		{
			/* State machine reactions. */
			return transitioned_before;
		}

		sc::integer OperatingModeStatechart::main_region_Exit_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Exit. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((completed_raised)) && ((!(ifaceOperationCallback->hasActiveRoutes()))))
				{
					exseq_main_region_Exit();
					react_main_region__choice_0();
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

		sc::integer OperatingModeStatechart::main_region__final__react(const sc::integer transitioned_before)
		{
			/* The reactions of state null. */
			return react(transitioned_before);
		}

		sc::integer OperatingModeStatechart::main_region_Running_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Running. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (finalize_raised)
				{
					exseq_main_region_Running();
					react_main_region__choice_0();
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

		sc::integer OperatingModeStatechart::main_region_Running_operating_Failed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Failed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((clear_raised)) && ((ifaceCan.ifaceCanOperationCallback->isConnected())))
				{
					exseq_main_region_Running_operating_Failed();
					emit cleared();
					enseq_main_region_Running_operating_Init_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (edit_raised)
					{
						exseq_main_region_Running_operating_Failed();
						enseq_main_region_Running_operating_Editing_default();
						main_region_Running_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Running_operating_Prepare_Bus_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Prepare Bus. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (ifaceCan.connected_raised)
				{
					exseq_main_region_Running_operating_Prepare_Bus();
					enseq_main_region_Running_operating_Init_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Running_operating_Prepare_Bus();
						timeEvents[0] = false;
						enseq_main_region_Running_operating_Failed_default();
						main_region_Running_react(0);
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Running_operating_Init_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Init. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[1])
				{
					exseq_main_region_Running_operating_Init();
					timeEvents[1] = false;
					enseq_main_region_Running_operating_Failed_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (failed_raised)
					{
						exseq_main_region_Running_operating_Init();
						enseq_main_region_Running_operating_Failed_default();
						main_region_Running_react(0);
						transitioned_after = 0;
					}
					else
					{
						if (started_raised)
						{
							exseq_main_region_Running_operating_Init();
							enseq_main_region_Running_operating_Operating_default();
							main_region_Running_react(0);
							transitioned_after = 0;
						}
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Running_operating_Operating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Operating. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((edit_raised)) && ((!(ifaceOperationCallback->hasActiveRoutes()))))
				{
					exseq_main_region_Running_operating_Operating();
					enseq_main_region_Running_operating_Editing_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (init_raised)
					{
						exseq_main_region_Running_operating_Operating();
						react_main_region_Running_operating__choice_0();
						transitioned_after = 0;
					}
					else
					{
						if (((manual_raised)) && (((manual_value) && (ifaceOperationCallback->isManualValid()))))
						{
							exseq_main_region_Running();
							enseq_main_region_Manual_default();
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
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Running_operating_Editing_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Editing. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (operate_raised)
				{
					exseq_main_region_Running_operating_Editing();
					enseq_main_region_Running_operating_Operating_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Running_operating_Disable_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Disable. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((completed_raised)) && ((!(ifaceOperationCallback->hasActiveRoutes()))))
				{
					exseq_main_region_Running_operating_Disable();
					enseq_main_region_Running_operating_Init_default();
					main_region_Running_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Running_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer OperatingModeStatechart::main_region_Manual_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Manual. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (((manual_raised)) && ((!(manual_value))))
				{
					exseq_main_region_Manual();
					enseq_main_region_Running_operating_Init_default();
					react(0);
					transitioned_after = 0;
				}
				else
				{
					if (finalize_raised)
					{
						exseq_main_region_Manual();
						enseq_main_region_Wait_default();
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

		sc::integer OperatingModeStatechart::main_region_Wait_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Wait();
					emit quit();
					enseq_main_region__final__default();
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

		void OperatingModeStatechart::clearInEvents() noexcept
		{
			clear_raised = false;
			started_raised = false;
			failed_raised = false;
			edit_raised = false;
			operate_raised = false;
			manual_raised = false;
			init_raised = false;
			finalize_raised = false;
			completed_raised = false;
			ifaceCan.connected_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
		}

		void OperatingModeStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::OperatingModeStatechart::State::main_region_Exit :
				{
					main_region_Exit_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region__final_ :
				{
					main_region__final__react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed :
				{
					main_region_Running_operating_Failed_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus :
				{
					main_region_Running_operating_Prepare_Bus_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init :
				{
					main_region_Running_operating_Init_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating :
				{
					main_region_Running_operating_Operating_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing :
				{
					main_region_Running_operating_Editing_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable :
				{
					main_region_Running_operating_Disable_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Manual :
				{
					main_region_Manual_react(-1);
					break;
				}
			case mrw::statechart::OperatingModeStatechart::State::main_region_Wait :
				{
					main_region_Wait_react(-1);
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		void OperatingModeStatechart::runCycle()
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

		void OperatingModeStatechart::enter()
		{
			/* Activates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default enter sequence for statechart OperatingModeStatechart */
			enseq_main_region_default();
			isExecuting = false;
		}

		void OperatingModeStatechart::exit()
		{
			/* Deactivates the state machine. */
			if (isExecuting)
			{
				return;
			}
			isExecuting = true;
			/* Default exit sequence for statechart OperatingModeStatechart */
			exseq_main_region();
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void OperatingModeStatechart::triggerWithoutEvent()
		{
			runCycle();
		}

	}
}
