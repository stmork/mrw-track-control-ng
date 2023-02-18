/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "OperatingModeStatechart.h"
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
		void initWhileOperatingWithRoutes();
		void disableCompletedWithRoutes();
		void disableCompletedWithoutRoutes();
		void doEdit();
		void doManual();
		void doQuitManual();
		void doWaitManual();
		void doQuitWithRoute();
		mrw::statechart::OperatingModeStatechart * statechart;


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

		class HasActiveRoutesMock
		{
			typedef bool (HasActiveRoutesMock::*functiontype)();
		public:
			bool (HasActiveRoutesMock::*hasActiveRoutesBehaviorDefault)();

			bool hasActiveRoutes1()
			{
				return (false);
			}

			bool hasActiveRoutes2()
			{
				return (true);
			}

			bool hasActiveRoutesDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return hasActiveRoutesBehaviorDefault;
			}

			void setDefaultBehavior(bool (HasActiveRoutesMock::*defaultBehavior)())
			{
				hasActiveRoutesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutesDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static HasActiveRoutesMock * hasActiveRoutesMock;

		class IsManualValidMock
		{
			typedef bool (IsManualValidMock::*functiontype)();
		public:
			bool (IsManualValidMock::*isManualValidBehaviorDefault)();

			bool isManualValid1()
			{
				return (true);
			}

			bool isManualValidDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return isManualValidBehaviorDefault;
			}

			void setDefaultBehavior(bool (IsManualValidMock::*defaultBehavior)())
			{
				isManualValidBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&IsManualValidMock::isManualValidDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static IsManualValidMock * isManualValidMock;

		class DisableRoutesMock
		{
			typedef void (DisableRoutesMock::*functiontype)();
		public:
			void (DisableRoutesMock::*disableRoutesBehaviorDefault)();
			int callCount;

			void disableRoutes1()
			{
			}

			void disableRoutesDefault()
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

			void disableRoutes()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return disableRoutesBehaviorDefault;
			}

			void setDefaultBehavior(void (DisableRoutesMock::*defaultBehavior)())
			{
				disableRoutesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DisableRoutesMock::disableRoutesDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static DisableRoutesMock * disableRoutesMock;

		class ResetTransactionMock
		{
			typedef void (ResetTransactionMock::*functiontype)();
		public:
			void (ResetTransactionMock::*resetTransactionBehaviorDefault)();
			int callCount;

			void resetTransaction1()
			{
			}

			void resetTransactionDefault()
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

			void resetTransaction()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return resetTransactionBehaviorDefault;
			}

			void setDefaultBehavior(void (ResetTransactionMock::*defaultBehavior)())
			{
				resetTransactionBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ResetTransactionMock::resetTransactionDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static ResetTransactionMock * resetTransactionMock;

		class ActivateManualMock
		{
			typedef void (ActivateManualMock::*functiontype)();
			struct parameters
			{
				bool active;
				void (ActivateManualMock::*behavior)();
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->active == other.active);
				}
			};
		public:
			std::list<ActivateManualMock::parameters> mocks;
			std::list<ActivateManualMock::parameters> paramCount;
			void (ActivateManualMock::*activateManualBehaviorDefault)();
			int callCount;

			void activateManual1()
			{
			}

			void activateManualDefault()
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

			void setActivateManualBehavior(const bool active, void (ActivateManualMock::*func)())
			{
				parameters p;
				p.active = active;
				p.behavior = func;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					mocks.erase(i);
				}
				mocks.push_back(p);
			}

			bool calledAtLeast(const int times, const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void activateManual(const bool active)
			{
				++callCount;

				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					p.callCount = (++i->callCount);
					paramCount.erase(i);

				}
				else
				{
					p.callCount = 1;
				}
				paramCount.push_back(p);
			}

			functiontype getBehavior(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					return  i->behavior;
				}
				else
				{
					return activateManualBehaviorDefault;
				}
			}

			void setDefaultBehavior(void (ActivateManualMock::*defaultBehavior)())
			{
				activateManualBehaviorDefault = defaultBehavior;
				mocks.clear();
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ActivateManualMock::activateManualDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
				paramCount.clear();
				mocks.clear();
			}
		};
		static ActivateManualMock * activateManualMock;

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

		class MockDefault : public mrw::statechart::OperatingModeStatechart::OperationCallback
		{
		public:
			void resetTransaction()
			{
				resetTransactionMock->resetTransaction();
				return (resetTransactionMock->*(resetTransactionMock->getBehavior()))();
			}
			bool isManualValid()
			{
				return (isManualValidMock->*(isManualValidMock->getBehavior()))();
			}
			bool hasActiveRoutes()
			{
				return (hasActiveRoutesMock->*(hasActiveRoutesMock->getBehavior()))();
			}
			void disableRoutes()
			{
				disableRoutesMock->disableRoutes();
				return (disableRoutesMock->*(disableRoutesMock->getBehavior()))();
			}
			void activateManual(bool active)
			{
				activateManualMock->activateManual(active);
				return (activateManualMock->*(activateManualMock->getBehavior(active)))();
			}
		};
		class MockCan : public mrw::statechart::OperatingModeStatechart::Can::OperationCallback
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
				statechart = new mrw::statechart::OperatingModeStatechart();
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

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus));

			EXPECT_TRUE(canConnectBusMock->calledAtLeastOnce());

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			isManualValidMock->setDefaultBehavior(&IsManualValidMock::isManualValid1);





			canConnectBusMock->reset();
			hasActiveRoutesMock->reset();
			isManualValidMock->reset();
			disableRoutesMock->reset();
			resetTransactionMock->reset();
			activateManualMock->reset();
		}
		TEST_F(OperatingModeTest, wait)
		{
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			wait();
		}
		TEST_F(OperatingModeTest, timeoutAfterWait)
		{
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			wait();

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		void initial()
		{
			wait();

			statechart->can
			()->raiseConnected();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			resetTransactionMock->reset();
		}
		TEST_F(OperatingModeTest, initial)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

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

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		TEST_F(OperatingModeTest, failAfterStart)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();
		}
		TEST_F(OperatingModeTest, timeoutAfterStart)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			initial();

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

			EXPECT_TRUE(statechart->isRaisedFailing());


		}
		TEST_F(OperatingModeTest, clearFailState)
		{
			canIsConnectedMock = new CanIsConnectedMock();
			canIsConnectedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected1);

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

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
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected2);

			statechart->raiseClear();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));


			canIsConnectedMock->reset();
		}
		TEST_F(OperatingModeTest, editFailed)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			statechart->raiseEdit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (true));


		}
		void doOperating()
		{
			initial();

			statechart->raiseStarted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (true));


		}
		TEST_F(OperatingModeTest, doOperating)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();
		}
		TEST_F(OperatingModeTest, failWhileOperating)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));


		}
		void initWhileOperatingWithRoutes()
		{
			doOperating();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

			statechart->raiseInit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(disableRoutesMock->calledAtLeastOnce());


			hasActiveRoutesMock->reset();
			disableRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, initWhileOperatingWithRoutes)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			initWhileOperatingWithRoutes();
		}
		TEST_F(OperatingModeTest, initWhileOperatingWithoutRoutes)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseInit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			hasActiveRoutesMock->reset();
			resetTransactionMock->reset();
		}
		void disableCompletedWithRoutes()
		{
			initWhileOperatingWithRoutes();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable));


			hasActiveRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, disableCompletedWithRoutes)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			disableCompletedWithRoutes();
		}
		void disableCompletedWithoutRoutes()
		{
			initWhileOperatingWithRoutes();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			hasActiveRoutesMock->reset();
			resetTransactionMock->reset();
		}
		TEST_F(OperatingModeTest, disableCompletedWithoutRoutes)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			disableCompletedWithoutRoutes();
		}
		void doEdit()
		{
			doOperating();

			statechart->raiseEdit();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (true));


		}
		TEST_F(OperatingModeTest, doEdit)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doEdit();
		}
		void doManual()
		{
			doOperating();

			isManualValidMock->setDefaultBehavior(&IsManualValidMock::isManualValid1);

			statechart->raiseManual(true);

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Manual));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedPlaying());

			EXPECT_TRUE((statechart->getPlayingValue()) == (true));

			EXPECT_TRUE(activateManualMock->calledAtLeastOnce(true));


			isManualValidMock->reset();
			activateManualMock->reset();
		}
		TEST_F(OperatingModeTest, doManual)
		{
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doManual();
		}
		TEST_F(OperatingModeTest, doLeaveManual)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doManual();

			statechart->raiseManual(false);

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

			EXPECT_TRUE(statechart->isRaisedPlaying());

			EXPECT_TRUE((statechart->getPlayingValue()) == (false));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedStart());


			resetTransactionMock->reset();
		}
		void doQuitManual()
		{
			doManual();

			statechart->raiseFinalize();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Wait));

			EXPECT_TRUE(statechart->isRaisedPlaying());

			EXPECT_TRUE((statechart->getPlayingValue()) == (false));

			EXPECT_TRUE(activateManualMock->calledAtLeastOnce(false));


			activateManualMock->reset();
		}
		TEST_F(OperatingModeTest, doQuitManual)
		{
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doQuitManual();
		}
		void doWaitManual()
		{
			doQuitManual();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedQuit());


		}
		TEST_F(OperatingModeTest, doWaitManual)
		{
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doWaitManual();
		}
		TEST_F(OperatingModeTest, operateAfterEdit)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doEdit();

			statechart->raiseOperate();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (true));


		}
		TEST_F(OperatingModeTest, doQuitOperating)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doOperating();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseFinalize();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedQuit());


			hasActiveRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, doQuitEditing)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doEdit();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseFinalize();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedEditing());

			EXPECT_TRUE((statechart->getEditingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedQuit());


			hasActiveRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, doQuitFailed)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			failAfterStart();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseFinalize();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedQuit());


			hasActiveRoutesMock->reset();
		}
		void doQuitWithRoute()
		{
			doOperating();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

			statechart->raiseFinalize();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Exit));

			EXPECT_TRUE(disableRoutesMock->calledAtLeastOnce());

			EXPECT_TRUE(statechart->isRaisedOperating());

			EXPECT_TRUE((statechart->getOperatingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedQuitting());

			EXPECT_TRUE((statechart->getQuittingValue()) == (true));


			hasActiveRoutesMock->reset();
			disableRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, doQuitWithRoute)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doQuitWithRoute();
		}
		TEST_F(OperatingModeTest, doCompleteQuit)
		{
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			doQuitWithRoute();

			hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedQuitting());

			EXPECT_TRUE((statechart->getQuittingValue()) == (false));

			EXPECT_TRUE(statechart->isRaisedQuit());


			hasActiveRoutesMock->reset();
		}
		TEST_F(OperatingModeTest, doExit)
		{
			canConnectBusMock = new CanConnectBusMock();
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();

			MockDefault defaultMock;
			MockCan canMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->can()->setOperationCallback(&canMock);
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			wait();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			initial();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doOperating();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doEdit();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doManual();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doWaitManual();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			initWhileOperatingWithRoutes();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			disableCompletedWithoutRoutes();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			disableCompletedWithRoutes();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doQuitWithRoute();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			failAfterStart();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());


		}

	}
}

