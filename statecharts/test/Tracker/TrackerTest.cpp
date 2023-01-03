/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
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

		void clear()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static ClearMock * clearMock;

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

	class FirstMock
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

		void first()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FirstMock * firstMock;

	class OccupyMock
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

		void occupy()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static OccupyMock * occupyMock;

	class MockDefault : public mrw::statechart::TrackerStatechart::OperationCallback
	{
	public:
		void first()
		{
			firstMock->first();
		}
		void free()
		{
		}
		void occupy()
		{
			occupyMock->occupy();
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
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class TrackerTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::TrackerStatechart();
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


	void idle()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());


		clearMock->reset();
	}
	TEST_F(TrackerTest, idle)
	{
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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


		validMock->reset();
		lastMock->reset();
		firstMock->reset();
	}
	TEST_F(TrackerTest, delayValid)
	{
		validMock = new ValidMock();
		validMock->initializeBehavior();
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		firstMock = new FirstMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		delayValid();
	}
	TEST_F(TrackerTest, delayInvalid)
	{
		validMock = new ValidMock();
		validMock->initializeBehavior();
		clearMock = new ClearMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		receivedFurther();

		validMock->setDefaultBehavior(&ValidMock::valid2);

		runner->proceed_time(statechart->getStart());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());


		validMock->reset();
		clearMock->reset();
	}
	void occupyState()
	{
		delayValid();

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Occupy));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());


		occupyMock->reset();
	}
	TEST_F(TrackerTest, occupyState)
	{
		occupyMock = new OccupyMock();
		validMock = new ValidMock();
		validMock->initializeBehavior();
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		firstMock = new FirstMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		occupyState();
	}
	void freeState()
	{
		occupyState();

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Driving_Tracking_Free));

		EXPECT_TRUE(occupyMock->calledAtLeastOnce());


		occupyMock->reset();
	}
	TEST_F(TrackerTest, freeState)
	{
		occupyMock = new OccupyMock();
		occupyMock = new OccupyMock();
		validMock = new ValidMock();
		validMock->initializeBehavior();
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		firstMock = new FirstMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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


		occupyMock->reset();
	}
	TEST_F(TrackerTest, cascade)
	{
		occupyMock = new OccupyMock();
		occupyMock = new OccupyMock();
		occupyMock = new OccupyMock();
		validMock = new ValidMock();
		validMock->initializeBehavior();
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		firstMock = new FirstMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		cascade();
	}
	TEST_F(TrackerTest, completed)
	{
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		clearMock = new ClearMock();
		occupyMock = new OccupyMock();
		occupyMock = new OccupyMock();
		occupyMock = new OccupyMock();
		validMock = new ValidMock();
		validMock->initializeBehavior();
		lastMock = new LastMock();
		lastMock->initializeBehavior();
		firstMock = new FirstMock();
		clearMock = new ClearMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		cascade();

		lastMock->setDefaultBehavior(&LastMock::last2);

		runner->proceed_time(statechart->getStep());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Idle));

		EXPECT_TRUE(clearMock->calledAtLeastOnce());


		lastMock->reset();
		clearMock->reset();
	}

}

