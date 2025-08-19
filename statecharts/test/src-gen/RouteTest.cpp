/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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

namespace
{

	class RouteTest : public ::testing::Test
	{
	public:
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

	protected:
		mrw::statechart::RouteStatechart * statechart;


	public:
		class ResetTransactionMock
		{
			typedef void (ResetTransactionMock::*functiontype)();
		public:
			RouteTest * owner;
			void (ResetTransactionMock::*resetTransactionBehaviorDefault)();
			int callCount;

			ResetTransactionMock(RouteTest * owner) :
				owner(owner),
				resetTransactionBehaviorDefault(0),
				callCount(0)
			{}


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
		ResetTransactionMock * resetTransactionMock;

		class DisableSectionsMock
		{
			typedef void (DisableSectionsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (DisableSectionsMock::*disableSectionsBehaviorDefault)();
			int callCount;

			DisableSectionsMock(RouteTest * owner) :
				owner(owner),
				disableSectionsBehaviorDefault(0),
				callCount(0)
			{}


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
		DisableSectionsMock * disableSectionsMock;

		class DisableSignalsMock
		{
			typedef void (DisableSignalsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (DisableSignalsMock::*disableSignalsBehaviorDefault)();
			int callCount;

			DisableSignalsMock(RouteTest * owner) :
				owner(owner),
				disableSignalsBehaviorDefault(0),
				callCount(0)
			{}


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
		DisableSignalsMock * disableSignalsMock;

		class TryCompleteMock
		{
			typedef void (TryCompleteMock::*functiontype)();
		public:
			RouteTest * owner;
			void (TryCompleteMock::*tryCompleteBehaviorDefault)();
			int callCount;

			TryCompleteMock(RouteTest * owner) :
				owner(owner),
				tryCompleteBehaviorDefault(0),
				callCount(0)
			{}


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
		TryCompleteMock * tryCompleteMock;

		class UnlockRailPartsMock
		{
			typedef void (UnlockRailPartsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (UnlockRailPartsMock::*unlockRailPartsBehaviorDefault)();
			int callCount;

			UnlockRailPartsMock(RouteTest * owner) :
				owner(owner),
				unlockRailPartsBehaviorDefault(0),
				callCount(0)
			{}


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
		UnlockRailPartsMock * unlockRailPartsMock;

		class UnlockSectionsMock
		{
			typedef void (UnlockSectionsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (UnlockSectionsMock::*unlockSectionsBehaviorDefault)();
			int callCount;

			UnlockSectionsMock(RouteTest * owner) :
				owner(owner),
				unlockSectionsBehaviorDefault(0),
				callCount(0)
			{}


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
		UnlockSectionsMock * unlockSectionsMock;

		class PrepareRouteMock
		{
			typedef void (PrepareRouteMock::*functiontype)();
		public:
			RouteTest * owner;
			void (PrepareRouteMock::*prepareRouteBehaviorDefault)();
			int callCount;

			PrepareRouteMock(RouteTest * owner) :
				owner(owner),
				prepareRouteBehaviorDefault(0),
				callCount(0)
			{}


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
		PrepareRouteMock * prepareRouteMock;

		class PrepareFlankMock
		{
			typedef void (PrepareFlankMock::*functiontype)();
		public:
			RouteTest * owner;
			void (PrepareFlankMock::*prepareFlankBehaviorDefault)();
			int callCount;

			PrepareFlankMock(RouteTest * owner) :
				owner(owner),
				prepareFlankBehaviorDefault(0),
				callCount(0)
			{}


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
		PrepareFlankMock * prepareFlankMock;

		class FailMock
		{
			typedef void (FailMock::*functiontype)();
		public:
			RouteTest * owner;
			void (FailMock::*failBehaviorDefault)();
			int callCount;

			FailMock(RouteTest * owner) :
				owner(owner),
				failBehaviorDefault(0),
				callCount(0)
			{}


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
		FailMock * failMock;

		class IsTourMock
		{
			typedef bool (IsTourMock::*functiontype)();
		public:
			RouteTest * owner;
			bool (IsTourMock::*isTourBehaviorDefault)();

			IsTourMock(RouteTest * owner) :
				owner(owner),
				isTourBehaviorDefault(0)
			{}


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
		IsTourMock * isTourMock;

		class IsCompletedMock
		{
			typedef bool (IsCompletedMock::*functiontype)();
		public:
			RouteTest * owner;
			bool (IsCompletedMock::*isCompletedBehaviorDefault)();

			IsCompletedMock(RouteTest * owner) :
				owner(owner),
				isCompletedBehaviorDefault(0)
			{}


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
		IsCompletedMock * isCompletedMock;

		class TurnSwitchesMock
		{
			typedef void (TurnSwitchesMock::*functiontype)();
		public:
			RouteTest * owner;
			void (TurnSwitchesMock::*turnSwitchesBehaviorDefault)();
			int callCount;

			TurnSwitchesMock(RouteTest * owner) :
				owner(owner),
				turnSwitchesBehaviorDefault(0),
				callCount(0)
			{}


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
		TurnSwitchesMock * turnSwitchesMock;

		class TurnFlanksMock
		{
			typedef void (TurnFlanksMock::*functiontype)();
		public:
			RouteTest * owner;
			void (TurnFlanksMock::*turnFlanksBehaviorDefault)();
			int callCount;

			TurnFlanksMock(RouteTest * owner) :
				owner(owner),
				turnFlanksBehaviorDefault(0),
				callCount(0)
			{}


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
		TurnFlanksMock * turnFlanksMock;

		class EnableSignalsMock
		{
			typedef void (EnableSignalsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (EnableSignalsMock::*enableSignalsBehaviorDefault)();
			int callCount;

			EnableSignalsMock(RouteTest * owner) :
				owner(owner),
				enableSignalsBehaviorDefault(0),
				callCount(0)
			{}


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
		EnableSignalsMock * enableSignalsMock;

		class ExtendSignalsMock
		{
			typedef void (ExtendSignalsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (ExtendSignalsMock::*extendSignalsBehaviorDefault)();
			int callCount;

			ExtendSignalsMock(RouteTest * owner) :
				owner(owner),
				extendSignalsBehaviorDefault(0),
				callCount(0)
			{}


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
		ExtendSignalsMock * extendSignalsMock;

		class EnableSectionsMock
		{
			typedef void (EnableSectionsMock::*functiontype)();
		public:
			RouteTest * owner;
			void (EnableSectionsMock::*enableSectionsBehaviorDefault)();
			int callCount;

			EnableSectionsMock(RouteTest * owner) :
				owner(owner),
				enableSectionsBehaviorDefault(0),
				callCount(0)
			{}


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
		EnableSectionsMock * enableSectionsMock;

		class UnlockFlanksMock
		{
			typedef void (UnlockFlanksMock::*functiontype)();
		public:
			RouteTest * owner;
			void (UnlockFlanksMock::*unlockFlanksBehaviorDefault)();
			int callCount;

			UnlockFlanksMock(RouteTest * owner) :
				owner(owner),
				unlockFlanksBehaviorDefault(0),
				callCount(0)
			{}


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
		UnlockFlanksMock * unlockFlanksMock;

		class MockDefault : public mrw::statechart::RouteStatechart::OperationCallback
		{
		public:
			RouteTest * owner;
			MockDefault(RouteTest * owner) : owner(owner) {}
			void prepareRoute()
			{
				owner->prepareRouteMock->prepareRoute();
				return (owner->prepareRouteMock->*(owner->prepareRouteMock->getBehavior()))();
			}
			void prepareFlank()
			{
				owner->prepareFlankMock->prepareFlank();
				return (owner->prepareFlankMock->*(owner->prepareFlankMock->getBehavior()))();
			}
			bool isTour()
			{
				return (owner->isTourMock->*(owner->isTourMock->getBehavior()))();
			}
			bool isCompleted()
			{
				return (owner->isCompletedMock->*(owner->isCompletedMock->getBehavior()))();
			}
			void resetTransaction()
			{
				owner->resetTransactionMock->resetTransaction();
				return (owner->resetTransactionMock->*(owner->resetTransactionMock->getBehavior()))();
			}
			void fail()
			{
				owner->failMock->fail();
				return (owner->failMock->*(owner->failMock->getBehavior()))();
			}
			void tryComplete()
			{
				owner->tryCompleteMock->tryComplete();
				return (owner->tryCompleteMock->*(owner->tryCompleteMock->getBehavior()))();
			}
			void turnSwitches()
			{
				owner->turnSwitchesMock->turnSwitches();
				return (owner->turnSwitchesMock->*(owner->turnSwitchesMock->getBehavior()))();
			}
			void turnFlanks()
			{
				owner->turnFlanksMock->turnFlanks();
				return (owner->turnFlanksMock->*(owner->turnFlanksMock->getBehavior()))();
			}
			void enableSignals()
			{
				owner->enableSignalsMock->enableSignals();
				return (owner->enableSignalsMock->*(owner->enableSignalsMock->getBehavior()))();
			}
			void extendSignals()
			{
				owner->extendSignalsMock->extendSignals();
				return (owner->extendSignalsMock->*(owner->extendSignalsMock->getBehavior()))();
			}
			void enableSections()
			{
				owner->enableSectionsMock->enableSections();
				return (owner->enableSectionsMock->*(owner->enableSectionsMock->getBehavior()))();
			}
			void disableSections()
			{
				owner->disableSectionsMock->disableSections();
				return (owner->disableSectionsMock->*(owner->disableSectionsMock->getBehavior()))();
			}
			void disableSignals()
			{
				owner->disableSignalsMock->disableSignals();
				return (owner->disableSignalsMock->*(owner->disableSignalsMock->getBehavior()))();
			}
			void unlockFlanks()
			{
				owner->unlockFlanksMock->unlockFlanks();
				return (owner->unlockFlanksMock->*(owner->unlockFlanksMock->getBehavior()))();
			}
			void unlockRailParts()
			{
				owner->unlockRailPartsMock->unlockRailParts();
				return (owner->unlockRailPartsMock->*(owner->unlockRailPartsMock->getBehavior()))();
			}
			void unlockSections()
			{
				owner->unlockSectionsMock->unlockSections();
				return (owner->unlockSectionsMock->*(owner->unlockSectionsMock->getBehavior()))();
			}
		};

		//! The timers are managed by a timer service. */
		TimedSctUnitRunner * runner;

		MockDefault * defaultMock;

		virtual void SetUp()
		{
			statechart = new mrw::statechart::RouteStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			resetTransactionMock = new ResetTransactionMock(this);
			resetTransactionMock->initializeBehavior();
			disableSectionsMock = new DisableSectionsMock(this);
			disableSectionsMock->initializeBehavior();
			disableSignalsMock = new DisableSignalsMock(this);
			disableSignalsMock->initializeBehavior();
			tryCompleteMock = new TryCompleteMock(this);
			tryCompleteMock->initializeBehavior();
			unlockRailPartsMock = new UnlockRailPartsMock(this);
			unlockRailPartsMock->initializeBehavior();
			unlockSectionsMock = new UnlockSectionsMock(this);
			unlockSectionsMock->initializeBehavior();
			prepareRouteMock = new PrepareRouteMock(this);
			prepareRouteMock->initializeBehavior();
			prepareFlankMock = new PrepareFlankMock(this);
			prepareFlankMock->initializeBehavior();
			failMock = new FailMock(this);
			failMock->initializeBehavior();
			isTourMock = new IsTourMock(this);
			isTourMock->initializeBehavior();
			isCompletedMock = new IsCompletedMock(this);
			isCompletedMock->initializeBehavior();
			turnSwitchesMock = new TurnSwitchesMock(this);
			turnSwitchesMock->initializeBehavior();
			turnFlanksMock = new TurnFlanksMock(this);
			turnFlanksMock->initializeBehavior();
			enableSignalsMock = new EnableSignalsMock(this);
			enableSignalsMock->initializeBehavior();
			extendSignalsMock = new ExtendSignalsMock(this);
			extendSignalsMock->initializeBehavior();
			enableSectionsMock = new EnableSectionsMock(this);
			enableSectionsMock->initializeBehavior();
			unlockFlanksMock = new UnlockFlanksMock(this);
			unlockFlanksMock->initializeBehavior();
			defaultMock = new MockDefault(this);
			statechart->setOperationCallback(defaultMock);
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
			delete defaultMock;
			defaultMock = 0;
			delete runner;
		}
	};


	void RouteTest::disabled()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Disable));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(disableSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(disableSignalsMock->calledAtLeastOnce());

		EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

	}
	void RouteTest::unlocked()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Unlock));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockRailPartsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(tryCompleteMock->calledAtLeastOnce());

	}

	void RouteTest::start()
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
	void RouteTest::turningSwitches()
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
	void RouteTest::turningFlanks()
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
	void RouteTest::turningSignals()
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
	void RouteTest::extendingSignals()
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
	void RouteTest::sections()
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
	void RouteTest::activate()
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
	void RouteTest::deactivate()
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
	void RouteTest::unlock()
	{
		deactivate();

		statechart->raiseCompleted();

		unlocked();

	}
	TEST_F(RouteTest, unlock)
	{
		unlock();
	}
	void RouteTest::finish()
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
	void RouteTest::failTurningSwitchesIncomplete()
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
	void RouteTest::failTurningSignalsIncomplete()
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
	void RouteTest::failTurningSectionsIncomplete()
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
	void RouteTest::emergencySections()
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

