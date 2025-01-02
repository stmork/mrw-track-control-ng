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
#include "SwitchStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void turning();
	void lockedState();
	void failState();
	void waitForStart();
	void initial();
	void failAfterStart();
	void operational();
	void turnLeft();
	void turnRight();
	void queuedLeft();
	void queuedRight();
	void okLeft();
	void okRight();
	mrw::statechart::SwitchStatechart * statechart;


	class IncMock
	{
		typedef void (IncMock::*functiontype)();
	public:
		void (IncMock::*incBehaviorDefault)();
		int callCount;

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
	static IncMock * incMock;

	class PendingMock
	{
		typedef void (PendingMock::*functiontype)();
	public:
		void (PendingMock::*pendingBehaviorDefault)();
		int callCount;

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
	static PendingMock * pendingMock;

	class LockMock
	{
		typedef void (LockMock::*functiontype)();
		struct parameters
		{
			bool do_it;
			void (LockMock::*behavior)();
			int callCount;
			inline bool operator==(const parameters & other)
			{
				return (this->do_it == other.do_it);
			}
		};
	public:
		std::list<LockMock::parameters> mocks;
		std::list<LockMock::parameters> paramCount;
		void (LockMock::*lockBehaviorDefault)();
		int callCount;

		void lock1()
		{
		}

		void lockDefault()
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

		void setLockBehavior(const bool do_it, void (LockMock::*func)())
		{
			parameters p;
			p.do_it = do_it;
			p.behavior = func;

			std::list<LockMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				mocks.erase(i);
			}
			mocks.push_back(p);
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

		functiontype getBehavior(const bool do_it)
		{
			parameters p;
			p.do_it = do_it;

			std::list<LockMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				return  i->behavior;
			}
			else
			{
				return lockBehaviorDefault;
			}
		}

		void setDefaultBehavior(void (LockMock::*defaultBehavior)())
		{
			lockBehaviorDefault = defaultBehavior;
			mocks.clear();
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&LockMock::lockDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
			paramCount.clear();
			mocks.clear();
		}
	};
	static LockMock * lockMock;

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

	class DecMock
	{
		typedef void (DecMock::*functiontype)();
	public:
		void (DecMock::*decBehaviorDefault)();
		int callCount;

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
	static DecMock * decMock;

	class LeftMock
	{
		typedef void (LeftMock::*functiontype)();
	public:
		void (LeftMock::*leftBehaviorDefault)();
		int callCount;

		void left1()
		{
		}

		void leftDefault()
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

		void left()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return leftBehaviorDefault;
		}

		void setDefaultBehavior(void (LeftMock::*defaultBehavior)())
		{
			leftBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&LeftMock::leftDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static LeftMock * leftMock;

	class RightMock
	{
		typedef void (RightMock::*functiontype)();
	public:
		void (RightMock::*rightBehaviorDefault)();
		int callCount;

		void right1()
		{
		}

		void rightDefault()
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

		void right()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return rightBehaviorDefault;
		}

		void setDefaultBehavior(void (RightMock::*defaultBehavior)())
		{
			rightBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&RightMock::rightDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static RightMock * rightMock;

	class RequestMock
	{
		typedef void (RequestMock::*functiontype)();
	public:
		void (RequestMock::*requestBehaviorDefault)();
		int callCount;

		void request1()
		{
		}

		void requestDefault()
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

		void request()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return requestBehaviorDefault;
		}

		void setDefaultBehavior(void (RequestMock::*defaultBehavior)())
		{
			requestBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&RequestMock::requestDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static RequestMock * requestMock;

	class IsFreeMock
	{
		typedef bool (IsFreeMock::*functiontype)();
	public:
		bool (IsFreeMock::*isFreeBehaviorDefault)();

		bool isFree1()
		{
			return (true);
		}

		bool isFree2()
		{
			return (false);
		}

		bool isFreeDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return isFreeBehaviorDefault;
		}

		void setDefaultBehavior(bool (IsFreeMock::*defaultBehavior)())
		{
			isFreeBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&IsFreeMock::isFreeDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static IsFreeMock * isFreeMock;

	class IsReservedMock
	{
		typedef bool (IsReservedMock::*functiontype)();
	public:
		bool (IsReservedMock::*isReservedBehaviorDefault)();

		bool isReserved1()
		{
			return (true);
		}

		bool isReservedDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return isReservedBehaviorDefault;
		}

		void setDefaultBehavior(bool (IsReservedMock::*defaultBehavior)())
		{
			isReservedBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&IsReservedMock::isReservedDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static IsReservedMock * isReservedMock;

	class DoTurnLeftMock
	{
		typedef bool (DoTurnLeftMock::*functiontype)();
	public:
		bool (DoTurnLeftMock::*doTurnLeftBehaviorDefault)();

		bool doTurnLeft1()
		{
			return (true);
		}

		bool doTurnLeft2()
		{
			return (false);
		}

		bool doTurnLeftDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return doTurnLeftBehaviorDefault;
		}

		void setDefaultBehavior(bool (DoTurnLeftMock::*defaultBehavior)())
		{
			doTurnLeftBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&DoTurnLeftMock::doTurnLeftDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static DoTurnLeftMock * doTurnLeftMock;

	class MockDefault : public mrw::statechart::SwitchStatechart::OperationCallback
	{
	public:
		void inc()
		{
			incMock->inc();
			return (incMock->*(incMock->getBehavior()))();
		}
		void dec()
		{
			decMock->dec();
			return (decMock->*(decMock->getBehavior()))();
		}
		void left()
		{
			leftMock->left();
			return (leftMock->*(leftMock->getBehavior()))();
		}
		void right()
		{
			rightMock->right();
			return (rightMock->*(rightMock->getBehavior()))();
		}
		void request()
		{
			requestMock->request();
			return (requestMock->*(requestMock->getBehavior()))();
		}
		bool doTurnLeft()
		{
			return (doTurnLeftMock->*(doTurnLeftMock->getBehavior()))();
		}
		bool isFree()
		{
			return (isFreeMock->*(isFreeMock->getBehavior()))();
		}
		bool isReserved()
		{
			return (isReservedMock->*(isReservedMock->getBehavior()))();
		}
		void fail()
		{
			failMock->fail();
			return (failMock->*(failMock->getBehavior()))();
		}
		void pending()
		{
			pendingMock->pending();
			return (pendingMock->*(pendingMock->getBehavior()))();
		}
		void lock(bool do_it)
		{
			lockMock->lock(do_it);
			return (lockMock->*(lockMock->getBehavior(do_it)))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class SwitchTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SwitchStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			incMock = new IncMock();
			incMock->initializeBehavior();
			pendingMock = new PendingMock();
			pendingMock->initializeBehavior();
			lockMock = new LockMock();
			lockMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			decMock = new DecMock();
			decMock->initializeBehavior();
			leftMock = new LeftMock();
			leftMock->initializeBehavior();
			rightMock = new RightMock();
			rightMock->initializeBehavior();
			requestMock = new RequestMock();
			requestMock->initializeBehavior();
			isFreeMock = new IsFreeMock();
			isFreeMock->initializeBehavior();
			isReservedMock = new IsReservedMock();
			isReservedMock->initializeBehavior();
			doTurnLeftMock = new DoTurnLeftMock();
			doTurnLeftMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete doTurnLeftMock;
			delete isReservedMock;
			delete isFreeMock;
			delete requestMock;
			delete rightMock;
			delete leftMock;
			delete decMock;
			delete failMock;
			delete lockMock;
			delete pendingMock;
			delete incMock;
			delete statechart;
			delete runner;
		}
	};

	void turning()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

	}
	void lockedState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Locked));

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

	}
	void failState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStop());

	}

	void waitForStart()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start));









		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		doTurnLeftMock->setDefaultBehavior(&DoTurnLeftMock::doTurnLeft1);

	}
	TEST_F(SwitchTest, waitForStart)
	{
		waitForStart();
	}
	void initial()
	{
		waitForStart();

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

	}
	TEST_F(SwitchTest, initial)
	{
		initial();
	}
	void failAfterStart()
	{
		initial();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SwitchTest, failAfterStart)
	{
		failAfterStart();
	}
	TEST_F(SwitchTest, timeoutAfterStart)
	{
		initial();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	void operational()
	{
		initial();

		statechart->raiseResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStarted());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

	}
	TEST_F(SwitchTest, operational)
	{
		operational();
	}
	TEST_F(SwitchTest, restart)
	{
		operational();

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());

	}
	TEST_F(SwitchTest, clearing)
	{
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start));

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());

	}
	void turnLeft()
	{
		operational();

		doTurnLeftMock->setDefaultBehavior(&DoTurnLeftMock::doTurnLeft1);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseTurn();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(leftMock->calledAtLeastOnce());

		turning();

	}
	TEST_F(SwitchTest, turnLeft)
	{
		turnLeft();
	}
	void turnRight()
	{
		operational();

		doTurnLeftMock->setDefaultBehavior(&DoTurnLeftMock::doTurnLeft2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseTurn();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(rightMock->calledAtLeastOnce());

		turning();

	}
	TEST_F(SwitchTest, turnRight)
	{
		turnRight();
	}
	void queuedLeft()
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		lockedState();

	}
	TEST_F(SwitchTest, queuedLeft)
	{
		queuedLeft();
	}
	void queuedRight()
	{
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseRightResponse();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		lockedState();

	}
	TEST_F(SwitchTest, queuedRight)
	{
		queuedRight();
	}
	TEST_F(SwitchTest, freeLeft)
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

	}
	TEST_F(SwitchTest, freeRight)
	{
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

	}
	void okLeft()
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseLeftResponse();

		lockedState();

	}
	TEST_F(SwitchTest, okLeft)
	{
		okLeft();
	}
	void okRight()
	{
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseRightResponse();

		lockedState();

	}
	TEST_F(SwitchTest, okRight)
	{
		okRight();
	}
	TEST_F(SwitchTest, failLeft)
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SwitchTest, failRight)
	{
		turnRight();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SwitchTest, timeoutLeft)
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		isReservedMock->setDefaultBehavior(&IsReservedMock::isReserved1);

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SwitchTest, timeoutRight)
	{
		turnRight();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SwitchTest, unlockLeft)
	{
		okLeft();

		statechart->raiseUnlock();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

	}
	TEST_F(SwitchTest, unlockRight)
	{
		okRight();

		statechart->raiseUnlock();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

	}
	TEST_F(SwitchTest, failTurnRight)
	{
		okLeft();

		statechart->raiseResponse();

		failState();

	}
	TEST_F(SwitchTest, failTurnLeft)
	{
		okRight();

		statechart->raiseResponse();

		failState();

	}
	TEST_F(SwitchTest, doExit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		waitForStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		waitForStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		operational();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		turnLeft();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		turnRight();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		queuedLeft();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		queuedRight();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		okLeft();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		okRight();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failAfterStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

