/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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



		RouteStatechart::RouteStatechart(QObject * parent) noexcept :
			turn_raised(false),
			completed_raised(false),
			failed_raised(false),
			disable_raised(false),
			timerService(nullptr),
			ifaceOperationCallback(nullptr),
			isExecuting(false)
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



		std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance> RouteStatechart::getNextEvent() noexcept
		{
			std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance> nextEvent = 0;

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

		bool RouteStatechart::dispatchEvent(std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance> event) noexcept
		{
			if (event == nullptr)
			{
				return false;
			}

			switch (event->eventId)
			{
			case mrw::statechart::RouteStatechart::Event::turn:
				{
					turn_raised = true;
					break;
				}
			case mrw::statechart::RouteStatechart::Event::completed:
				{
					completed_raised = true;
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


			case mrw::statechart::RouteStatechart::Event::_te0_main_region_Disable_:
			case mrw::statechart::RouteStatechart::Event::_te1_main_region_Active_processing_Switch_Turning_:
			case mrw::statechart::RouteStatechart::Event::_te2_main_region_Active_processing_Signal_Turning_:
			case mrw::statechart::RouteStatechart::Event::_te3_main_region_Active_processing_Section_Activation_:
			case mrw::statechart::RouteStatechart::Event::_te4_main_region_Active_processing_Signal_Updating_:
			case mrw::statechart::RouteStatechart::Event::_te5_main_region_Active_processing_Flank_Turning_:
			case mrw::statechart::RouteStatechart::Event::_te6_main_region_Wait_:
			case mrw::statechart::RouteStatechart::Event::_te7_main_region_Emergency_Shutdown_:
				{
					timeEvents[static_cast<sc::integer>(event->eventId) - static_cast<sc::integer>(mrw::statechart::RouteStatechart::Event::_te0_main_region_Disable_)] = true;
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
		void mrw::statechart::RouteStatechart::turn()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance>(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::turn)))
			;
			runCycle();
		}


		/*! Slot for the in event 'completed' that is defined in the default interface scope. */
		void mrw::statechart::RouteStatechart::completed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance>(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::completed)))
			;
			runCycle();
		}


		/*! Slot for the in event 'failed' that is defined in the default interface scope. */
		void mrw::statechart::RouteStatechart::failed()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance>(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::failed)))
			;
			runCycle();
		}


		/*! Slot for the in event 'disable' that is defined in the default interface scope. */
		void mrw::statechart::RouteStatechart::disable()
		{
			incomingEventQueue.push_back(std::unique_ptr<mrw::statechart::RouteStatechart::EventInstance>(new mrw::statechart::RouteStatechart::EventInstance(mrw::statechart::RouteStatechart::Event::disable)))
			;
			runCycle();
		}



		bool RouteStatechart::isActive() const noexcept
		{
			return stateConfVector[0] != mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		bool RouteStatechart::isFinal() const noexcept
		{
			return (stateConfVector[0] == mrw::statechart::RouteStatechart::State::main_region__final_);
		}

		bool RouteStatechart::check() const noexcept
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


		void RouteStatechart::setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept
		{
			this->timerService = timerService_;
		}

		std::shared_ptr<sc::timer::TimerServiceInterface> RouteStatechart::getTimerService() noexcept
		{
			return timerService;
		}

		sc::integer RouteStatechart::getNumberOfParallelTimeEvents() noexcept
		{
			return parallelTimeEventsCount;
		}

		void RouteStatechart::raiseTimeEvent(sc::eventid evid)
		{
			if (evid < timeEventsCount)
			{
				incomingEventQueue.push_back(std::unique_ptr< EventInstance>(new EventInstance(static_cast<mrw::statechart::RouteStatechart::Event>(evid + static_cast<sc::integer>(mrw::statechart::RouteStatechart::Event::_te0_main_region_Disable_)))));
				runCycle();
			}
		}


		bool RouteStatechart::isStateActive(State state) const noexcept
		{
			switch (state)
			{
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
			case mrw::statechart::RouteStatechart::State::main_region__final_ :
				{
					return  (stateConfVector[scvi_main_region__final_] == mrw::statechart::RouteStatechart::State::main_region__final_);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active :
				{
					return  (stateConfVector[scvi_main_region_Active] >= mrw::statechart::RouteStatechart::State::main_region_Active && stateConfVector[scvi_main_region_Active] <= mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Switch_Turning] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Signal_Turning] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Section_Activation] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Signal_Updating] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Flank_Turning] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed :
				{
					return  (stateConfVector[scvi_main_region_Active_processing_Completed] == mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Wait :
				{
					return  (stateConfVector[scvi_main_region_Wait] == mrw::statechart::RouteStatechart::State::main_region_Wait);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown :
				{
					return  (stateConfVector[scvi_main_region_Emergency_Shutdown] == mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Unlock :
				{
					return  (stateConfVector[scvi_main_region_Unlock] == mrw::statechart::RouteStatechart::State::main_region_Unlock);
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

		sc::integer RouteStatechart::getSwitch_timeout() noexcept
		{
			return switch_timeout
				;
		}

		sc::integer RouteStatechart::getSignal_timeout() noexcept
		{
			return signal_timeout
				;
		}

		sc::integer RouteStatechart::getSection_timeout() noexcept
		{
			return section_timeout
				;
		}

		sc::integer RouteStatechart::getEmergency() noexcept
		{
			return emergency
				;
		}

		void RouteStatechart::setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept
		{
			ifaceOperationCallback = operationCallback;
		}

// implementations of all internal functions
		/* Entry action for state 'Disable'. */
		void RouteStatechart::enact_main_region_Disable()
		{
			/* Entry action for state 'Disable'. */
			timerService->setTimer(shared_from_this(), 0, (static_cast<sc::time> (RouteStatechart::emergency)), false);
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->disableSections();
			ifaceOperationCallback->disableSignals();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Switch Turning'. */
		void RouteStatechart::enact_main_region_Active_processing_Switch_Turning()
		{
			/* Entry action for state 'Switch Turning'. */
			timerService->setTimer(shared_from_this(), 1, (static_cast<sc::time> (RouteStatechart::switch_timeout)), false);
			ifaceOperationCallback->prepareRoute();
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->turnSwitches();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Signal Turning'. */
		void RouteStatechart::enact_main_region_Active_processing_Signal_Turning()
		{
			/* Entry action for state 'Signal Turning'. */
			timerService->setTimer(shared_from_this(), 2, (static_cast<sc::time> (RouteStatechart::signal_timeout)), false);
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->enableSignals();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Section Activation'. */
		void RouteStatechart::enact_main_region_Active_processing_Section_Activation()
		{
			/* Entry action for state 'Section Activation'. */
			timerService->setTimer(shared_from_this(), 3, (static_cast<sc::time> (RouteStatechart::section_timeout)), false);
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->enableSections();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Signal Updating'. */
		void RouteStatechart::enact_main_region_Active_processing_Signal_Updating()
		{
			/* Entry action for state 'Signal Updating'. */
			timerService->setTimer(shared_from_this(), 4, (static_cast<sc::time> (RouteStatechart::signal_timeout)), false);
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->extendSignals();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Flank Turning'. */
		void RouteStatechart::enact_main_region_Active_processing_Flank_Turning()
		{
			/* Entry action for state 'Flank Turning'. */
			timerService->setTimer(shared_from_this(), 5, (static_cast<sc::time> (RouteStatechart::switch_timeout)), false);
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->prepareFlank();
			ifaceOperationCallback->turnFlanks();
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Completed'. */
		void RouteStatechart::enact_main_region_Active_processing_Completed()
		{
			/* Entry action for state 'Completed'. */
			emit activated();
		}

		/* Entry action for state 'Wait'. */
		void RouteStatechart::enact_main_region_Wait()
		{
			/* Entry action for state 'Wait'. */
			timerService->setTimer(shared_from_this(), 6, (static_cast<sc::time> (RouteStatechart::emergency)), false);
			ifaceOperationCallback->tryComplete();
		}

		/* Entry action for state 'Emergency Shutdown'. */
		void RouteStatechart::enact_main_region_Emergency_Shutdown()
		{
			/* Entry action for state 'Emergency Shutdown'. */
			timerService->setTimer(shared_from_this(), 7, (static_cast<sc::time> (RouteStatechart::section_timeout)), false);
			ifaceOperationCallback->fail();
			ifaceOperationCallback->disableSections();
			ifaceOperationCallback->disableSignals();
		}

		/* Entry action for state 'Unlock'. */
		void RouteStatechart::enact_main_region_Unlock()
		{
			/* Entry action for state 'Unlock'. */
			ifaceOperationCallback->resetTransaction();
			ifaceOperationCallback->unlockRailParts();
			ifaceOperationCallback->unlockSections();
			ifaceOperationCallback->tryComplete();
		}

		/* Exit action for state 'Disable'. */
		void RouteStatechart::exact_main_region_Disable()
		{
			/* Exit action for state 'Disable'. */
			timerService->unsetTimer(shared_from_this(), 0);
		}

		/* Exit action for state 'Switch Turning'. */
		void RouteStatechart::exact_main_region_Active_processing_Switch_Turning()
		{
			/* Exit action for state 'Switch Turning'. */
			timerService->unsetTimer(shared_from_this(), 1);
		}

		/* Exit action for state 'Signal Turning'. */
		void RouteStatechart::exact_main_region_Active_processing_Signal_Turning()
		{
			/* Exit action for state 'Signal Turning'. */
			timerService->unsetTimer(shared_from_this(), 2);
		}

		/* Exit action for state 'Section Activation'. */
		void RouteStatechart::exact_main_region_Active_processing_Section_Activation()
		{
			/* Exit action for state 'Section Activation'. */
			timerService->unsetTimer(shared_from_this(), 3);
		}

		/* Exit action for state 'Signal Updating'. */
		void RouteStatechart::exact_main_region_Active_processing_Signal_Updating()
		{
			/* Exit action for state 'Signal Updating'. */
			timerService->unsetTimer(shared_from_this(), 4);
		}

		/* Exit action for state 'Flank Turning'. */
		void RouteStatechart::exact_main_region_Active_processing_Flank_Turning()
		{
			/* Exit action for state 'Flank Turning'. */
			timerService->unsetTimer(shared_from_this(), 5);
		}

		/* Exit action for state 'Wait'. */
		void RouteStatechart::exact_main_region_Wait()
		{
			/* Exit action for state 'Wait'. */
			timerService->unsetTimer(shared_from_this(), 6);
		}

		/* Exit action for state 'Emergency Shutdown'. */
		void RouteStatechart::exact_main_region_Emergency_Shutdown()
		{
			/* Exit action for state 'Emergency Shutdown'. */
			timerService->unsetTimer(shared_from_this(), 7);
		}

		/* 'default' enter sequence for state Disable */
		void RouteStatechart::enseq_main_region_Disable_default()
		{
			/* 'default' enter sequence for state Disable */
			enact_main_region_Disable();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Disable;
		}

		/* 'default' enter sequence for state Start */
		void RouteStatechart::enseq_main_region_Start_default()
		{
			/* 'default' enter sequence for state Start */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Start;
		}

		/* Default enter sequence for final state */
		void RouteStatechart::enseq_main_region__final__default()
		{
			/* Default enter sequence for final state */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region__final_;
		}

		/* 'default' enter sequence for state Switch Turning */
		void RouteStatechart::enseq_main_region_Active_processing_Switch_Turning_default()
		{
			/* 'default' enter sequence for state Switch Turning */
			enact_main_region_Active_processing_Switch_Turning();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning;
		}

		/* 'default' enter sequence for state Signal Turning */
		void RouteStatechart::enseq_main_region_Active_processing_Signal_Turning_default()
		{
			/* 'default' enter sequence for state Signal Turning */
			enact_main_region_Active_processing_Signal_Turning();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning;
		}

		/* 'default' enter sequence for state Section Activation */
		void RouteStatechart::enseq_main_region_Active_processing_Section_Activation_default()
		{
			/* 'default' enter sequence for state Section Activation */
			enact_main_region_Active_processing_Section_Activation();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation;
		}

		/* 'default' enter sequence for state Signal Updating */
		void RouteStatechart::enseq_main_region_Active_processing_Signal_Updating_default()
		{
			/* 'default' enter sequence for state Signal Updating */
			enact_main_region_Active_processing_Signal_Updating();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating;
		}

		/* 'default' enter sequence for state Flank Turning */
		void RouteStatechart::enseq_main_region_Active_processing_Flank_Turning_default()
		{
			/* 'default' enter sequence for state Flank Turning */
			enact_main_region_Active_processing_Flank_Turning();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning;
		}

		/* 'default' enter sequence for state Completed */
		void RouteStatechart::enseq_main_region_Active_processing_Completed_default()
		{
			/* 'default' enter sequence for state Completed */
			enact_main_region_Active_processing_Completed();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed;
		}

		/* 'default' enter sequence for state Wait */
		void RouteStatechart::enseq_main_region_Wait_default()
		{
			/* 'default' enter sequence for state Wait */
			enact_main_region_Wait();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Wait;
		}

		/* 'default' enter sequence for state Emergency Shutdown */
		void RouteStatechart::enseq_main_region_Emergency_Shutdown_default()
		{
			/* 'default' enter sequence for state Emergency Shutdown */
			enact_main_region_Emergency_Shutdown();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown;
		}

		/* 'default' enter sequence for state Unlock */
		void RouteStatechart::enseq_main_region_Unlock_default()
		{
			/* 'default' enter sequence for state Unlock */
			enact_main_region_Unlock();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Unlock;
		}

		/* 'default' enter sequence for region main region */
		void RouteStatechart::enseq_main_region_default()
		{
			/* 'default' enter sequence for region main region */
			react_main_region__entry_Default();
		}

		/* Default exit sequence for state Disable */
		void RouteStatechart::exseq_main_region_Disable()
		{
			/* Default exit sequence for state Disable */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Disable();
		}

		/* Default exit sequence for state Start */
		void RouteStatechart::exseq_main_region_Start()
		{
			/* Default exit sequence for state Start */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/* Default exit sequence for final state. */
		void RouteStatechart::exseq_main_region__final_()
		{
			/* Default exit sequence for final state. */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Active */
		void RouteStatechart::exseq_main_region_Active()
		{
			/* Default exit sequence for state Active */
			exseq_main_region_Active_processing();
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
		}

		/* Default exit sequence for state Switch Turning */
		void RouteStatechart::exseq_main_region_Active_processing_Switch_Turning()
		{
			/* Default exit sequence for state Switch Turning */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			exact_main_region_Active_processing_Switch_Turning();
		}

		/* Default exit sequence for state Signal Turning */
		void RouteStatechart::exseq_main_region_Active_processing_Signal_Turning()
		{
			/* Default exit sequence for state Signal Turning */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			exact_main_region_Active_processing_Signal_Turning();
		}

		/* Default exit sequence for state Section Activation */
		void RouteStatechart::exseq_main_region_Active_processing_Section_Activation()
		{
			/* Default exit sequence for state Section Activation */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			exact_main_region_Active_processing_Section_Activation();
		}

		/* Default exit sequence for state Signal Updating */
		void RouteStatechart::exseq_main_region_Active_processing_Signal_Updating()
		{
			/* Default exit sequence for state Signal Updating */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			exact_main_region_Active_processing_Signal_Updating();
		}

		/* Default exit sequence for state Flank Turning */
		void RouteStatechart::exseq_main_region_Active_processing_Flank_Turning()
		{
			/* Default exit sequence for state Flank Turning */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
			exact_main_region_Active_processing_Flank_Turning();
		}

		/* Default exit sequence for state Completed */
		void RouteStatechart::exseq_main_region_Active_processing_Completed()
		{
			/* Default exit sequence for state Completed */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::main_region_Active;
		}

		/* Default exit sequence for state Wait */
		void RouteStatechart::exseq_main_region_Wait()
		{
			/* Default exit sequence for state Wait */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Wait();
		}

		/* Default exit sequence for state Emergency Shutdown */
		void RouteStatechart::exseq_main_region_Emergency_Shutdown()
		{
			/* Default exit sequence for state Emergency Shutdown */
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			exact_main_region_Emergency_Shutdown();
		}

		/* Default exit sequence for region main region */
		void RouteStatechart::exseq_main_region()
		{
			/* Default exit sequence for region main region */
			/* Handle exit of all possible states (of mrw.statechart.RouteStatechart.main_region) at position 0... */
			switch (stateConfVector[ 0 ])
			{
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
			case mrw::statechart::RouteStatechart::State::main_region__final_ :
				{
					exseq_main_region__final_();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active :
				{
					exseq_main_region_Active();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning :
				{
					exseq_main_region_Active_processing_Switch_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning :
				{
					exseq_main_region_Active_processing_Signal_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation :
				{
					exseq_main_region_Active_processing_Section_Activation();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating :
				{
					exseq_main_region_Active_processing_Signal_Updating();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning :
				{
					exseq_main_region_Active_processing_Flank_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed :
				{
					exseq_main_region_Active_processing_Completed();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Wait :
				{
					exseq_main_region_Wait();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown :
				{
					exseq_main_region_Emergency_Shutdown();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* Default exit sequence for region processing */
		void RouteStatechart::exseq_main_region_Active_processing()
		{
			/* Default exit sequence for region processing */
			/* Handle exit of all possible states (of mrw.statechart.RouteStatechart.main_region.Active.processing) at position 0... */
			switch (stateConfVector[ 0 ])
			{
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning :
				{
					exseq_main_region_Active_processing_Switch_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning :
				{
					exseq_main_region_Active_processing_Signal_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation :
				{
					exseq_main_region_Active_processing_Section_Activation();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating :
				{
					exseq_main_region_Active_processing_Signal_Updating();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning :
				{
					exseq_main_region_Active_processing_Flank_Turning();
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed :
				{
					exseq_main_region_Active_processing_Completed();
					break;
				}
			default:
				/* do nothing */
				break;
			}
		}

		/* The reactions of state null. */
		void RouteStatechart::react_main_region_Active_processing__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isTour())
			{
				enseq_main_region_Active_processing_Flank_Turning_default();
			}
			else
			{
				enseq_main_region_Active_processing_Signal_Turning_default();
			}
		}

		/* The reactions of state null. */
		void RouteStatechart::react_main_region__choice_0()
		{
			/* The reactions of state null. */
			if (ifaceOperationCallback->isCompleted())
			{
				enseq_main_region_Disable_default();
			}
			else
			{
				enseq_main_region_Wait_default();
			}
		}

		/* Default react sequence for initial entry  */
		void RouteStatechart::react_main_region__entry_Default()
		{
			/* Default react sequence for initial entry  */
			enseq_main_region_Start_default();
		}

		sc::integer RouteStatechart::main_region_Disable_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Disable. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Disable();
					enseq_main_region_Unlock_default();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[0])
					{
						exseq_main_region_Disable();
						timeEvents[0] = false;
						enseq_main_region_Unlock_default();
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

		sc::integer RouteStatechart::main_region_Start_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Start. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (turn_raised)
				{
					exseq_main_region_Start();
					enseq_main_region_Active_processing_Switch_Turning_default();
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

		sc::integer RouteStatechart::main_region_Active_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Active. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (failed_raised)
				{
					exseq_main_region_Active();
					ifaceOperationCallback->fail();
					react_main_region__choice_0();
					transitioned_after = 0;
				}
				else
				{
					if (disable_raised)
					{
						exseq_main_region_Active();
						react_main_region__choice_0();
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

		sc::integer RouteStatechart::main_region_Active_processing_Switch_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Switch Turning. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Active_processing_Switch_Turning();
					react_main_region_Active_processing__choice_0();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[1])
					{
						exseq_main_region_Active();
						timeEvents[1] = false;
						enseq_main_region_Emergency_Shutdown_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_processing_Signal_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Signal Turning. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Active_processing_Signal_Turning();
					enseq_main_region_Active_processing_Signal_Updating_default();
					main_region_Active_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[2])
					{
						exseq_main_region_Active();
						timeEvents[2] = false;
						enseq_main_region_Emergency_Shutdown_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_processing_Section_Activation_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Section Activation. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Active_processing_Section_Activation();
					enseq_main_region_Active_processing_Completed_default();
					main_region_Active_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[3])
					{
						exseq_main_region_Active();
						timeEvents[3] = false;
						enseq_main_region_Emergency_Shutdown_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_processing_Signal_Updating_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Signal Updating. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Active_processing_Signal_Updating();
					enseq_main_region_Active_processing_Section_Activation_default();
					main_region_Active_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[4])
					{
						exseq_main_region_Active();
						timeEvents[4] = false;
						enseq_main_region_Emergency_Shutdown_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_processing_Flank_Turning_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Flank Turning. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Active_processing_Flank_Turning();
					ifaceOperationCallback->unlockFlanks();
					enseq_main_region_Active_processing_Signal_Turning_default();
					main_region_Active_react(0);
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[5])
					{
						exseq_main_region_Active();
						timeEvents[5] = false;
						enseq_main_region_Emergency_Shutdown_default();
						transitioned_after = 0;
					}
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Active_processing_Completed_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Completed. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (turn_raised)
				{
					exseq_main_region_Active_processing_Completed();
					enseq_main_region_Active_processing_Switch_Turning_default();
					main_region_Active_react(0);
					transitioned_after = 0;
				}
			}
			/* If no transition was taken */
			if ((transitioned_after) == (transitioned_before))
			{
				/* then execute local reactions. */
				transitioned_after = main_region_Active_react(transitioned_before);
			}
			return transitioned_after;
		}

		sc::integer RouteStatechart::main_region_Wait_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Wait. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					exseq_main_region_Wait();
					enseq_main_region_Disable_default();
					transitioned_after = 0;
				}
				else
				{
					if (timeEvents[6])
					{
						exseq_main_region_Wait();
						timeEvents[6] = false;
						enseq_main_region_Disable_default();
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

		sc::integer RouteStatechart::main_region_Emergency_Shutdown_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Emergency Shutdown. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (timeEvents[7])
				{
					exseq_main_region_Emergency_Shutdown();
					timeEvents[7] = false;
					enseq_main_region_Unlock_default();
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

		sc::integer RouteStatechart::main_region_Unlock_react(const sc::integer transitioned_before)
		{
			/* The reactions of state Unlock. */
			sc::integer transitioned_after = transitioned_before;
			if ((transitioned_after) < (0))
			{
				if (completed_raised)
				{
					emit finished();
					enseq_main_region__final__default();
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

		void RouteStatechart::clearInEvents() noexcept
		{
			turn_raised = false;
			completed_raised = false;
			failed_raised = false;
			disable_raised = false;
			timeEvents[0] = false;
			timeEvents[1] = false;
			timeEvents[2] = false;
			timeEvents[3] = false;
			timeEvents[4] = false;
			timeEvents[5] = false;
			timeEvents[6] = false;
			timeEvents[7] = false;
		}

		void RouteStatechart::microStep()
		{
			switch (stateConfVector[ 0 ])
			{
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
			case mrw::statechart::RouteStatechart::State::main_region__final_ :
				{
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning :
				{
					main_region_Active_processing_Switch_Turning_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning :
				{
					main_region_Active_processing_Signal_Turning_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation :
				{
					main_region_Active_processing_Section_Activation_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating :
				{
					main_region_Active_processing_Signal_Updating_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning :
				{
					main_region_Active_processing_Flank_Turning_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed :
				{
					main_region_Active_processing_Completed_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Wait :
				{
					main_region_Wait_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown :
				{
					main_region_Emergency_Shutdown_react(-1);
					break;
				}
			case mrw::statechart::RouteStatechart::State::main_region_Unlock :
				{
					main_region_Unlock_react(-1);
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
				microStep();
				clearInEvents();
			}
			while (dispatchEvent(getNextEvent()));
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
			stateConfVector[0] = mrw::statechart::RouteStatechart::State::NO_STATE;
			isExecuting = false;
		}

		/* Can be used by the client code to trigger a run to completion step without raising an event. */
		void RouteStatechart::triggerWithoutEvent()
		{
			runCycle();
		}


	}
}
