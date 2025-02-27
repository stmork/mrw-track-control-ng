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
#include "TrackerStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void idle();
	void receivedFirst();
	void receivedFurther();
	void delayValid();
	void occupyState();
	void freeState();
	void cascade();
	mrw::statechart::TrackerStatechart * statechart;


	class ClearMock
	{
		typedef void (ClearMock::*functiontype)();
	public:
		void (ClearMock::*clearBehaviorDefault)();
		int callCount;

		void clear1()
		{
		}

		void clearDefault()
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

		void clear()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return clearBehaviorDefault;
		}

		void setDefaultBehavior(void (ClearMock::*defaultBehavior)())
		{
			clearBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&ClearMock::clearDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static ClearMock * clearMock;

	class FirstMock
	{
		typedef void (FirstMock::*functiontype)();
	public:
		void (FirstMock::*firstBehaviorDefault)();
		int callCount;

		void first1()
		{
		}

		void firstDefault()
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

		void first()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return firstBehaviorDefault;
		}

		void setDefaultBehavior(void (FirstMock::*defaultBehavior)())
		{
			firstBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&FirstMock::firstDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static FirstMock * firstMock;

	class FreeMock
	{
		typedef void (FreeMock::*functiontype)();
	public:
		void (FreeMock::*freeBehaviorDefault)();
		int callCount;

		void free1()
		{
		}

		void freeDefault()
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

		void free()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return freeBehaviorDefault;
		}

		void setDefaultBehavior(void (FreeMock::*defaultBehavior)())
		{
			freeBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&FreeMock::freeDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static FreeMock * freeMock;

	class OccupyMock
	{
		typedef void (OccupyMock::*functiontype)();
	public:
		void (OccupyMock::*occupyBehaviorDefault)();
		int callCount;

		void occupy1()
		{
		}

		void occupyDefault()
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

		void occupy()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return occupyBehaviorDefault;
		}

		void setDefaultBehavior(void (OccupyMock::*defaultBehavior)())
		{
			occupyBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&OccupyMock::occupyDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static OccupyMock * occupyMock;

	class ValidMock
	{
		typedef bool (ValidMock::*functiontype)();
	public:
		bool (ValidMock::*validBehaviorDefault)();

		bool valid1()
		{
			return (true);
		}

		bool valid2()
		{
			return (false);
		}

		bool validDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return validBehaviorDefault;
		}

		void setDefaultBehavior(bool (ValidMock::*defaultBehavior)())
		{
			validBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&ValidMock::validDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static ValidMock * validMock;

	class LastMock
	{
		typedef bool (LastMock::*functiontype)();
	public:
		bool (LastMock::*lastBehaviorDefault)();

		bool last1()
		{
			return (false);
		}

		bool last2()
		{
			return (true);
		}

		bool lastDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return lastBehaviorDefault;
		}

		void setDefaultBehavior(bool (LastMock::*defaultBehavior)())
		{
			lastBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&LastMock::lastDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static LastMock * lastMock;

	class MockDefault : public mrw::statechart::TrackerStatechart::OperationCallback
	{
	public:
		void first()
		{
			firstMock->first();
			return (firstMock->*(firstMock->getBehavior()))();
		}
		void free()
		{
			freeMock->free();
			return (freeMock->*(freeMock->getBehavior()))();
		}
		void occupy()
		{
			occupyMock->occupy();
			return (occupyMock->*(occupyMock->getBehavior()))();
		}
		bool valid()
		{
			return (validMock->*(validMock->getBehavior()))();
		}
		bool last()
		{
			return (lastMock->*(lastMock->getBehavior()))();
		}
		void clear()
		{
			clearMock->clear();
			return (clearMock->*(clearMock->getBehavior()))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class TrackerTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::TrackerStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			clearMock = new ClearMock();
			clearMock->initializeBehavior();
			firstMock = new FirstMock();
			firstMock->initializeBehavior();
			freeMock = new FreeMock();
			freeMock->initializeBehavior();
			occupyMock = new OccupyMock();
			occupyMock->initializeBehavior();
			validMock = new ValidMock();
			validMock->initializeBehavior();
			lastMock = new LastMock();
			lastMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete lastMock;
			delete validMock;
			delete occupyMock;
			delete freeMock;
			delete firstMock;
			delete clearMock;
			delete statechart;
			delete runner;
		}
	};


	void idle()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());





	}
	TEST_F(TrackerTest, idle)
	{
		idle();
	}
	void receivedFirst()
	{
		idle();

		statechart->raiseReceived();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Preparing));

	}
	TEST_F(TrackerTest, receivedFirst)
	{
		receivedFirst();
	}
	void receivedFurther()
	{
		receivedFirst();

		statechart->raiseReceived();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Preparing));

	}
	TEST_F(TrackerTest, receivedFurther)
	{
		receivedFurther();
	}
	void delayValid()
	{
		receivedFurther();

		validMock->setDefaultBehavior(&ValidMock::valid1);

		lastMock->setDefaultBehavior(&LastMock::last1);

		runner->proceed_time(statechart->getStart());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_First));

		EXPECT_TRUE(firstMock->calledAtLeastOnce());

	}
	TEST_F(TrackerTest, delayValid)
	{
		delayValid();
	}
	TEST_F(TrackerTest, delayInvalid)
	{
		receivedFurther();

		validMock->setDefaultBehavior(&ValidMock::valid2);

		runner->proceed_time(statechart->getStart());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());

	}
	void occupyState()
	{
		delayValid();

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());

	}
	TEST_F(TrackerTest, occupyState)
	{
		occupyState();
	}
	void freeState()
	{
		occupyState();

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free));

		EXPECT_TRUE(freeMock->calledAtLeastOnce());

	}
	TEST_F(TrackerTest, freeState)
	{
		freeState();
	}
	void cascade()
	{
		freeState();

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());

	}
	TEST_F(TrackerTest, cascade)
	{
		cascade();
	}
	TEST_F(TrackerTest, completed)
	{
		cascade();

		lastMock->setDefaultBehavior(&LastMock::last2);

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());

	}
	TEST_F(TrackerTest, doExit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		idle();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		receivedFirst();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		delayValid();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		occupyState();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		freeState();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

