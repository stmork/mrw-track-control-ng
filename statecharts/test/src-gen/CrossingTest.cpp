/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//
* */
#include <string>
#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "CrossingStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#ifndef SC_UNUSED
#define SC_UNUSED(P) (void)P
#endif

namespace
{

	class CrossingTest : public ::testing::Test
	{
	public:
		void failState();
		void initing();
		void waitForStart();
		void initial();
		void failAfterStart();
		void operational();
		void closeUsed();
		void lockUsed();
		void openNotUsed();

	protected:
		mrw::statechart::CrossingStatechart * statechart;


	public:
		class FailMock
		{
			typedef void (FailMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (FailMock::*failBehaviorDefault)();
			int callCount;

			FailMock(CrossingTest * owner) :
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

		class IncMock
		{
			typedef void (IncMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (IncMock::*incBehaviorDefault)();
			int callCount;

			IncMock(CrossingTest * owner) :
				owner(owner),
				incBehaviorDefault(0),
				callCount(0)
			{}


			void inc1()
			{
			}

			void incDefault()
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

			void inc()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return incBehaviorDefault;
			}

			void setDefaultBehavior(void (IncMock::*defaultBehavior)())
			{
				incBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&IncMock::incDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		IncMock * incMock;

		class DecMock
		{
			typedef void (DecMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (DecMock::*decBehaviorDefault)();
			int callCount;

			DecMock(CrossingTest * owner) :
				owner(owner),
				decBehaviorDefault(0),
				callCount(0)
			{}


			void dec1()
			{
			}

			void decDefault()
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

			void dec()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return decBehaviorDefault;
			}

			void setDefaultBehavior(void (DecMock::*defaultBehavior)())
			{
				decBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DecMock::decDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		DecMock * decMock;

		class PendingMock
		{
			typedef void (PendingMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (PendingMock::*pendingBehaviorDefault)();
			int callCount;

			PendingMock(CrossingTest * owner) :
				owner(owner),
				pendingBehaviorDefault(0),
				callCount(0)
			{}


			void pending1()
			{
			}

			void pendingDefault()
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

			void pending()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return pendingBehaviorDefault;
			}

			void setDefaultBehavior(void (PendingMock::*defaultBehavior)())
			{
				pendingBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&PendingMock::pendingDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		PendingMock * pendingMock;

		class OpenMock
		{
			typedef void (OpenMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (OpenMock::*openBehaviorDefault)();
			int callCount;

			OpenMock(CrossingTest * owner) :
				owner(owner),
				openBehaviorDefault(0),
				callCount(0)
			{}


			void open1()
			{
			}

			void openDefault()
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

			void open()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return openBehaviorDefault;
			}

			void setDefaultBehavior(void (OpenMock::*defaultBehavior)())
			{
				openBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&OpenMock::openDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		OpenMock * openMock;

		class CloseMock
		{
			typedef void (CloseMock::*functiontype)();
		public:
			CrossingTest * owner;
			void (CloseMock::*closeBehaviorDefault)();
			int callCount;

			CloseMock(CrossingTest * owner) :
				owner(owner),
				closeBehaviorDefault(0),
				callCount(0)
			{}


			void close1()
			{
			}

			void closeDefault()
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

			void close()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return closeBehaviorDefault;
			}

			void setDefaultBehavior(void (CloseMock::*defaultBehavior)())
			{
				closeBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&CloseMock::closeDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		CloseMock * closeMock;

		class LockMock
		{
			struct parameters
			{
				bool do_it;
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->do_it == other.do_it);
				}
			};
		public:
			CrossingTest * owner;
			std::list<LockMock::parameters> paramCount;
			int callCount;

			LockMock(CrossingTest * owner) :
				owner(owner),
				callCount(0)
			{}


			bool calledAtLeast(const int times)
			{
				return (callCount >= times);
			}

			bool calledAtLeastOnce()
			{
				return (callCount > 0);
			}

			bool calledAtLeast(const int times, const bool do_it)
			{
				parameters p;
				p.do_it = do_it;

				std::list<LockMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const bool do_it)
			{
				parameters p;
				p.do_it = do_it;

				std::list<LockMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void lock(const bool do_it)
			{
				++callCount;

				parameters p;
				p.do_it = do_it;

				std::list<LockMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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
			void reset()
			{
				callCount = 0;
				paramCount.clear();
			}
		};
		LockMock * lockMock;

		class UsedMock
		{
			typedef bool (UsedMock::*functiontype)();
		public:
			CrossingTest * owner;
			bool (UsedMock::*usedBehaviorDefault)();

			UsedMock(CrossingTest * owner) :
				owner(owner),
				usedBehaviorDefault(0)
			{}


			bool used1()
			{
				return (false);
			}

			bool used2()
			{
				return (true);
			}

			bool usedDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return usedBehaviorDefault;
			}

			void setDefaultBehavior(bool (UsedMock::*defaultBehavior)())
			{
				usedBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&UsedMock::usedDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		UsedMock * usedMock;

		class MockDefault : public mrw::statechart::CrossingStatechart::OperationCallback
		{
		public:
			CrossingTest * owner;
			MockDefault(CrossingTest * owner) : owner(owner) {}
			void inc()
			{
				owner->incMock->inc();
				return (owner->incMock->*(owner->incMock->getBehavior()))();
			}
			void dec()
			{
				owner->decMock->dec();
				return (owner->decMock->*(owner->decMock->getBehavior()))();
			}
			bool used()
			{
				return (owner->usedMock->*(owner->usedMock->getBehavior()))();
			}
			void close()
			{
				owner->closeMock->close();
				return (owner->closeMock->*(owner->closeMock->getBehavior()))();
			}
			void open()
			{
				owner->openMock->open();
				return (owner->openMock->*(owner->openMock->getBehavior()))();
			}
			void fail()
			{
				owner->failMock->fail();
				return (owner->failMock->*(owner->failMock->getBehavior()))();
			}
			void pending()
			{
				owner->pendingMock->pending();
				return (owner->pendingMock->*(owner->pendingMock->getBehavior()))();
			}
			void lock(bool do_it)
			{
				owner->lockMock->lock(do_it);
			}
		};

		//! The timers are managed by a timer service. */
		TimedSctUnitRunner * runner;

		MockDefault * defaultMock;

		virtual void SetUp()
		{
			statechart = new mrw::statechart::CrossingStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			failMock = new FailMock(this);
			failMock->initializeBehavior();
			incMock = new IncMock(this);
			incMock->initializeBehavior();
			decMock = new DecMock(this);
			decMock->initializeBehavior();
			pendingMock = new PendingMock(this);
			pendingMock->initializeBehavior();
			openMock = new OpenMock(this);
			openMock->initializeBehavior();
			closeMock = new CloseMock(this);
			closeMock->initializeBehavior();
			lockMock = new LockMock(this);
			usedMock = new UsedMock(this);
			usedMock->initializeBehavior();
			defaultMock = new MockDefault(this);
			statechart->setOperationCallback(defaultMock);
		}
		virtual void TearDown()
		{
			delete usedMock;
			delete lockMock;
			delete closeMock;
			delete openMock;
			delete pendingMock;
			delete decMock;
			delete incMock;
			delete failMock;
			delete statechart;
			delete defaultMock;
			defaultMock = 0;
			delete runner;
		}
	};


	void CrossingTest::failState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

	}
	void CrossingTest::initing()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Init));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(1));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(lockMock->calledAtLeast(0));

	}

	void CrossingTest::waitForStart()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start));

		usedMock->setDefaultBehavior(&UsedMock::used1);







	}
	TEST_F(CrossingTest, waitForStart)
	{
		waitForStart();
	}
	void CrossingTest::initial()
	{
		waitForStart();

		statechart->raiseStart();

		initing();

	}
	TEST_F(CrossingTest, initial)
	{
		initial();
	}
	void CrossingTest::failAfterStart()
	{
		initial();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(CrossingTest, failAfterStart)
	{
		failAfterStart();
	}
	void CrossingTest::operational()
	{
		initial();

		statechart->raiseResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(lockMock->calledAtLeast(1));

	}
	TEST_F(CrossingTest, operational)
	{
		operational();
	}
	TEST_F(CrossingTest, restart)
	{
		operational();

		statechart->raiseStart();

		initing();

	}
	TEST_F(CrossingTest, clearing)
	{
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Wait_For_Start));

		statechart->raiseStart();

		initing();

	}
	TEST_F(CrossingTest, closeNotUsed)
	{
		operational();

		usedMock->setDefaultBehavior(&UsedMock::used1);

		statechart->raiseAction();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

	}
	void CrossingTest::closeUsed()
	{
		operational();

		usedMock->setDefaultBehavior(&UsedMock::used2);

		statechart->raiseAction();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeast(1));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(1));

	}
	TEST_F(CrossingTest, closeUsed)
	{
		closeUsed();
	}
	void CrossingTest::lockUsed()
	{
		closeUsed();

		statechart->raiseResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeast(1));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

	}
	TEST_F(CrossingTest, lockUsed)
	{
		lockUsed();
	}
	void CrossingTest::openNotUsed()
	{
		lockUsed();

		usedMock->setDefaultBehavior(&UsedMock::used1);

		statechart->raiseAction();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeast(1));

		EXPECT_TRUE(openMock->calledAtLeast(1));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

	}
	TEST_F(CrossingTest, openNotUsed)
	{
		openNotUsed();
	}
	TEST_F(CrossingTest, openUsed)
	{
		lockUsed();

		usedMock->setDefaultBehavior(&UsedMock::used2);

		statechart->raiseAction();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(1));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

	}
	TEST_F(CrossingTest, unlock)
	{
		openNotUsed();

		statechart->raiseResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

	}
	TEST_F(CrossingTest, closeTimeout)
	{
		closeUsed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing));

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Failed));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(failMock->calledAtLeast(1));

	}
	TEST_F(CrossingTest, openTimeout)
	{
		openNotUsed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening));

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Failed));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(failMock->calledAtLeast(1));

	}
	TEST_F(CrossingTest, closeFailed)
	{
		closeUsed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Closing));

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Failed));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(failMock->calledAtLeast(1));

	}
	TEST_F(CrossingTest, openFailed)
	{
		openNotUsed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Operating_Processing_Pending_Crossing_processing_Opening));

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::CrossingStatechart::State::main_region_Failed));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(pendingMock->calledAtLeast(0));

		EXPECT_TRUE(openMock->calledAtLeast(0));

		EXPECT_TRUE(closeMock->calledAtLeast(0));

		EXPECT_TRUE(failMock->calledAtLeast(1));

	}

}

