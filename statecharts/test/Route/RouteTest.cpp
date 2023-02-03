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
		void turningSwitches();
		void turningFlanks();
		void turningSignals();
		void extendingSignals();
		void sections();
		void activate();
		void deactivate();
		void unlock();
		void finish();
		void failTurningSwitchesIncomplete();
		void failTurningSignalsIncomplete();
		void failTurningSectionsIncomplete();
		void emergencySections();
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

		class TryCompleteMock
		{
			typedef void (TryCompleteMock::*functiontype)();
		public:
			void (TryCompleteMock::*tryCompleteBehaviorDefault)();
			int callCount;

			void tryComplete1()
			{
			}

			void tryCompleteDefault()
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

			void tryComplete()
			{
				++callCount;
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
				callCount = 0;
			}
		};
		static TryCompleteMock * tryCompleteMock;

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

		class IsTourMock
		{
			typedef bool (IsTourMock::*functiontype)();
		public:
			bool (IsTourMock::*isTourBehaviorDefault)();

			bool isTour1()
			{
				return (false);
			}

			bool isTour2()
			{
				return (true);
			}

			bool isTourDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return isTourBehaviorDefault;
			}

			void setDefaultBehavior(bool (IsTourMock::*defaultBehavior)())
			{
				isTourBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&IsTourMock::isTourDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		static IsTourMock * isTourMock;

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

		class TurnFlanksMock
		{
			typedef void (TurnFlanksMock::*functiontype)();
		public:
			void (TurnFlanksMock::*turnFlanksBehaviorDefault)();
			int callCount;

			void turnFlanks1()
			{
			}

			void turnFlanksDefault()
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

			void turnFlanks()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return turnFlanksBehaviorDefault;
			}

			void setDefaultBehavior(void (TurnFlanksMock::*defaultBehavior)())
			{
				turnFlanksBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&TurnFlanksMock::turnFlanksDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static TurnFlanksMock * turnFlanksMock;

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

		class UnlockSectionsMock
		{
			typedef void (UnlockSectionsMock::*functiontype)();
		public:
			void (UnlockSectionsMock::*unlockSectionsBehaviorDefault)();
			int callCount;

			void unlockSections1()
			{
			}

			void unlockSectionsDefault()
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

			void unlockSections()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return unlockSectionsBehaviorDefault;
			}

			void setDefaultBehavior(void (UnlockSectionsMock::*defaultBehavior)())
			{
				unlockSectionsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UnlockSectionsMock::unlockSectionsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static UnlockSectionsMock * unlockSectionsMock;

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
			bool isTour()
			{
				return (isTourMock->*(isTourMock->getBehavior()))();
			}
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
				tryCompleteMock->tryComplete();
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
			void turnFlanks()
			{
				turnFlanksMock->turnFlanks();
				return (turnFlanksMock->*(turnFlanksMock->getBehavior()))();
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
			void activateSections()
			{
				activateSectionsMock->activateSections();
				return (activateSectionsMock->*(activateSectionsMock->getBehavior()))();
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
			void unlockSections()
			{
				unlockSectionsMock->unlockSections();
				return (unlockSectionsMock->*(unlockSectionsMock->getBehavior()))();
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

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


		}

		void start()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Start));




			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);











			resetTransactionMock->reset();
			failMock->reset();
			tryCompleteMock->reset();
			isTourMock->reset();
			turnSwitchesMock->reset();
			turnFlanksMock->reset();
			turnSignalsMock->reset();
			extendSignalsMock->reset();
			activateSectionsMock->reset();
			unlockSignalsMock->reset();
			unlockSwitchesMock->reset();
			deactivateSectionsMock->reset();
			unlockSectionsMock->reset();
		}
		TEST_F(RouteTest, start)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			start();
		}
		void turningSwitches()
		{
			start();

			statechart->raiseExtended();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSwitchesMock->reset();
			tryCompleteMock->reset();
		}
		TEST_F(RouteTest, turningSwitches)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSwitches();
		}
		void turningFlanks()
		{
			turningSwitches();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Flank_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnFlanksMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			isTourMock->reset();
			resetTransactionMock->reset();
			turnFlanksMock->reset();
			tryCompleteMock->reset();
		}
		TEST_F(RouteTest, turningFlanks)
		{
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningFlanks();
		}
		TEST_F(RouteTest, turningSignalsTour)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningFlanks();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSignalsMock->reset();
			tryCompleteMock->reset();
		}
		void turningSignals()
		{
			turningSwitches();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			isTourMock->reset();
			resetTransactionMock->reset();
			turnSignalsMock->reset();
			tryCompleteMock->reset();
		}
		TEST_F(RouteTest, turningSignals)
		{
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

			EXPECT_TRUE(extendSignalsMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			tryCompleteMock->reset();
			extendSignalsMock->reset();
		}
		TEST_F(RouteTest, extendingSignals)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			activateSectionsMock->reset();
			tryCompleteMock->reset();
		}
		TEST_F(RouteTest, sections)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			activate();

			statechart->raiseExtended();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			turnSwitchesMock->reset();
			tryCompleteMock->reset();
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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			deactivate();
		}
		void unlock()
		{
			deactivate();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Unlock));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());


			resetTransactionMock->reset();
			unlockSectionsMock->reset();
			tryCompleteMock->reset();
		}
		TEST_F(RouteTest, unlock)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			unlock();
		}
		void finish()
		{
			unlock();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region__final_));

			EXPECT_TRUE(statechart->isRaisedFinished());

			EXPECT_TRUE(statechart->isActive());


		}
		TEST_F(RouteTest, finish)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			finish();
		}
		TEST_F(RouteTest, timeout)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSwitches();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningFlanks)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningFlanks();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();


			isCompletedMock->reset();
		}
		TEST_F(RouteTest, failTurningSignals)
		{
			isCompletedMock = new IsCompletedMock();
			isCompletedMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			turningSwitches();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningSwitches();

			runner->proceed_time(statechart->getSwitch_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			disabled();


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}
		TEST_F(RouteTest, timeoutTurningFlanks)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			turningFlanks();

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
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

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
		void emergencySections()
		{
			sections();

			runner->proceed_time(statechart->getSection_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());


			deactivateSectionsMock->reset();
			unlockSignalsMock->reset();
		}
		TEST_F(RouteTest, emergencySections)
		{
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			emergencySections();
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
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			emergencySections();

			runner->proceed_time(statechart->getEmergency());

			disabled();


		}
		TEST_F(RouteTest, doExit)
		{
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock();
			tryCompleteMock->initializeBehavior();
			isTourMock = new IsTourMock();
			isTourMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock();
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock();
			turnFlanksMock->initializeBehavior();
			turnSignalsMock = new TurnSignalsMock();
			turnSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock();
			extendSignalsMock->initializeBehavior();
			activateSectionsMock = new ActivateSectionsMock();
			activateSectionsMock->initializeBehavior();
			unlockSignalsMock = new UnlockSignalsMock();
			unlockSignalsMock->initializeBehavior();
			unlockSwitchesMock = new UnlockSwitchesMock();
			unlockSwitchesMock->initializeBehavior();
			deactivateSectionsMock = new DeactivateSectionsMock();
			deactivateSectionsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock();
			unlockSectionsMock->initializeBehavior();

			MockDefault defaultMock;
			statechart->setOperationCallback(&defaultMock);
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			start();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			turningSwitches();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			turningFlanks();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			turningSignals();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			extendingSignals();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			sections();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			activate();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			deactivate();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			unlock();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			finish();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			emergencySections();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			failTurningSectionsIncomplete();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());


		}

	}
}

