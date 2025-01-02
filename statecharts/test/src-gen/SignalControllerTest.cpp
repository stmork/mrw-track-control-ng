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
	void startVersion1();
	void startVersion2();
	void failAfterStart();
	void operational();
	void operationalCombined();
	void pendingTour();
	void pendingTourLight();
	void pendingTourForm();
	void pendingTourCombined();
	void pendingShunting();
	void tourLocked();
	void tourLockedCombined();
	void shuntingLocked();
	void disableTour();
	void disableTourCombined();
	void disableTourCombinedCompleted();
	void disableTourCombinedLight();
	void disableTourCombinedForm();
	void disableShunting();
	void disableShuntingCompleted();
	void extendTour();
	void extendTourCompleted();
	void extendTourCombined();
	void extendTourCombinedCompleted();
	void extendShunting();
	void extendShuntingCompleted();
	void failedTour();
	void failedTourCombined();
	void failedShunting();
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
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SignalControllerStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			failMock = new FailMock();
			failMock->initializeBehavior();
			lockMock = new LockMock();
			lockMock->initializeBehavior();
			incMock = new IncMock();
			incMock->initializeBehavior();
			decMock = new DecMock();
			decMock->initializeBehavior();
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
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete isTourMock;
			delete isMainAndShuntMock;
			delete isLightSignalMock;
			delete hasMainSignalMock;
			delete pendingMock;
			delete decMock;
			delete incMock;
			delete lockMock;
			delete failMock;
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


	}
	TEST_F(SignalControllerTest, waitForStart)
	{
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

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

	}
	TEST_F(SignalControllerTest, initial)
	{
		initial();
	}
	void startVersion1()
	{
		waitForStart();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal1);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE(!statechart->isRaisedTurnShunt());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Completed));

	}
	TEST_F(SignalControllerTest, startVersion1)
	{
		startVersion1();
	}
	void startVersion2()
	{
		waitForStart();

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		statechart->raiseStart();

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_main_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_distant_Turn));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Init_Init_process_Turning_shunt_Turn));

	}
	TEST_F(SignalControllerTest, startVersion2)
	{
		startVersion2();
	}
	void failAfterStart()
	{
		initial();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SignalControllerTest, failAfterStart)
	{
		failAfterStart();
	}
	TEST_F(SignalControllerTest, timeoutAfterStart)
	{
		initial();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalControllerTest, operationalVersion1)
	{
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

	}
	TEST_F(SignalControllerTest, operationalVersion2)
	{
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

	}
	TEST_F(SignalControllerTest, operational)
	{
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

	}
	TEST_F(SignalControllerTest, operationalCombined)
	{
		operationalCombined();
	}
	TEST_F(SignalControllerTest, restart)
	{
		operational();

		statechart->raiseStart();

		initial();

	}
	TEST_F(SignalControllerTest, clearing)
	{
		failAfterStart();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Wait_for_Start));

		EXPECT_TRUE(statechart->isRaisedCleared());

		statechart->raiseStart();

		initial();

	}
	TEST_F(SignalControllerTest, enableStop)
	{
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

		EXPECT_TRUE(pendingMock->calledAtLeast(1));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Main));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getGO()));

	}
	TEST_F(SignalControllerTest, pendingTour)
	{
		pendingTour();
	}
	void pendingTourLight()
	{
		pendingTour();

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal2);

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Delay));

		runner->proceed_time(statechart->getDelay());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getGO()));

	}
	TEST_F(SignalControllerTest, pendingTourLight)
	{
		pendingTourLight();
	}
	void pendingTourForm()
	{
		pendingTour();

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getGO()));

	}
	TEST_F(SignalControllerTest, pendingTourForm)
	{
		pendingTourForm();
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

		EXPECT_TRUE(pendingMock->calledAtLeast(1));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		statechart->raiseCompletedMain();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Pending_Pending_Go_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getGO()));

	}
	TEST_F(SignalControllerTest, pendingTourCombined)
	{
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

		EXPECT_TRUE(pendingMock->calledAtLeast(1));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

	}
	TEST_F(SignalControllerTest, pendingShunting)
	{
		pendingShunting();
	}
	void tourLocked()
	{
		pendingTourLight();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, tourLocked)
	{
		tourLocked();
	}
	void tourLockedCombined()
	{
		pendingTourCombined();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, tourLockedCombined)
	{
		tourLockedCombined();
	}
	void shuntingLocked()
	{
		pendingShunting();

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		EXPECT_TRUE(lockMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, shuntingLocked)
	{
		shuntingLocked();
	}
	void disableTour()
	{
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

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

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

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		unlockedState();

	}
	TEST_F(SignalControllerTest, disableTour)
	{
		disableTour();
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

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(statechart->isRaisedTurnMain());

		EXPECT_TRUE((statechart->getTurnMainValue()) == (statechart->getSTOP()));

	}
	TEST_F(SignalControllerTest, disableTourCombined)
	{
		disableTourCombined();
	}
	void disableTourCombinedCompleted()
	{
		disableTourCombined();

		statechart->raiseCompletedMain();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant));

		EXPECT_TRUE(statechart->isRaisedTurnDistant());

		EXPECT_TRUE((statechart->getTurnDistantValue()) == (statechart->getSTOP()));

	}
	TEST_F(SignalControllerTest, disableTourCombinedCompleted)
	{
		disableTourCombinedCompleted();
	}
	void disableTourCombinedLight()
	{
		disableTourCombinedCompleted();

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

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		unlockedState();

	}
	TEST_F(SignalControllerTest, disableTourCombinedLight)
	{
		disableTourCombinedLight();
	}
	void disableTourCombinedForm()
	{
		disableTourCombinedCompleted();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt1);

		hasMainSignalMock->setDefaultBehavior(&HasMainSignalMock::hasMainSignal2);

		isLightSignalMock->setDefaultBehavior(&IsLightSignalMock::isLightSignal1);

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		unlockedState();

	}
	TEST_F(SignalControllerTest, disableTourCombinedForm)
	{
		disableTourCombinedForm();
	}
	void disableShunting()
	{
		shuntingLocked();

		statechart->raiseDisable();

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(statechart->isRaisedTurnShunt());

		EXPECT_TRUE((statechart->getTurnShuntValue()) == (statechart->getSTOP()));

	}
	TEST_F(SignalControllerTest, disableShunting)
	{
		disableShunting();
	}
	void disableShuntingCompleted()
	{
		disableShunting();

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(decMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

		unlockedState();

	}
	TEST_F(SignalControllerTest, disableShuntingCompleted)
	{
		disableShuntingCompleted();
	}
	void extendTour()
	{
		tourLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

	}
	TEST_F(SignalControllerTest, extendTour)
	{
		extendTour();
	}
	void extendTourCompleted()
	{
		extendTour();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, extendTourCompleted)
	{
		extendTourCompleted();
	}
	void extendTourCombined()
	{
		tourLockedCombined();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

	}
	TEST_F(SignalControllerTest, extendTourCombined)
	{
		extendTourCombined();
	}
	void extendTourCombinedCompleted()
	{
		extendTourCombined();

		statechart->raiseCompletedDistant();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, extendTourCombinedCompleted)
	{
		extendTourCombinedCompleted();
	}
	void extendShunting()
	{
		shuntingLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeast(1));

		EXPECT_TRUE(decMock->calledAtLeast(0));

	}
	TEST_F(SignalControllerTest, extendShunting)
	{
		extendShunting();
	}
	void extendShuntingCompleted()
	{
		extendShunting();

		statechart->raiseCompletedShunt();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Idle));

		EXPECT_TRUE(incMock->calledAtLeast(0));

		EXPECT_TRUE(decMock->calledAtLeast(1));

	}
	TEST_F(SignalControllerTest, extendShuntingCompleted)
	{
		extendShuntingCompleted();
	}
	void failedTour()
	{
		tourLocked();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SignalControllerTest, failedTour)
	{
		failedTour();
	}
	void failedTourCombined()
	{
		tourLockedCombined();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SignalControllerTest, failedTourCombined)
	{
		failedTourCombined();
	}
	void failedShunting()
	{
		shuntingLocked();

		statechart->raiseFailed();

		failState();

	}
	TEST_F(SignalControllerTest, failedShunting)
	{
		failedShunting();
	}
	TEST_F(SignalControllerTest, timeoutPendingToTour)
	{
		pendingTour();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutPendingToTourCombined)
	{
		pendingTourCombined();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutPendingToShunting)
	{
		pendingShunting();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableTour)
	{
		tourLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableTourCombined)
	{
		tourLockedCombined();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingEnableShunting)
	{
		shuntingLocked();

		statechart->raiseExtend();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableTour)
	{
		tourLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableTourCombined)
	{
		tourLockedCombined();

		isMainAndShuntMock->setDefaultBehavior(&IsMainAndShuntMock::isMainAndShunt2);

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, timeoutWaitingDisableShunting)
	{
		shuntingLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalControllerStatechart::State::main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		failState();

	}
	TEST_F(SignalControllerTest, doExit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		waitForStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		initial();

		statechart->raiseCompletedDistant();

		statechart->raiseCompletedMain();

		statechart->raiseCompletedShunt();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		startVersion1();

		statechart->raiseCompletedDistant();

		statechart->raiseCompletedMain();

		statechart->raiseCompletedShunt();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		startVersion2();

		statechart->raiseCompletedDistant();

		statechart->raiseCompletedMain();

		statechart->raiseCompletedShunt();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		operational();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		pendingTourLight();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		pendingTourForm();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		pendingTourCombined();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendTourCompleted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendTourCombined();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		tourLocked();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		tourLockedCombined();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendTour();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendTourCombinedCompleted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableTour();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableTourCombined();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableTourCombinedCompleted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableTourCombinedLight();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		pendingShunting();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		shuntingLocked();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendShunting();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		extendShuntingCompleted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableShunting();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableShuntingCompleted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failAfterStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failedTour();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failedTourCombined();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failedShunting();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		disableTourCombinedForm();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

