/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "SignalControllerStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void failState();
	void unlockedState();
	void waitForStart();
	void initial();
	void failAfterStart();
	void operational();
	void operationalCombined();
	void pendingTour();
	void pendingTourCombined();
	void pendingShunting();
	void tourLocked();
	void tourLockedCombined();
	void shuntingLocked();
	void disableTourCombined();
	mrw::statechart::SignalControllerStatechart * statechart;


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

	class HasMainSignalMock
	{
		typedef bool (HasMainSignalMock::*functiontype)();
	public:
		bool (HasMainSignalMock::*hasMainSignalBehaviorDefault)();

		bool hasMainSignal1()
		{
			return (false);
		}

		bool hasMainSignal2()
		{
			return (true);
		}

		bool hasMainSignalDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return hasMainSignalBehaviorDefault;
		}

		void setDefaultBehavior(bool (HasMainSignalMock::*defaultBehavior)())
		{
			hasMainSignalBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&HasMainSignalMock::hasMainSignalDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static HasMainSignalMock * hasMainSignalMock;

	class IsLightSignalMock
	{
		typedef bool (IsLightSignalMock::*functiontype)();
	public:
		bool (IsLightSignalMock::*isLightSignalBehaviorDefault)();

		bool isLightSignal1()
		{
			return (false);
		}

		bool isLightSignal2()
		{
			return (true);
		}

		bool isLightSignalDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return isLightSignalBehaviorDefault;
		}

		void setDefaultBehavior(bool (IsLightSignalMock::*defaultBehavior)())
		{
			isLightSignalBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&IsLightSignalMock::isLightSignalDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static IsLightSignalMock * isLightSignalMock;

	class IsMainAndShuntMock
	{
		typedef bool (IsMainAndShuntMock::*functiontype)();
	public:
		bool (IsMainAndShuntMock::*isMainAndShuntBehaviorDefault)();

		bool isMainAndShunt1()
		{
			return (false);
		}

		bool isMainAndShunt2()
		{
			return (true);
		}

		bool isMainAndShuntDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return isMainAndShuntBehaviorDefault;
		}

		void setDefaultBehavior(bool (IsMainAndShuntMock::*defaultBehavior)())
		{
			isMainAndShuntBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&IsMainAndShuntMock::isMainAndShuntDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static IsMainAndShuntMock * isMainAndShuntMock;

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

	class MockDefault : public mrw::statechart::SignalControllerStatechart::OperationCallback
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
		bool hasMainSignal()
		{
			return (hasMainSignalMock->*(hasMainSignalMock->getBehavior()))();
		}
		bool isMainAndShunt()
		{
			return (isMainAndShuntMock->*(isMainAndShuntMock->getBehavior()))();
		}
		bool isLightSignal()
		{
			return (isLightSignalMock->*(isLightSignalMock->getBehavior()))();
		}
		bool isTour()
		{
			return (isTourMock->*(isTourMock->getBehavior()))();
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

	class SignalControllerTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SignalControllerStatechart();
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

	void failState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


	}
	void unlockedState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(lockMock->calledAtLeastOnce());


	}

	void waitForStart()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start));





		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal1);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		isTourMock->setDefaultBehavior(&IsTourMock::isTour1);



		incMock->reset();
		decMock->reset();
		failMock->reset();
		pendingMock->reset();
		hasMainSignalMock->reset();
		isLightSignalMock->reset();
		isMainAndShuntMock->reset();
		isTourMock->reset();
		lockMock->reset();
	}
	TEST_F(SignalControllerTest, waitForStart)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		waitForStart();
	}
	void initial()
	{
		waitForStart();

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE(incMock->calledAtLeastOnce());


		incMock->reset();
	}
	TEST_F(SignalControllerTest, initial)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

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
	TEST_F(SignalControllerTest, failAfterStart)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failAfterStart();
	}
	TEST_F(SignalControllerTest, timeoutAfterStart)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		initial();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	TEST_F(SignalControllerTest, operationalVersion1)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		initial();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal1);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isRaisedStarted());

		unlockedState();


		hasMainSignalMock->reset();
		isMainAndShuntMock->reset();
	}
	TEST_F(SignalControllerTest, operationalVersion2)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		initial();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Completed));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isRaisedStarted());

		unlockedState();


		hasMainSignalMock->reset();
		isMainAndShuntMock->reset();
	}
	void operational()
	{
		initial();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal1);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		statechart->raiseCompletedMain();

		statechart->raiseCompletedDistant();

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isRaisedStarted());

		unlockedState();


		hasMainSignalMock->reset();
		isMainAndShuntMock->reset();
	}
	TEST_F(SignalControllerTest, operational)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();
	}
	void operationalCombined()
	{
		initial();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseCompletedMain();

		statechart->raiseCompletedDistant();

		statechart->raiseCompletedShunt();

		unlockedState();


		hasMainSignalMock->reset();
		isMainAndShuntMock->reset();
	}
	TEST_F(SignalControllerTest, operationalCombined)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operationalCombined();
	}
	TEST_F(SignalControllerTest, restart)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();

		statechart->raiseStart();

		initial();


	}
	TEST_F(SignalControllerTest, clearing)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start));

		statechart->raiseStart();

		initial();


	}
	TEST_F(SignalControllerTest, enableStop)
	{
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();

		statechart->setSymbol(statechart->getSTOP());

		statechart->raiseEnable();

		unlockedState();


	}
	void pendingTour()
	{
		operational();

		statechart->setSymbol(statechart->getGO());

		isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

		statechart->raiseEnable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt));

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE((statechart->getTurnShuntValue()) == (statechart->getGO()));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getGO()));

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getGO()));


		isTourMock->reset();
		incMock->reset();
		pendingMock->reset();
	}
	TEST_F(SignalControllerTest, pendingTour)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingTour();
	}
	void pendingTourCombined()
	{
		statechart->setSymbol(statechart->getGO());

		operationalCombined();

		isTourMock->setDefaultBehavior(&IsTourMock::isTour2);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseEnable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getGO()));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getGO()));


		isTourMock->reset();
		isMainAndShuntMock->reset();
		incMock->reset();
		pendingMock->reset();
	}
	TEST_F(SignalControllerTest, pendingTourCombined)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingTourCombined();
	}
	void pendingShunting()
	{
		statechart->setSymbol(statechart->getGO());

		operational();

		isTourMock->setDefaultBehavior(&IsTourMock::isTour1);

		statechart->raiseEnable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Shunt));

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE((statechart->getTurnShuntValue()) == (statechart->getGO()));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());


		isTourMock->reset();
		incMock->reset();
		pendingMock->reset();
	}
	TEST_F(SignalControllerTest, pendingShunting)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingShunting();
	}
	void tourLocked()
	{
		pendingTour();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SignalControllerTest, tourLocked)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();
	}
	void tourLockedCombined()
	{
		pendingTourCombined();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SignalControllerTest, tourLockedCombined)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLockedCombined();
	}
	void shuntingLocked()
	{
		pendingShunting();

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SignalControllerTest, shuntingLocked)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();
	}
	TEST_F(SignalControllerTest, disableTour)
	{
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal1);

		statechart->raiseDisable();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt));

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE((statechart->getTurnShuntValue()) == (statechart->getSTOP()));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getSTOP()));

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getSTOP()));

		statechart->raiseCompletedDistant();

		unlockedState();


		isMainAndShuntMock->reset();
		isLightSignalMock->reset();
		hasMainSignalMock->reset();
		incMock->reset();
	}
	void disableTourCombined()
	{
		tourLockedCombined();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		statechart->raiseDisable();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getSTOP()));

		statechart->raiseCompletedMain();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getSTOP()));


		isMainAndShuntMock->reset();
		hasMainSignalMock->reset();
		isLightSignalMock->reset();
		incMock->reset();
	}
	TEST_F(SignalControllerTest, disableTourCombined)
	{
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableTourCombined();
	}
	TEST_F(SignalControllerTest, disableTourCombinedLight)
	{
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableTourCombined();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal2);

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay));

		runner->proceed_time(statechart->getDelay());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getOFF()));

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		unlockedState();


		isMainAndShuntMock->reset();
		hasMainSignalMock->reset();
		isLightSignalMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, disableTourCombinedForm)
	{
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableTourCombined();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		statechart->raiseCompletedDistant();

		unlockedState();


		isMainAndShuntMock->reset();
		hasMainSignalMock->reset();
		isLightSignalMock->reset();
	}
	TEST_F(SignalControllerTest, disableShunting)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();

		statechart->raiseDisable();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE((statechart->getTurnShuntValue()) == (statechart->getSTOP()));

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		unlockedState();


		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, extendTour)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}
	TEST_F(SignalControllerTest, extendTourCombined)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLockedCombined();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}
	TEST_F(SignalControllerTest, extendShunting)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend));

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}
	TEST_F(SignalControllerTest, failedTour)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();

		statechart->raiseFailed();

		failState();


	}
	TEST_F(SignalControllerTest, failedTourCombined)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLockedCombined();

		statechart->raiseFailed();

		failState();


	}
	TEST_F(SignalControllerTest, failedShunting)
	{
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();

		statechart->raiseFailed();

		failState();


	}
	TEST_F(SignalControllerTest, timeoutPendingToTour)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingTour();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	TEST_F(SignalControllerTest, timeoutPendingToTourCombined)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingTourCombined();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	TEST_F(SignalControllerTest, timeoutPendingToShunting)
	{
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		pendingShunting();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableTour)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableTourCombined)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLockedCombined();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableShunting)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableTour)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableTourCombined)
	{
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		tourLockedCombined();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		isMainAndShuntMock->reset();
		incMock->reset();
		decMock->reset();
	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableShunting)
	{
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		incMock = new IncMock();
		incMock->initializeBehavior();
		decMock = new DecMock();
		decMock->initializeBehavior();
		failMock = new FailMock();
		failMock->initializeBehavior();
		pendingMock = new PendingMock();
		pendingMock->initializeBehavior();
		hasMainSignalMock = new HasMainSignalMock();
		hasMainSignalMock->initializeBehavior();
		isLightSignalMock = new IsLightSignalMock();
		isLightSignalMock->initializeBehavior();
		isMainAndShuntMock = new IsMainAndShuntMock();
		isMainAndShuntMock->initializeBehavior();
		isTourMock = new IsTourMock();
		isTourMock->initializeBehavior();
		lockMock = new LockMock();
		lockMock->initializeBehavior();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		shuntingLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();


		incMock->reset();
		decMock->reset();
	}

}

