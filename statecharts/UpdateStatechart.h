/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef UPDATESTATECHART_H_
#define UPDATESTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the UpdateStatechart state machine.
		*/
		class UpdateStatechart;
	}
}


#include <deque>
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'UpdateStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class UpdateStatechart : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			UpdateStatechart(QObject * parent);

			virtual ~UpdateStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Ping,
				main_region_Reset,
				main_region_Flash_Request,
				main_region_Flash_Complete_Page,
				main_region_Flash_Rest,
				main_region_Flash_Check,
				main_region__final_
			};

			/*! The number of states. */
			static const sc::integer numStates = 7;
			static const sc::integer scvi_main_region_Ping = 0;
			static const sc::integer scvi_main_region_Reset = 0;
			static const sc::integer scvi_main_region_Flash_Request = 0;
			static const sc::integer scvi_main_region_Flash_Complete_Page = 0;
			static const sc::integer scvi_main_region_Flash_Rest = 0;
			static const sc::integer scvi_main_region_Flash_Check = 0;
			static const sc::integer scvi_main_region__final_ = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				complete,
				_te0_main_region_Ping_,
				_te1_main_region_Reset_,
				_te2_main_region_Flash_Request_,
				_te3_main_region_Flash_Complete_Page_,
				_te4_main_region_Flash_Rest_,
				_te5_main_region_Flash_Check_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			sc::integer getTimeout() const;

			/*! Sets the value of the variable 'timeout' that is defined in the default interface scope. */
			void setTimeout(sc::integer timeout);

			/*! Gets the value of the variable 'delay_boot' that is defined in the default interface scope. */
			sc::integer getDelay_boot() const;

			/*! Sets the value of the variable 'delay_boot' that is defined in the default interface scope. */
			void setDelay_boot(sc::integer delay_boot);

			/*! Gets the value of the variable 'delay_reset' that is defined in the default interface scope. */
			sc::integer getDelay_reset() const;

			/*! Sets the value of the variable 'delay_reset' that is defined in the default interface scope. */
			void setDelay_reset(sc::integer delay_reset);

			/*! Gets the value of the variable 'delay_flash_request' that is defined in the default interface scope. */
			sc::integer getDelay_flash_request() const;

			/*! Sets the value of the variable 'delay_flash_request' that is defined in the default interface scope. */
			void setDelay_flash_request(sc::integer delay_flash_request);

			/*! Gets the value of the variable 'delay_flash_page' that is defined in the default interface scope. */
			sc::integer getDelay_flash_page() const;

			/*! Sets the value of the variable 'delay_flash_page' that is defined in the default interface scope. */
			void setDelay_flash_page(sc::integer delay_flash_page);

			/*! Gets the value of the variable 'count' that is defined in the default interface scope. */
			sc::integer getCount() const;

			/*! Sets the value of the variable 'count' that is defined in the default interface scope. */
			void setCount(sc::integer count);

			/*! Gets the value of the variable 'retry' that is defined in the default interface scope. */
			static sc::integer getRetry() ;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void ping() = 0;

				virtual void reset() = 0;

				virtual void flashRequest() = 0;

				virtual void flashCompletePage() = 0;

				virtual void flashRestPage() = 0;

				virtual void flashCheck() = 0;

				virtual void quit() = 0;

				virtual bool hasPages() = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback);

			/*
			 * Functions inherited from StatemachineInterface
			 */
			void enter() override;

			void exit() override;

			/*!
			 * Checks if the state machine is active (until 2.4.1 this method was used for states).
			 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
			 */
			bool isActive() const override;


			/*!
			* Checks if all active states are final.
			* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
			*/
			bool isFinal() const override;

			/*!
			 * Checks if member of the state machine must be set. For example an operation callback.
			 */
			bool check() const;

			/*
			 * Functions inherited from TimedStatemachineInterface
			 */
			void setTimerService(sc::timer::TimerServiceInterface * timerService_) override;

			sc::timer::TimerServiceInterface * getTimerService() override;

			void raiseTimeEvent(sc::eventid event) override;

			sc::integer getNumberOfParallelTimeEvents() override;



			/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
			bool isStateActive(State state) const;

			//! number of time events used by the state machine.
			static const sc::integer timeEventsCount = 6;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		public slots:
			/*! Slot for the in event 'complete' that is defined in the default interface scope. */
			void complete();


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			UpdateStatechart(const UpdateStatechart & rhs);
			UpdateStatechart & operator=(const UpdateStatechart &);

			sc::integer timeout;
			sc::integer delay_boot;
			sc::integer delay_reset;
			sc::integer delay_flash_request;
			sc::integer delay_flash_page;
			sc::integer count;
			static const sc::integer retry;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Ping();
			void enact_main_region_Reset();
			void enact_main_region_Flash_Request();
			void enact_main_region_Flash_Complete_Page();
			void enact_main_region_Flash_Rest();
			void enact_main_region_Flash_Check();
			void exact_main_region_Ping();
			void exact_main_region_Reset();
			void exact_main_region_Flash_Request();
			void exact_main_region_Flash_Complete_Page();
			void exact_main_region_Flash_Rest();
			void exact_main_region_Flash_Check();
			void enseq_main_region_Ping_default();
			void enseq_main_region_Reset_default();
			void enseq_main_region_Flash_Request_default();
			void enseq_main_region_Flash_Complete_Page_default();
			void enseq_main_region_Flash_Rest_default();
			void enseq_main_region_Flash_Check_default();
			void enseq_main_region__final__default();
			void enseq_main_region_default();
			void exseq_main_region_Ping();
			void exseq_main_region_Reset();
			void exseq_main_region_Flash_Request();
			void exseq_main_region_Flash_Complete_Page();
			void exseq_main_region_Flash_Rest();
			void exseq_main_region_Flash_Check();
			void exseq_main_region__final_();
			void exseq_main_region();
			void react_main_region__choice_0();
			void react_main_region__choice_1();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Ping_react(const sc::integer transitioned_before);
			sc::integer main_region_Reset_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Request_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Complete_Page_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Rest_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Check_react(const sc::integer transitioned_before);
			sc::integer main_region__final__react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'complete' of default interface scope is active. */
			bool complete_raised;



		};


		inline UpdateStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* UPDATESTATECHART_H_ */