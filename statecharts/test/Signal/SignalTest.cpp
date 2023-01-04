/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "SignalStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace mrw
{
	namespace test
	{

		void failState();
		void idle();
		void turning();
		void pending();
		void failSending();
		mrw::statechart::SignalStatechart * statechart;


		class PrepareMock
		{
		public:
			int callCount;

			bool calledAtLeast(const int times)
			{
				return (callCount >= times);
			}

			bool calledAtLeastOnce()
			{
				return (callCount > 0);
			}

			void prepare()
			{
				++callCount;
			}
			void reset()
			{
				callCount = 0;
			}
		};
		static PrepareMock * prepareMock;

		class SendMock
		{
		public:
			int callCount;

			bool calledAtLeast(const int times)
			{
				return (callCount >= times);
			}

			bool calledAtLeastOnce()
			{
				return (callCount > 0);
			}

			void send()
			{
				++callCount;
			}
			void reset()
			{
				callCount = 0;
			}
		};
		static SendMock * sendMock;

		class HasSignalMock
		{
			typedef bool (HasSignalMock::*functiontype)();
		public:
			bool (HasSignalMock::*hasSignalBehaviorDefault)();

			bool hasSignal1()
			{
				return (false);
			}

			bool hasSignal2()
			{
				return (true);
			}

			bool hasSignalDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return hasSignalBehaviorDefault;
			}

			void setDefaultBehavior(bool (HasSignalMock::*defaultBehavior)())
			{
				hasSignalBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&HasSignalMock::hasSignalDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static HasSignalMock * hasSignalMock;

		class MockDefault : public mrw::statechart::SignalStatechart::OperationCallback
		{
		public:
			bool hasSignal()
			{
				return (hasSignalMock->*(hasSignalMock->getBehavior()))();
			}
			void prepare()
			{
				prepareMock->prepare();
			}
			void send()
			{
				sendMock->send();
			}
			void dump()
			{
			}
		};

//! The timers are managed by a timer service. */
		static TimedSctUnitRunner * runner;

		class SignalTest : public ::testing::Test
		{
		protected:
			virtual void SetUp()
			{
				statechart = new mrw::statechart::SignalStatechart();
				runner = new TimedSctUnitRunner(
					statechart,
					true,
					200
				);
				statechart->setTimerService(runner);
			}
			virtual void TearDown()
			{
				delete statechart;
				delete runner;
			}
		};

		void failState()
		{
			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Fail));

			EXPECT_TRUE((statechart->getSymbol()) == (statechart->getOFF()));

			EXPECT_TRUE(prepareMock->calledAtLeastOnce());

			EXPECT_TRUE(sendMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedFailed());


		}

		void idle()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));


		}
		TEST_F(SignalTest, idle)
		{

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();
		}
		TEST_F(SignalTest, turnWithoutSignal)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

			statechart->raiseTurn(statechart->getGO());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

			EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));


			hasSignalMock->reset();
		}
		void turning()
		{
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal2);

			statechart->raiseTurn(statechart->getGO());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send));

			EXPECT_TRUE((statechart->getSymbol()) == (statechart->getGO()));

			EXPECT_TRUE(prepareMock->calledAtLeastOnce());

			EXPECT_TRUE(sendMock->calledAtLeastOnce());


			hasSignalMock->reset();
			prepareMock->reset();
			sendMock->reset();
		}
		TEST_F(SignalTest, turning)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turning();
		}
		void pending()
		{
			turning();

			statechart->raiseQueued();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending));


		}
		TEST_F(SignalTest, pending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			pending();
		}
		TEST_F(SignalTest, turnCompleted)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turning();

			statechart->raiseOk();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

			EXPECT_TRUE(statechart->isRaisedCompleted());


		}
		TEST_F(SignalTest, turnCompletedAfterPending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			pending();

			statechart->raiseOk();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

			EXPECT_TRUE(statechart->isRaisedCompleted());


		}
		void failSending()
		{
			turning();

			statechart->raiseFail();

			failState();


		}
		TEST_F(SignalTest, failSending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failSending();
		}
		TEST_F(SignalTest, failPending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			pending();

			statechart->raiseFail();

			failState();


		}
		TEST_F(SignalTest, timeoutSending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turning();

			runner->proceed_time(statechart->getTimeout());

			failState();


		}
		TEST_F(SignalTest, timeoutPending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			pending();

			runner->proceed_time(statechart->getTimeout());

			failState();


		}
		TEST_F(SignalTest, clearing)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			sendMock = new SendMock();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failSending();

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));


		}

	}
}

