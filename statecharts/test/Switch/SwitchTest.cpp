/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
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
	void okLeft();
	void okRight();
	mrw::statechart::SwitchStatechart * statechart;


	class IncMock
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

		void inc()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static IncMock * incMock;

	class PendingMock
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

		void pending()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static PendingMock * pendingMock;

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
		std::list<LockMock::parameters> paramCount;
		int callCount;

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
	static LockMock * lockMock;

	class FailMock
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

		void fail()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FailMock * failMock;

	class RequestMock
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

		void request()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static RequestMock * requestMock;

	class DecMock
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

		void dec()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static DecMock * decMock;

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

	class LeftMock
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

		void left()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static LeftMock * leftMock;

	class RightMock
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

		void right()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static RightMock * rightMock;

	class IsFreeMock
	{
		typedef bool (IsFreeMock::*functiontype)();
	public:
		bool (IsFreeMock::*isFreeBehaviorDefault)();

		bool isFree1()
		{
			return (false);
		}

		bool isFree2()
		{
			return (true);
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

	class MockDefault : public mrw::statechart::SwitchStatechart::OperationCallback
	{
	public:
		void inc()
		{
			incMock->inc();
		}
		void dec()
		{
			decMock->dec();
		}
		void left()
		{
			leftMock->left();
		}
		void right()
		{
			rightMock->right();
		}
		void request()
		{
			requestMock->request();
		}
		bool doTurnLeft()
		{
			return (doTurnLeftMock->*(doTurnLeftMock->getBehavior()))();
		}
		bool isFree()
		{
			return (isFreeMock->*(isFreeMock->getBehavior()))();
		}
		void fail()
		{
			failMock->fail();
		}
		void pending()
		{
			pendingMock->pending();
		}
		void lock(bool do_it)
		{
			lockMock->lock(do_it);
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class SwitchTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SwitchStatechart();
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

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start));


	}
	TEST_F(SwitchTest, waitForStart)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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


		incMock->reset();
		requestMock->reset();
	}
	TEST_F(SwitchTest, initial)
	{
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failAfterStart();
	}
	TEST_F(SwitchTest, timeoutAfterStart)
	{
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SwitchTest, operational)
	{
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();
	}
	TEST_F(SwitchTest, restart)
	{
		incMock = new IncMock();
		requestMock = new RequestMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());


		incMock->reset();
		requestMock->reset();
	}
	TEST_F(SwitchTest, clearing)
	{
		incMock = new IncMock();
		requestMock = new RequestMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Wait_for_Start));

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());


		incMock->reset();
		requestMock->reset();
	}
	void turnLeft()
	{
		operational();

		doTurnLeftMock->setDefaultBehavior(&DoTurnLeftMock::doTurnLeft1);

		statechart->raiseTurn();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Left));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(leftMock->calledAtLeastOnce());

		turning();


		doTurnLeftMock->reset();
		incMock->reset();
		leftMock->reset();
	}
	TEST_F(SwitchTest, turnLeft)
	{
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnLeft();
	}
	void turnRight()
	{
		operational();

		doTurnLeftMock->setDefaultBehavior(&DoTurnLeftMock::doTurnLeft2);

		statechart->raiseTurn();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Turn_Right));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(rightMock->calledAtLeastOnce());

		turning();


		doTurnLeftMock->reset();
		incMock->reset();
		rightMock->reset();
	}
	TEST_F(SwitchTest, turnRight)
	{
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnRight();
	}
	TEST_F(SwitchTest, queuedLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		decMock = new DecMock();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		lockedState();


		isFreeMock->reset();
		decMock->reset();
	}
	TEST_F(SwitchTest, queuedRight)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		decMock = new DecMock();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseRightResponse();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		lockedState();


		isFreeMock->reset();
		decMock->reset();
	}
	TEST_F(SwitchTest, freeLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		decMock = new DecMock();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		isFreeMock->reset();
		decMock->reset();
	}
	TEST_F(SwitchTest, freeRight)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		decMock = new DecMock();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree2);

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Turning_Turning_process_Pending));

		statechart->raiseLeftResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		isFreeMock->reset();
		decMock->reset();
	}
	void okLeft()
	{
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		statechart->raiseLeftResponse();

		lockedState();


		isFreeMock->reset();
	}
	TEST_F(SwitchTest, okLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okLeft();
	}
	void okRight()
	{
		turnRight();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		statechart->raiseRightResponse();

		lockedState();


		isFreeMock->reset();
	}
	TEST_F(SwitchTest, okRight)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okRight();
	}
	TEST_F(SwitchTest, failLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		statechart->raiseFailed();

		failState();


		isFreeMock->reset();
	}
	TEST_F(SwitchTest, failRight)
	{
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnRight();

		statechart->raiseFailed();

		failState();


	}
	TEST_F(SwitchTest, timeoutLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnLeft();

		isFreeMock->setDefaultBehavior(&IsFreeMock::isFree1);

		runner->proceed_time(statechart->getTimeout());

		failState();


		isFreeMock->reset();
	}
	TEST_F(SwitchTest, timeoutRight)
	{
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turnRight();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	TEST_F(SwitchTest, unlockLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okLeft();

		statechart->raiseUnlock();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));


	}
	TEST_F(SwitchTest, unlockRight)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okRight();

		statechart->raiseUnlock();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SwitchStatechart::State::main_region_Operating_operating_Unlocked));


	}
	TEST_F(SwitchTest, failTurnRight)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		leftMock = new LeftMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okLeft();

		statechart->raiseResponse();

		failState();


	}
	TEST_F(SwitchTest, failTurnLeft)
	{
		isFreeMock = new IsFreeMock();
		isFreeMock->initializeBehavior();
		doTurnLeftMock = new DoTurnLeftMock();
		doTurnLeftMock->initializeBehavior();
		incMock = new IncMock();
		rightMock = new RightMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		requestMock = new RequestMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okRight();

		statechart->raiseResponse();

		failState();


	}

}

