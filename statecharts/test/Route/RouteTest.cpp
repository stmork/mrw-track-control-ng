/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "RouteStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace mrw
{
	namespace test
	{

		void disabled();
		void start();
		void switches();
		void turningSignals();
		void extendingSignals();
		void sections();
		void activate();
		void deactivate();
		void failTurningSwitchesIncomplete();
		void failTurningSignalsIncomplete();
		void failTurningSectionsIncomplete();
		mrw::statechart::RouteStatechart * statechart;


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

		class DeactivateSectionsMock
		{
			typedef void (DeactivateSectionsMock::*functiontype)();
		public:
			void (DeactivateSectionsMock::*deactivateSectionsBehaviorDefault)();
			int callCount;

			void deactivateSections1()
			{
			}

			void deactivateSectionsDefault()
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

			void deactivateSections()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return deactivateSectionsBehaviorDefault;
			}

			void setDefaultBehavior(void (DeactivateSectionsMock::*defaultBehavior)())
			{
				deactivateSectionsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DeactivateSectionsMock::deactivateSectionsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static DeactivateSectionsMock * deactivateSectionsMock;

		class UnlockSignalsMock
		{
			typedef void (UnlockSignalsMock::*functiontype)();
		public:
			void (UnlockSignalsMock::*unlockSignalsBehaviorDefault)();
			int callCount;

			void unlockSignals1()
			{
			}

			void unlockSignalsDefault()
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

			void unlockSignals()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return unlockSignalsBehaviorDefault;
			}

			void setDefaultBehavior(void (UnlockSignalsMock::*defaultBehavior)())
			{
				unlockSignalsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UnlockSignalsMock::unlockSignalsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static UnlockSignalsMock * unlockSignalsMock;

		class UnlockSwitchesMock
		{
			typedef void (UnlockSwitchesMock::*functiontype)();
		public:
			void (UnlockSwitchesMock::*unlockSwitchesBehaviorDefault)();
			int callCount;

			void unlockSwitches1()
			{
			}

			void unlockSwitchesDefault()
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

			void unlockSwitches()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return unlockSwitchesBehaviorDefault;
			}

			void setDefaultBehavior(void (UnlockSwitchesMock::*defaultBehavior)())
			{
				unlockSwitchesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UnlockSwitchesMock::unlockSwitchesDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static UnlockSwitchesMock * unlockSwitchesMock;

		class FailMock
		{
			typedef void (FailMock::*functiontype)();
		public:
			void (FailMock::*failBehaviorDefault)();

			void fail1()
			{
			}

			void failDefault()
			{
			}

			functiontype getBehavior()
			{
				return failBehaviorDefault;
			}

			void setDefaultBehavior(void (FailMock::*defaultBehavior)())
			{
				failBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&FailMock::failDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static FailMock * failMock;

		class TryCompleteMock
		{
			typedef void (TryCompleteMock::*functiontype)();
		public:
			void (TryCompleteMock::*tryCompleteBehaviorDefault)();

			void tryComplete1()
			{
			}

			void tryCompleteDefault()
			{
			}

			functiontype getBehavior()
			{
				return tryCompleteBehaviorDefault;
			}

			void setDefaultBehavior(void (TryCompleteMock::*defaultBehavior)())
			{
				tryCompleteBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&TryCompleteMock::tryCompleteDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static TryCompleteMock * tryCompleteMock;

		class TurnSwitchesMock
		{
			typedef void (TurnSwitchesMock::*functiontype)();
		public:
			void (TurnSwitchesMock::*turnSwitchesBehaviorDefault)();
			int callCount;

			void turnSwitches1()
			{
			}

			void turnSwitchesDefault()
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

			void turnSwitches()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return turnSwitchesBehaviorDefault;
			}

			void setDefaultBehavior(void (TurnSwitchesMock::*defaultBehavior)())
			{
				turnSwitchesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&TurnSwitchesMock::turnSwitchesDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static TurnSwitchesMock * turnSwitchesMock;

		class ActivateSectionsMock
		{
			typedef void (ActivateSectionsMock::*functiontype)();
		public:
			void (ActivateSectionsMock::*activateSectionsBehaviorDefault)();
			int callCount;

			void activateSections1()
			{
			}

			void activateSectionsDefault()
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

			void activateSections()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return activateSectionsBehaviorDefault;
			}

			void setDefaultBehavior(void (ActivateSectionsMock::*defaultBehavior)())
			{
				activateSectionsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ActivateSectionsMock::activateSectionsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static ActivateSectionsMock * activateSectionsMock;

		class TurnSignalsMock
		{
			typedef void (TurnSignalsMock::*functiontype)();
		public:
			void (TurnSignalsMock::*turnSignalsBehaviorDefault)();
			int callCount;

			void turnSignals1()
			{
			}

			void turnSignalsDefault()
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

			void turnSignals()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return turnSignalsBehaviorDefault;
			}

			void setDefaultBehavior(void (TurnSignalsMock::*defaultBehavior)())
			{
				turnSignalsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&TurnSignalsMock::turnSignalsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static TurnSignalsMock * turnSignalsMock;

		class ExtendSignalsMock
		{
			typedef void (ExtendSignalsMock::*functiontype)();
		public:
			void (ExtendSignalsMock::*extendSignalsBehaviorDefault)();
			int callCount;

			void extendSignals1()
			{
			}

			void extendSignalsDefault()
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

			void extendSignals()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return extendSignalsBehaviorDefault;
			}

			void setDefaultBehavior(void (ExtendSignalsMock::*defaultBehavior)())
			{
				extendSignalsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ExtendSignalsMock::extendSignalsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static ExtendSignalsMock * extendSignalsMock;

		class IsCompletedMock
		{
			typedef bool (IsCompletedMock::*functiontype)();
		public:
			bool (IsCompletedMock::*isCompletedBehaviorDefault)();

			bool isCompleted1()
			{
				return (true);
			}

			bool isCompleted2()
			{
				return (false);
			}

			bool isCompletedDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return isCompletedBehaviorDefault;
			}

			void setDefaultBehavior(bool (IsCompletedMock::*defaultBehavior)())
			{
				isCompletedBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&IsCompletedMock::isCompletedDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static IsCompletedMock * isCompletedMock;

		class MockDefault : public mrw::statechart::RouteStatechart::OperationCallback
		{
		public:
			void resetTransaction()
			{
				resetTransactionMock->resetTransaction();
				return (resetTransactionMock->*(resetTransactionMock->getBehavior()))();
			}
			void fail()
			{
				return (failMock->*(failMock->getBehavior()))();
			}
			void tryComplete()
			{
				return (tryCompleteMock->*(tryCompleteMock->getBehavior()))();
			}
			bool isCompleted()
			{
				return (isCompletedMock->*(isCompletedMock->getBehavior()))();
			}
			void turnSwitches()
			{
				turnSwitchesMock->turnSwitches();
				return (turnSwitchesMock->*(turnSwitchesMock->getBehavior()))();
			}
			void activateSections()
			{
				activateSectionsMock->activateSections();
				return (activateSectionsMock->*(activateSectionsMock->getBehavior()))();
			}
			void turnSignals()
			{
				turnSignalsMock->turnSignals();
				return (turnSignalsMock->*(turnSignalsMock->getBehavior()))();
			}
			void extendSignals()
			{
				extendSignalsMock->extendSignals();
				return (extendSignalsMock->*(extendSignalsMock->getBehavior()))();
			}
			void deactivateSections()
			{
				deactivateSectionsMock->deactivateSections();
				return (deactivateSectionsMock->*(deactivateSectionsMock->getBehavior()))();
			}
			void unlockSignals()
			{
				unlockSignalsMock->unlockSignals();
				return (unlockSignalsMock->*(unlockSignalsMock->getBehavior()))();
			}
			void unlockSwitches()
			{
				unlockSwitchesMock->unlockSwitches();
				return (unlockSwitchesMock->*(unlockSwitchesMock->getBehavior()))();
			}
		};

//! The timers are managed by a timer service. */
		static TimedSctUnitRunner * runner;

		class RouteTest : public ::testing::Test
		{
		protected:
			virtual void SetUp()
			{
				statechart = new mrw::statechart::RouteStatechart();
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

		void disabled()
		{
			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Disable));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSwitchesMock->calledAtLeastOnce());


		}

		void start()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Start));












			resetTransactionMock->reset();
			failMock->reset();
			tryCompleteMock->reset();
			turnSwitchesMock->reset();
			activateSectionsMock->reset();
			turnSignalsMock->reset();
			extendSignalsMock->reset();
			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
			unlockSwitchesMock->reset();
		}
		TEST_F(RouteTest, start)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			start();
		}
		void switches()
		{
			start();

			statechart->raiseExtended();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSwitchesMock->reset();
		}
		TEST_F(RouteTest, switches)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			switches();
		}
		void turningSignals()
		{
			switches();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSignalsMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSignalsMock->reset();
		}
		TEST_F(RouteTest, turningSignals)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSignals();
		}
		void extendingSignals()
		{
			turningSignals();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Updating));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(extendSignalsMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			extendSignalsMock->reset();
		}
		TEST_F(RouteTest, extendingSignals)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			extendingSignals();
		}
		void sections()
		{
			extendingSignals();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Section_Activation));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(activateSectionsMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			activateSectionsMock->reset();
		}
		TEST_F(RouteTest, sections)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			sections();
		}
		void activate()
		{
			sections();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isRaisedActivated());


		}
		TEST_F(RouteTest, activate)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			activate();
		}
		TEST_F(RouteTest, extend)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			activate();

			statechart->raiseExtended();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSwitchesMock->reset();
		}
		void deactivate()
		{
			activate();

			statechart->raiseDisable();

			disabled();


		}
		TEST_F(RouteTest, deactivate)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			deactivate();
		}
		TEST_F(RouteTest, finish)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			deactivate();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedFinished());

			EXPECT_TRUE(statechart->isActive());


		}
		TEST_F(RouteTest, timeout)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			deactivate();

			runner->proceed_time(statechart->getEmergency());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedFinished());

			EXPECT_TRUE(statechart->isActive());


		}
		TEST_F(RouteTest, failActive)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			activate();

			statechart->raiseFailed();

			disabled();


		}
		TEST_F(RouteTest, failTurningSwitches)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			switches();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSignals)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failExtendingSignals)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			extendingSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSections)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			sections();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		void failTurningSwitchesIncomplete()
		{
			switches();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSwitchesIncomplete)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSwitchesIncomplete();
		}
		void failTurningSignalsIncomplete()
		{
			turningSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSignalsIncomplete)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSignalsIncomplete();
		}
		TEST_F(RouteTest, failExtendingSignalsIncomplete)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			extendingSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


			isCompletedMock->reset();
		}
		void failTurningSectionsIncomplete()
		{
			sections();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSectionsIncomplete)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSectionsIncomplete();
		}
		TEST_F(RouteTest, failWaitSwitches)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSwitchesIncomplete();

			statechart->raiseCompleted();

			disabled();


		}
		TEST_F(RouteTest, failWaitSignals)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSignalsIncomplete();

			statechart->raiseCompleted();

			disabled();


		}
		TEST_F(RouteTest, failWaitSections)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSectionsIncomplete();

			statechart->raiseCompleted();

			disabled();


		}
		TEST_F(RouteTest, timeoutWaitSwitches)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSwitchesIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();


		}
		TEST_F(RouteTest, timeoutWaitSignals)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSignalsIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();


		}
		TEST_F(RouteTest, timeoutWaitSections)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			failTurningSectionsIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();


		}
		TEST_F(RouteTest, timeoutTurningSwitches)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			switches();

			runner->proceed_time(statechart->getSwitch_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			disabled();


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}
		TEST_F(RouteTest, timeoutTurningSignals)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSignals();

			runner->proceed_time(statechart->getSignal_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			disabled();


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}
		TEST_F(RouteTest, timeoutExtendingSignals)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			extendingSignals();

			runner->proceed_time(statechart->getSignal_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			disabled();


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}
		TEST_F(RouteTest, timeoutTurningSections)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			sections();

			runner->proceed_time(statechart->getSection_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			disabled();


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}

	}
}

