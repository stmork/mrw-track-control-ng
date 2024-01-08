/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */
#include <string>
#include <list>
#include <algorithm>
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
		void unlocked();
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

		class DisableSectionsMock
		{
			typedef void (DisableSectionsMock::*functiontype)();
		public:
			void (DisableSectionsMock::*disableSectionsBehaviorDefault)();
			int callCount;

			void disableSections1()
			{
			}

			void disableSectionsDefault()
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

			void disableSections()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return disableSectionsBehaviorDefault;
			}

			void setDefaultBehavior(void (DisableSectionsMock::*defaultBehavior)())
			{
				disableSectionsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DisableSectionsMock::disableSectionsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static DisableSectionsMock * disableSectionsMock;

		class DisableSignalsMock
		{
			typedef void (DisableSignalsMock::*functiontype)();
		public:
			void (DisableSignalsMock::*disableSignalsBehaviorDefault)();
			int callCount;

			void disableSignals1()
			{
			}

			void disableSignalsDefault()
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

			void disableSignals()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return disableSignalsBehaviorDefault;
			}

			void setDefaultBehavior(void (DisableSignalsMock::*defaultBehavior)())
			{
				disableSignalsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DisableSignalsMock::disableSignalsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static DisableSignalsMock * disableSignalsMock;

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

		class UnlockRailPartsMock
		{
			typedef void (UnlockRailPartsMock::*functiontype)();
		public:
			void (UnlockRailPartsMock::*unlockRailPartsBehaviorDefault)();
			int callCount;

			void unlockRailParts1()
			{
			}

			void unlockRailPartsDefault()
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

			void unlockRailParts()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return unlockRailPartsBehaviorDefault;
			}

			void setDefaultBehavior(void (UnlockRailPartsMock::*defaultBehavior)())
			{
				unlockRailPartsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UnlockRailPartsMock::unlockRailPartsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static UnlockRailPartsMock * unlockRailPartsMock;

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

		class PrepareRouteMock
		{
			typedef void (PrepareRouteMock::*functiontype)();
		public:
			void (PrepareRouteMock::*prepareRouteBehaviorDefault)();
			int callCount;

			void prepareRoute1()
			{
			}

			void prepareRouteDefault()
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

			void prepareRoute()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return prepareRouteBehaviorDefault;
			}

			void setDefaultBehavior(void (PrepareRouteMock::*defaultBehavior)())
			{
				prepareRouteBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&PrepareRouteMock::prepareRouteDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static PrepareRouteMock * prepareRouteMock;

		class PrepareFlankMock
		{
			typedef void (PrepareFlankMock::*functiontype)();
		public:
			void (PrepareFlankMock::*prepareFlankBehaviorDefault)();
			int callCount;

			void prepareFlank1()
			{
			}

			void prepareFlankDefault()
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

			void prepareFlank()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return prepareFlankBehaviorDefault;
			}

			void setDefaultBehavior(void (PrepareFlankMock::*defaultBehavior)())
			{
				prepareFlankBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&PrepareFlankMock::prepareFlankDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static PrepareFlankMock * prepareFlankMock;

		class FailMock
		{
			typedef void (FailMock::*functiontype)();
		public:
			void (FailMock::*failBehaviorDefault)();
			int callCount;

			void fail1()
			{
			}

			void failDefault()
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

			void fail()
			{
				++callCount;
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
				callCount = 0;
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
				return (true);
			}

			bool isTour2()
			{
				return (false);
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

		class EnableSignalsMock
		{
			typedef void (EnableSignalsMock::*functiontype)();
		public:
			void (EnableSignalsMock::*enableSignalsBehaviorDefault)();
			int callCount;

			void enableSignals1()
			{
			}

			void enableSignalsDefault()
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

			void enableSignals()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return enableSignalsBehaviorDefault;
			}

			void setDefaultBehavior(void (EnableSignalsMock::*defaultBehavior)())
			{
				enableSignalsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&EnableSignalsMock::enableSignalsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static EnableSignalsMock * enableSignalsMock;

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

		class EnableSectionsMock
		{
			typedef void (EnableSectionsMock::*functiontype)();
		public:
			void (EnableSectionsMock::*enableSectionsBehaviorDefault)();
			int callCount;

			void enableSections1()
			{
			}

			void enableSectionsDefault()
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

			void enableSections()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return enableSectionsBehaviorDefault;
			}

			void setDefaultBehavior(void (EnableSectionsMock::*defaultBehavior)())
			{
				enableSectionsBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&EnableSectionsMock::enableSectionsDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static EnableSectionsMock * enableSectionsMock;

		class UnlockFlanksMock
		{
			typedef void (UnlockFlanksMock::*functiontype)();
		public:
			void (UnlockFlanksMock::*unlockFlanksBehaviorDefault)();
			int callCount;

			void unlockFlanks1()
			{
			}

			void unlockFlanksDefault()
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

			void unlockFlanks()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return unlockFlanksBehaviorDefault;
			}

			void setDefaultBehavior(void (UnlockFlanksMock::*defaultBehavior)())
			{
				unlockFlanksBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UnlockFlanksMock::unlockFlanksDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static UnlockFlanksMock * unlockFlanksMock;

		class MockDefault : public mrw::statechart::RouteStatechart::OperationCallback
		{
		public:
			void prepareRoute()
			{
				prepareRouteMock->prepareRoute();
				return (prepareRouteMock->*(prepareRouteMock->getBehavior()))();
			}
			void prepareFlank()
			{
				prepareFlankMock->prepareFlank();
				return (prepareFlankMock->*(prepareFlankMock->getBehavior()))();
			}
			bool isTour()
			{
				return (isTourMock->*(isTourMock->getBehavior()))();
			}
			bool isCompleted()
			{
				return (isCompletedMock->*(isCompletedMock->getBehavior()))();
			}
			void resetTransaction()
			{
				resetTransactionMock->resetTransaction();
				return (resetTransactionMock->*(resetTransactionMock->getBehavior()))();
			}
			void fail()
			{
				failMock->fail();
				return (failMock->*(failMock->getBehavior()))();
			}
			void tryComplete()
			{
				tryCompleteMock->tryComplete();
				return (tryCompleteMock->*(tryCompleteMock->getBehavior()))();
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
			void enableSignals()
			{
				enableSignalsMock->enableSignals();
				return (enableSignalsMock->*(enableSignalsMock->getBehavior()))();
			}
			void extendSignals()
			{
				extendSignalsMock->extendSignals();
				return (extendSignalsMock->*(extendSignalsMock->getBehavior()))();
			}
			void enableSections()
			{
				enableSectionsMock->enableSections();
				return (enableSectionsMock->*(enableSectionsMock->getBehavior()))();
			}
			void disableSections()
			{
				disableSectionsMock->disableSections();
				return (disableSectionsMock->*(disableSectionsMock->getBehavior()))();
			}
			void disableSignals()
			{
				disableSignalsMock->disableSignals();
				return (disableSignalsMock->*(disableSignalsMock->getBehavior()))();
			}
			void unlockFlanks()
			{
				unlockFlanksMock->unlockFlanks();
				return (unlockFlanksMock->*(unlockFlanksMock->getBehavior()))();
			}
			void unlockRailParts()
			{
				unlockRailPartsMock->unlockRailParts();
				return (unlockRailPartsMock->*(unlockRailPartsMock->getBehavior()))();
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
			MockDefault defaultMock;
			virtual void SetUp()
			{
				statechart = new mrw::statechart::RouteStatechart();
				size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
				runner = new TimedSctUnitRunner(
					maximalParallelTimeEvents
				);
				statechart->setTimerService(runner);
				resetTransactionMock = new ResetTransactionMock();
				resetTransactionMock->initializeBehavior();
				disableSectionsMock = new DisableSectionsMock();
				disableSectionsMock->initializeBehavior();
				disableSignalsMock = new DisableSignalsMock();
				disableSignalsMock->initializeBehavior();
				tryCompleteMock = new TryCompleteMock();
				tryCompleteMock->initializeBehavior();
				unlockRailPartsMock = new UnlockRailPartsMock();
				unlockRailPartsMock->initializeBehavior();
				unlockSectionsMock = new UnlockSectionsMock();
				unlockSectionsMock->initializeBehavior();
				prepareRouteMock = new PrepareRouteMock();
				prepareRouteMock->initializeBehavior();
				prepareFlankMock = new PrepareFlankMock();
				prepareFlankMock->initializeBehavior();
				failMock = new FailMock();
				failMock->initializeBehavior();
				isTourMock = new IsTourMock();
				isTourMock->initializeBehavior();
				isCompletedMock = new IsCompletedMock();
				isCompletedMock->initializeBehavior();
				turnSwitchesMock = new TurnSwitchesMock();
				turnSwitchesMock->initializeBehavior();
				turnFlanksMock = new TurnFlanksMock();
				turnFlanksMock->initializeBehavior();
				enableSignalsMock = new EnableSignalsMock();
				enableSignalsMock->initializeBehavior();
				extendSignalsMock = new ExtendSignalsMock();
				extendSignalsMock->initializeBehavior();
				enableSectionsMock = new EnableSectionsMock();
				enableSectionsMock->initializeBehavior();
				unlockFlanksMock = new UnlockFlanksMock();
				unlockFlanksMock->initializeBehavior();
				statechart->setOperationCallback(&defaultMock);
			}
			virtual void TearDown()
			{
				delete unlockFlanksMock;
				delete enableSectionsMock;
				delete extendSignalsMock;
				delete enableSignalsMock;
				delete turnFlanksMock;
				delete turnSwitchesMock;
				delete isCompletedMock;
				delete isTourMock;
				delete failMock;
				delete prepareFlankMock;
				delete prepareRouteMock;
				delete unlockSectionsMock;
				delete unlockRailPartsMock;
				delete tryCompleteMock;
				delete disableSignalsMock;
				delete disableSectionsMock;
				delete resetTransactionMock;
				delete statechart;
				delete runner;
			}
		};

		void disabled()
		{
			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Disable));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		void unlocked()
		{
			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Unlock));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockRailPartsMock->calledAtLeastOnce());

			EXPECT_TRUE(unlockSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}

		void start()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Start));






			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);











		}
		TEST_F(RouteTest, start)
		{
			start();
		}
		void turningSwitches()
		{
			start();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

			statechart->raiseTurn();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning));

			EXPECT_TRUE(prepareRouteMock->calledAtLeast(1));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, turningSwitches)
		{
			turningSwitches();
		}
		TEST_F(RouteTest, turningSwitchesShunt)
		{
			start();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

			statechart->raiseTurn();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning));

			EXPECT_TRUE(prepareRouteMock->calledAtLeast(1));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		void turningFlanks()
		{
			turningSwitches();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Flank_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(prepareFlankMock->calledAtLeastOnce());

			EXPECT_TRUE(turnFlanksMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, turningFlanks)
		{
			turningFlanks();
		}
		TEST_F(RouteTest, turningSignalsTour)
		{
			turningFlanks();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning));

			EXPECT_TRUE(unlockFlanksMock->calledAtLeastOnce());

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(enableSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		void turningSignals()
		{
			turningSwitches();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(enableSignalsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, turningSignals)
		{
			turningSignals();
		}
		void extendingSignals()
		{
			turningSignals();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Signal_Updating));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

			EXPECT_TRUE(extendSignalsMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, extendingSignals)
		{
			extendingSignals();
		}
		void sections()
		{
			extendingSignals();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Section_Activation));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(enableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, sections)
		{
			sections();
		}
		void activate()
		{
			sections();

			statechart->raiseCompleted();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Completed));

			EXPECT_TRUE(statechart->isRaisedActivated());

		}
		TEST_F(RouteTest, activate)
		{
			activate();
		}
		TEST_F(RouteTest, extend)
		{
			activate();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

			statechart->raiseTurn();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, extendShunt)
		{
			activate();

			isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

			statechart->raiseTurn();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active_processing_Switch_Turning));

			EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

			EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());

			EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

		}
		void deactivate()
		{
			activate();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseDisable();

			disabled();

		}
		TEST_F(RouteTest, deactivate)
		{
			deactivate();
		}
		void unlock()
		{
			deactivate();

			statechart->raiseCompleted();

			unlocked();

		}
		TEST_F(RouteTest, unlock)
		{
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
			finish();
		}
		TEST_F(RouteTest, timeout)
		{
			deactivate();

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		TEST_F(RouteTest, failActive)
		{
			activate();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			EXPECT_TRUE(failMock->calledAtLeastOnce());

			disabled();

		}
		TEST_F(RouteTest, failTurningSwitches)
		{
			turningSwitches();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();

		}
		TEST_F(RouteTest, failTurningFlanks)
		{
			turningFlanks();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();

		}
		TEST_F(RouteTest, failTurningSignals)
		{
			turningSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();

		}
		TEST_F(RouteTest, failExtendingSignals)
		{
			extendingSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();

		}
		TEST_F(RouteTest, failTurningSections)
		{
			sections();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

			statechart->raiseFailed();

			disabled();

		}
		void failTurningSwitchesIncomplete()
		{
			turningSwitches();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));

		}
		TEST_F(RouteTest, failTurningSwitchesIncomplete)
		{
			failTurningSwitchesIncomplete();
		}
		void failTurningSignalsIncomplete()
		{
			turningSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));

		}
		TEST_F(RouteTest, failTurningSignalsIncomplete)
		{
			failTurningSignalsIncomplete();
		}
		TEST_F(RouteTest, failExtendingSignalsIncomplete)
		{
			extendingSignals();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));

		}
		void failTurningSectionsIncomplete()
		{
			sections();

			isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));

		}
		TEST_F(RouteTest, failTurningSectionsIncomplete)
		{
			failTurningSectionsIncomplete();
		}
		TEST_F(RouteTest, failWaitSwitches)
		{
			failTurningSwitchesIncomplete();

			statechart->raiseCompleted();

			disabled();

		}
		TEST_F(RouteTest, failWaitSignals)
		{
			failTurningSignalsIncomplete();

			statechart->raiseCompleted();

			disabled();

		}
		TEST_F(RouteTest, failWaitSections)
		{
			failTurningSectionsIncomplete();

			statechart->raiseCompleted();

			disabled();

		}
		TEST_F(RouteTest, timeoutWaitSwitches)
		{
			failTurningSwitchesIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();

		}
		TEST_F(RouteTest, timeoutWaitSignals)
		{
			failTurningSignalsIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();

		}
		TEST_F(RouteTest, timeoutWaitSections)
		{
			failTurningSectionsIncomplete();

			runner->proceed_time(statechart->getEmergency());

			disabled();

		}
		TEST_F(RouteTest, timeoutTurningSwitches)
		{
			turningSwitches();

			runner->proceed_time(statechart->getSwitch_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		TEST_F(RouteTest, timeoutTurningFlanks)
		{
			turningFlanks();

			runner->proceed_time(statechart->getSwitch_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		TEST_F(RouteTest, timeoutTurningSignals)
		{
			turningSignals();

			runner->proceed_time(statechart->getSignal_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		TEST_F(RouteTest, timeoutExtendingSignals)
		{
			extendingSignals();

			runner->proceed_time(statechart->getSignal_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		void emergencySections()
		{
			sections();

			runner->proceed_time(statechart->getSection_timeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

			EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

			EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

		}
		TEST_F(RouteTest, emergencySections)
		{
			emergencySections();
		}
		TEST_F(RouteTest, timeoutTurningSections)
		{
			emergencySections();

			runner->proceed_time(statechart->getEmergency());

			unlocked();

		}
		TEST_F(RouteTest, doExit)
		{
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

