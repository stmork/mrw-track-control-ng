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
			typedef void (PrepareMock::*functiontype)();
		public:
			void (PrepareMock::*prepareBehaviorDefault)();
			int callCount;

			void prepare1()
			{
			}

			void prepareDefault()
			{
			}

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

			functiontype getBehavior()
			{
				return prepareBehaviorDefault;
			}

			void setDefaultBehavior(void (PrepareMock::*defaultBehavior)())
			{
				prepareBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&PrepareMock::prepareDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static PrepareMock * prepareMock;

		class SendMock
		{
			typedef void (SendMock::*functiontype)();
		public:
			void (SendMock::*sendBehaviorDefault)();
			int callCount;

			void send1()
			{
			}

			void sendDefault()
			{
			}

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

			functiontype getBehavior()
			{
				return sendBehaviorDefault;
			}

			void setDefaultBehavior(void (SendMock::*defaultBehavior)())
			{
				sendBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&SendMock::sendDefault);
			}

			void reset()
			{
				initializeBehavior();
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
				return (true);
			}

			bool hasSignal2()
			{
				return (false);
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

		class DumpMock
		{
			typedef void (DumpMock::*functiontype)();
		public:
			void (DumpMock::*dumpBehaviorDefault)();

			void dump1()
			{
			}

			void dumpDefault()
			{
			}

			functiontype getBehavior()
			{
				return dumpBehaviorDefault;
			}

			void setDefaultBehavior(void (DumpMock::*defaultBehavior)())
			{
				dumpBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DumpMock::dumpDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static DumpMock * dumpMock;

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
				return (prepareMock->*(prepareMock->getBehavior()))();
			}
			void send()
			{
				sendMock->send();
				return (sendMock->*(sendMock->getBehavior()))();
			}
			void dump()
			{
				return (dumpMock->*(dumpMock->getBehavior()))();
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

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);





			hasSignalMock->reset();
			prepareMock->reset();
			sendMock->reset();
			dumpMock->reset();
		}
		TEST_F(SignalTest, idle)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();
		}
		TEST_F(SignalTest, failWithSignal)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

			statechart->raiseFail();

			failState();


			hasSignalMock->reset();
		}
		TEST_F(SignalTest, failWithoutSignal)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal2);

			statechart->raiseFail();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));


			hasSignalMock->reset();
		}
		TEST_F(SignalTest, turnWithoutSignal)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal2);

			statechart->raiseTurn(statechart->getGO());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

			EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));


			hasSignalMock->reset();
		}
		void turning()
		{
			idle();

			hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			pending();
		}
		TEST_F(SignalTest, turnCompleted)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failSending();
		}
		TEST_F(SignalTest, failPending)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

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
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failSending();

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));


		}
		TEST_F(SignalTest, doExit)
		{
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			idle();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			turning();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			pending();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			failSending();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());


		}

	}
}

