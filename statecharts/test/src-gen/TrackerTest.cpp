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

	class TrackerTest : public ::testing::Test
	{
	public:
		void idle();
		void receivedFirst();
		void receivedFurther();
		void delayValid();
		void occupyState();
		void freeState();
		void cascade();

	protected:
		mrw::statechart::TrackerStatechart * statechart;


	public:
		class ClearMock
		{
			typedef void (ClearMock::*functiontype)();
		public:
			TrackerTest * owner;
			void (ClearMock::*clearBehaviorDefault)();
			int callCount;

			ClearMock(TrackerTest * owner) :
				owner(owner),
				clearBehaviorDefault(0),
				callCount(0)
			{}


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
		ClearMock * clearMock;

		class FirstMock
		{
			typedef void (FirstMock::*functiontype)();
		public:
			TrackerTest * owner;
			void (FirstMock::*firstBehaviorDefault)();
			int callCount;

			FirstMock(TrackerTest * owner) :
				owner(owner),
				firstBehaviorDefault(0),
				callCount(0)
			{}


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
		FirstMock * firstMock;

		class FreeMock
		{
			typedef void (FreeMock::*functiontype)();
		public:
			TrackerTest * owner;
			void (FreeMock::*freeBehaviorDefault)();
			int callCount;

			FreeMock(TrackerTest * owner) :
				owner(owner),
				freeBehaviorDefault(0),
				callCount(0)
			{}


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
		FreeMock * freeMock;

		class OccupyMock
		{
			typedef void (OccupyMock::*functiontype)();
		public:
			TrackerTest * owner;
			void (OccupyMock::*occupyBehaviorDefault)();
			int callCount;

			OccupyMock(TrackerTest * owner) :
				owner(owner),
				occupyBehaviorDefault(0),
				callCount(0)
			{}


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
		OccupyMock * occupyMock;

		class ValidMock
		{
			typedef bool (ValidMock::*functiontype)();
		public:
			TrackerTest * owner;
			bool (ValidMock::*validBehaviorDefault)();

			ValidMock(TrackerTest * owner) :
				owner(owner),
				validBehaviorDefault(0)
			{}


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
		ValidMock * validMock;

		class LastMock
		{
			typedef bool (LastMock::*functiontype)();
		public:
			TrackerTest * owner;
			bool (LastMock::*lastBehaviorDefault)();

			LastMock(TrackerTest * owner) :
				owner(owner),
				lastBehaviorDefault(0)
			{}


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
		LastMock * lastMock;

		class MockDefault : public mrw::statechart::TrackerStatechart::OperationCallback
		{
		public:
			TrackerTest * owner;
			MockDefault(TrackerTest * owner) : owner(owner) {}
			void first()
			{
				owner->firstMock->first();
				return (owner->firstMock->*(owner->firstMock->getBehavior()))();
			}
			void free()
			{
				owner->freeMock->free();
				return (owner->freeMock->*(owner->freeMock->getBehavior()))();
			}
			void occupy()
			{
				owner->occupyMock->occupy();
				return (owner->occupyMock->*(owner->occupyMock->getBehavior()))();
			}
			bool valid()
			{
				return (owner->validMock->*(owner->validMock->getBehavior()))();
			}
			bool last()
			{
				return (owner->lastMock->*(owner->lastMock->getBehavior()))();
			}
			void clear()
			{
				owner->clearMock->clear();
				return (owner->clearMock->*(owner->clearMock->getBehavior()))();
			}
		};

		//! The timers are managed by a timer service. */
		TimedSctUnitRunner * runner;

		MockDefault * defaultMock;

		virtual void SetUp()
		{
			statechart = new mrw::statechart::TrackerStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			clearMock = new ClearMock(this);
			clearMock->initializeBehavior();
			firstMock = new FirstMock(this);
			firstMock->initializeBehavior();
			freeMock = new FreeMock(this);
			freeMock->initializeBehavior();
			occupyMock = new OccupyMock(this);
			occupyMock->initializeBehavior();
			validMock = new ValidMock(this);
			validMock->initializeBehavior();
			lastMock = new LastMock(this);
			lastMock->initializeBehavior();
			defaultMock = new MockDefault(this);
			statechart->setOperationCallback(defaultMock);
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
			delete defaultMock;
			defaultMock = 0;
			delete runner;
		}
	};



	void TrackerTest::idle()
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
	void TrackerTest::receivedFirst()
	{
		idle();

		statechart->raiseReceived();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Preparing));

	}
	TEST_F(TrackerTest, receivedFirst)
	{
		receivedFirst();
	}
	void TrackerTest::receivedFurther()
	{
		receivedFirst();

		statechart->raiseReceived();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::TrackerStatechart::State::main_region_Preparing));

	}
	TEST_F(TrackerTest, receivedFurther)
	{
		receivedFurther();
	}
	void TrackerTest::delayValid()
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
	void TrackerTest::occupyState()
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
	void TrackerTest::freeState()
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
	void TrackerTest::cascade()
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

