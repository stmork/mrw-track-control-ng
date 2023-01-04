/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "OperatingMode.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace mrw
{
	namespace test
	{

		void wait();
		void initial();
		void failAfterStart();
		void doOperating();
		void doEdit();
		mrw::statechart::OperatingMode * statechart;


		class CanConnectBusMock
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

			void connectBus()
			{
				++callCount;
			}
			void reset()
			{
				callCount = 0;
			}
		};
		static CanConnectBusMock * canConnectBusMock;

		class ResetTransactionMock
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

			void resetTransaction()
			{
				++callCount;
			}
			void reset()
			{
				callCount = 0;
			}
		};
		static ResetTransactionMock * resetTransactionMock;

		class CanIsConnectedMock
		{
			typedef bool (CanIsConnectedMock::*functiontype)();
		public:
			bool (CanIsConnectedMock::*canIsConnectedBehaviorDefault)();

			bool isConnected1()
			{
				return (true);
			}

			bool isConnected2()
			{
				return (false);
			}

			bool isConnectedDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return canIsConnectedBehaviorDefault;
			}

			void setDefaultBehavior(bool (CanIsConnectedMock::*defaultBehavior)())
			{
				canIsConnectedBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&CanIsConnectedMock::isConnectedDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static CanIsConnectedMock * canIsConnectedMock;

		class MockDefault : public mrw::statechart::OperatingMode::OperationCallback
		{
		public:
			void resetTransaction()
			{
				resetTransactionMock->resetTransaction();
			}
			bool hasActiveRoutes()
			{
				return false;
			}
		};
		class MockCan : public mrw::statechart::OperatingMode::Can::OperationCallback
		{
		public:
			void connectBus()
			{
				canConnectBusMock->connectBus();
			}
			bool isConnected()
			{
				return (canIsConnectedMock->*(canIsConnectedMock->getBehavior()))();
			}
		};

//! The timers are managed by a timer service. */
		static TimedSctUnitRunner * runner;

		class OperatingModeTest : public ::testing::Test
		{
		protected:
			virtual void SetUp()
			{
				statechart = new mrw::statechart::OperatingMode();
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


		void wait()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Prepare_Bus));

			EXPECT_TRUE(canConnectBusMock->calledAtLeastOnce());


			canConnectBusMock->reset();
		}
		TEST_F(OperatingModeTest, wait)
		{
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			wait();
		}
		TEST_F(OperatingModeTest, timeoutAfterWait)
		{
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			wait();

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		void initial()
		{
			wait();

			statechart->can
			()->raiseConnected();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Init));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			resetTransactionMock->reset();
		}
		TEST_F(OperatingModeTest, initial)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			initial();
		}
		void failAfterStart()
		{
			initial();

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		TEST_F(OperatingModeTest, failAfterStart)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();
		}
		TEST_F(OperatingModeTest, timeoutAfterStart)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			initial();

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		TEST_F(OperatingModeTest, clearFailState)
		{
			canIsConnectedMock = new CanIsConnectedMock();
			canIsConnectedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected1);

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Init));

			EXPECT_TRUE(statechart->isRaisedCleared());

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			canIsConnectedMock->reset();
			resetTransactionMock->reset();
		}
		TEST_F(OperatingModeTest, stayFailState)
		{
			canIsConnectedMock = new CanIsConnectedMock();
			canIsConnectedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected2);

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Failed));


			canIsConnectedMock->reset();
		}
		TEST_F(OperatingModeTest, editFailed)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			statechart->raiseEdit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Editing));

			EXPECT_TRUE(statechart->isRaisedCleared());

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (true));


		}
		void doOperating()
		{
			initial();

			statechart->raiseStarted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Operating));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (true));


		}
		TEST_F(OperatingModeTest, doOperating)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();
		}
		TEST_F(OperatingModeTest, failWhileOperating)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Operating));


		}
		TEST_F(OperatingModeTest, resetWhileOperating)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();

			statechart->raiseInit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Init));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			resetTransactionMock->reset();
		}
		void doEdit()
		{
			doOperating();

			statechart->raiseEdit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Editing));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (true));


		}
		TEST_F(OperatingModeTest, doEdit)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doEdit();
		}
		TEST_F(OperatingModeTest, operateAfterEdit)
		{
			resetTransactionMock = new ResetTransactionMock();
			canConnectBusMock = new CanConnectBusMock();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doEdit();

			statechart->raiseOperate();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingMode::State::main_region_Operating));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (true));


		}

	}
}

