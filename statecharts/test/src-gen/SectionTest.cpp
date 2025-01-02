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
#include "SectionStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void failState();
	void initing();
	void unlocked();
	void waitForStart();
	void initial();
	void failAfterStart();
	void sync1();
	void sync2();
	void operational();
	void operational2();
	void enabling();
	void disabling();
	void enabledLocked();
	void disabledLocked();
	void sectionFree();
	void sectionOccupied();
	void disablingAfterEnabled();
	void disabledAfterDisabled();
	void disabledAfterEnabled();
	void unlockSection();
	void nextReached();
	void leaveSection();
	void passedState();
	void unlockAfterPassed();
	void enablingAfterDisabled();
	mrw::statechart::SectionStatechart * statechart;


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

	class OffMock
	{
		typedef void (OffMock::*functiontype)();
	public:
		void (OffMock::*offBehaviorDefault)();
		int callCount;

		void off1()
		{
		}

		void offDefault()
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

		void off()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return offBehaviorDefault;
		}

		void setDefaultBehavior(void (OffMock::*defaultBehavior)())
		{
			offBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&OffMock::offDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static OffMock * offMock;

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

	class OnMock
	{
		typedef void (OnMock::*functiontype)();
	public:
		void (OnMock::*onBehaviorDefault)();
		int callCount;

		void on1()
		{
		}

		void onDefault()
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

		void on()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return onBehaviorDefault;
		}

		void setDefaultBehavior(void (OnMock::*defaultBehavior)())
		{
			onBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&OnMock::onDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static OnMock * onMock;

	class PassedMock
	{
		typedef void (PassedMock::*functiontype)();
	public:
		void (PassedMock::*passedBehaviorDefault)();
		int callCount;

		void passed1()
		{
		}

		void passedDefault()
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

		void passed()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return passedBehaviorDefault;
		}

		void setDefaultBehavior(void (PassedMock::*defaultBehavior)())
		{
			passedBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&PassedMock::passedDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static PassedMock * passedMock;

	class LeftBeforeMock
	{
		typedef void (LeftBeforeMock::*functiontype)();
	public:
		void (LeftBeforeMock::*leftBeforeBehaviorDefault)();
		int callCount;

		void leftBefore1()
		{
		}

		void leftBeforeDefault()
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

		void leftBefore()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return leftBeforeBehaviorDefault;
		}

		void setDefaultBehavior(void (LeftBeforeMock::*defaultBehavior)())
		{
			leftBeforeBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&LeftBeforeMock::leftBeforeDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static LeftBeforeMock * leftBeforeMock;

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

	class MockDefault : public mrw::statechart::SectionStatechart::OperationCallback
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
		void off()
		{
			offMock->off();
			return (offMock->*(offMock->getBehavior()))();
		}
		void on()
		{
			onMock->on();
			return (onMock->*(onMock->getBehavior()))();
		}
		void request()
		{
			requestMock->request();
			return (requestMock->*(requestMock->getBehavior()))();
		}
		void passed()
		{
			passedMock->passed();
			return (passedMock->*(passedMock->getBehavior()))();
		}
		void free()
		{
			freeMock->free();
			return (freeMock->*(freeMock->getBehavior()))();
		}
		void leftBefore()
		{
			leftBeforeMock->leftBefore();
			return (leftBeforeMock->*(leftBeforeMock->getBehavior()))();
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

	class SectionTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SectionStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			failMock = new FailMock();
			failMock->initializeBehavior();
			incMock = new IncMock();
			incMock->initializeBehavior();
			offMock = new OffMock();
			offMock->initializeBehavior();
			requestMock = new RequestMock();
			requestMock->initializeBehavior();
			freeMock = new FreeMock();
			freeMock->initializeBehavior();
			lockMock = new LockMock();
			lockMock->initializeBehavior();
			decMock = new DecMock();
			decMock->initializeBehavior();
			onMock = new OnMock();
			onMock->initializeBehavior();
			passedMock = new PassedMock();
			passedMock->initializeBehavior();
			leftBeforeMock = new LeftBeforeMock();
			leftBeforeMock->initializeBehavior();
			pendingMock = new PendingMock();
			pendingMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete pendingMock;
			delete leftBeforeMock;
			delete passedMock;
			delete onMock;
			delete decMock;
			delete lockMock;
			delete freeMock;
			delete requestMock;
			delete offMock;
			delete incMock;
			delete failMock;
			delete statechart;
			delete runner;
		}
	};

	void failState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStop());

	}
	void initing()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation));

		EXPECT_TRUE(statechart->isRaisedEntered());

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());

		EXPECT_TRUE(requestMock->calledAtLeastOnce());

	}
	void unlocked()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(freeMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

	}

	void waitForStart()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start));












	}
	TEST_F(SectionTest, waitForStart)
	{
		waitForStart();
	}
	void initial()
	{
		waitForStart();

		statechart->raiseStart();

		initing();

	}
	TEST_F(SectionTest, initial)
	{
		initial();
	}
	void failAfterStart()
	{
		initial();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SectionTest, failAfterStart)
	{
		failAfterStart();
	}
	TEST_F(SectionTest, timeoutAfterStart)
	{
		initial();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	void sync1()
	{
		initial();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, sync1)
	{
		sync1();
	}
	void sync2()
	{
		initial();

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Relay));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Wait));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, sync2)
	{
		sync2();
	}
	void operational()
	{
		sync1();

		statechart->raiseStateResponse(false);

		unlocked();

		EXPECT_TRUE(statechart->isRaisedStarted());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE((statechart->getOccupied()) == (false));

	}
	TEST_F(SectionTest, operational)
	{
		operational();
	}
	void operational2()
	{
		sync2();

		statechart->raiseRelaisResponse();

		unlocked();

		EXPECT_TRUE(statechart->isRaisedStarted());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE((statechart->getOccupied()) == (false));

	}
	TEST_F(SectionTest, operational2)
	{
		operational2();
	}
	TEST_F(SectionTest, restart)
	{
		operational();

		statechart->raiseStart();

		initing();

	}
	TEST_F(SectionTest, clearing)
	{
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start));

		statechart->raiseStart();

		initing();

	}
	void enabling()
	{
		operational();

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, enabling)
	{
		enabling();
	}
	void disabling()
	{
		operational();

		statechart->raiseEnable(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Disabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, disabling)
	{
		disabling();
	}
	void enabledLocked()
	{
		enabling();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

		EXPECT_TRUE(!statechart->getOccupied());

	}
	TEST_F(SectionTest, enabledLocked)
	{
		enabledLocked();
	}
	void disabledLocked()
	{
		disabling();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

		EXPECT_TRUE(!statechart->getOccupied());

	}
	TEST_F(SectionTest, disabledLocked)
	{
		disabledLocked();
	}
	void sectionFree()
	{
		operational();

		statechart->setOccupied(false);

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));

	}
	TEST_F(SectionTest, sectionFree)
	{
		sectionFree();
	}
	void sectionOccupied()
	{
		operational();

		statechart->setOccupied(true);

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Occupied));

	}
	TEST_F(SectionTest, sectionOccupied)
	{
		sectionOccupied();
	}
	TEST_F(SectionTest, failAfterEnable)
	{
		enabling();

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, timeoutAfterEnable)
	{
		enabling();

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

	}
	void disablingAfterEnabled()
	{
		enabledLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(offMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, disablingAfterEnabled)
	{
		disablingAfterEnabled();
	}
	void disabledAfterDisabled()
	{
		disabledLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(0));

	}
	TEST_F(SectionTest, disabledAfterDisabled)
	{
		disabledAfterDisabled();
	}
	void disabledAfterEnabled()
	{
		disablingAfterEnabled();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

	}
	TEST_F(SectionTest, disabledAfterEnabled)
	{
		disabledAfterEnabled();
	}
	void unlockSection()
	{
		disabledAfterDisabled();

		statechart->raiseUnlock();

		unlocked();

	}
	TEST_F(SectionTest, unlockSection)
	{
		unlockSection();
	}
	TEST_F(SectionTest, failAfterDisable)
	{
		disablingAfterEnabled();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SectionTest, timeAfterDisable)
	{
		disablingAfterEnabled();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	void nextReached()
	{
		sectionFree();

		statechart->setAuto_off(true);

		statechart->raiseStateResponse(true);

		EXPECT_TRUE(statechart->getOccupied());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Occupied));

		EXPECT_TRUE(statechart->isRaisedEntered());

		statechart->raiseNext();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Next_Reached));

	}
	TEST_F(SectionTest, nextReached)
	{
		nextReached();
	}
	void leaveSection()
	{
		nextReached();

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(!statechart->getOccupied());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation__final_));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(offMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeave());

		EXPECT_TRUE(statechart->isRaisedLeaving());

	}
	TEST_F(SectionTest, leaveSection)
	{
		leaveSection();
	}
	TEST_F(SectionTest, leaveBeforeNext)
	{
		sectionFree();

		statechart->setAuto_off(true);

		statechart->raiseStateResponse(true);

		EXPECT_TRUE(statechart->getOccupied());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Occupied));

		EXPECT_TRUE(statechart->isRaisedEntered());

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(!statechart->getOccupied());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Occupied));

		statechart->raiseNext();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Next_Reached));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(offMock->calledAtLeastOnce());

		EXPECT_TRUE(leftBeforeMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeave());

		EXPECT_TRUE(statechart->isRaisedLeaving());

	}
	TEST_F(SectionTest, stay)
	{
		enabledLocked();

		statechart->setAuto_off(false);

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

	}
	TEST_F(SectionTest, autoUnlock)
	{
		leaveSection();

		statechart->setAuto_unlock(true);

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(statechart->isRaisedLeft());

	}
	void passedState()
	{
		leaveSection();

		statechart->setAuto_unlock(false);

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(passedMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeft());

	}
	TEST_F(SectionTest, passedState)
	{
		passedState();
	}
	void unlockAfterPassed()
	{
		passedState();

		statechart->raiseUnlock();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(statechart->isRaisedUnregister());

		EXPECT_TRUE(freeMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, unlockAfterPassed)
	{
		unlockAfterPassed();
	}
	void enablingAfterDisabled()
	{
		disabledLocked();

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(onMock->calledAtLeastOnce());

	}
	TEST_F(SectionTest, enablingAfterDisabled)
	{
		enablingAfterDisabled();
	}
	TEST_F(SectionTest, enabledAfterDisabled)
	{
		enablingAfterDisabled();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

	}
	TEST_F(SectionTest, doExit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		waitForStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		initial();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		sync1();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		sync2();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		operational();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		operational2();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		enabling();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disabling();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		enabledLocked();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disabledLocked();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disablingAfterEnabled();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		enablingAfterDisabled();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disabledAfterEnabled();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disabledAfterDisabled();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		unlockSection();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		unlockAfterPassed();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		leaveSection();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		passedState();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		sectionFree();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		sectionOccupied();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		nextReached();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

