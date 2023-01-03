/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "RouteStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void disabled();
	void start();
	void switches();
	void turningSignals();
	void extendingSignals();
	void sections();
	void activate();
	void deactivate();
	void failTurningSwitchesIncomplete();
	void failTurningSignalsIncomplete();
	void failTurningSectionsIncomplete();
	mrw::statechart::RouteStatechart * statechart;


	class ResetTransactionMock
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

		void resetTransaction()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static ResetTransactionMock * resetTransactionMock;

	class DeactivateSectionsMock
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

		void deactivateSections()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static DeactivateSectionsMock * deactivateSectionsMock;

	class UnlockSignalsMock
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

		void unlockSignals()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static UnlockSignalsMock * unlockSignalsMock;

	class UnlockSwitchesMock
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

		void unlockSwitches()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static UnlockSwitchesMock * unlockSwitchesMock;

	class TurnSwitchesMock
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

		void turnSwitches()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static TurnSwitchesMock * turnSwitchesMock;

	class TurnSignalsMock
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

		void turnSignals()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static TurnSignalsMock * turnSignalsMock;

	class ActivateSectionsMock
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

		void activateSections()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static ActivateSectionsMock * activateSectionsMock;

	class IsCompletedMock
	{
		typedef bool (IsCompletedMock::*functiontype)();
	public:
		bool (IsCompletedMock::*isCompletedBehaviorDefault)();

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
	static IsCompletedMock * isCompletedMock;

	class MockDefault : public mrw::statechart::RouteStatechart::OperationCallback
	{
	public:
		void resetTransaction()
		{
			resetTransactionMock->resetTransaction();
		}
		void fail()
		{
		}
		void tryComplete()
		{
		}
		bool isCompleted()
		{
			return (isCompletedMock->*(isCompletedMock->getBehavior()))();
		}
		void turnSwitches()
		{
			turnSwitchesMock->turnSwitches();
		}
		void activateSections()
		{
			activateSectionsMock->activateSections();
		}
		void turnSignals()
		{
			turnSignalsMock->turnSignals();
		}
		void extendSignals()
		{
		}
		void deactivateSections()
		{
			deactivateSectionsMock->deactivateSections();
		}
		void unlockSignals()
		{
			unlockSignalsMock->unlockSignals();
		}
		void unlockSwitches()
		{
			unlockSwitchesMock->unlockSwitches();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class RouteTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::RouteStatechart();
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

	void disabled()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Disable));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSwitchesMock->calledAtLeastOnce());


	}

	void start()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Start));


	}
	TEST_F(RouteTest, start)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		start();
	}
	void switches()
	{
		start();

		statechart->raiseExtended();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());


		resetTransactionMock->reset();
		turnSwitchesMock->reset();
	}
	TEST_F(RouteTest, switches)
	{
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		switches();
	}
	void turningSignals()
	{
		switches();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Turning));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(turnSignalsMock->calledAtLeastOnce());


		resetTransactionMock->reset();
		turnSignalsMock->reset();
	}
	TEST_F(RouteTest, turningSignals)
	{
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turningSignals();
	}
	void extendingSignals()
	{
		turningSignals();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Signal_Updating));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(turnSignalsMock->calledAtLeastOnce());


		resetTransactionMock->reset();
		turnSignalsMock->reset();
	}
	TEST_F(RouteTest, extendingSignals)
	{
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		extendingSignals();
	}
	void sections()
	{
		extendingSignals();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Section_Activation));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(activateSectionsMock->calledAtLeastOnce());


		resetTransactionMock->reset();
		activateSectionsMock->reset();
	}
	TEST_F(RouteTest, sections)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		sections();
	}
	void activate()
	{
		sections();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Active));

		EXPECT_TRUE(statechart->isRaisedActivated());


	}
	TEST_F(RouteTest, activate)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		activate();
	}
	TEST_F(RouteTest, extend)
	{
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		activate();

		statechart->raiseExtended();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Turning_Turning_process_Switch_Turning));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(turnSwitchesMock->calledAtLeastOnce());


		resetTransactionMock->reset();
		turnSwitchesMock->reset();
	}
	void deactivate()
	{
		activate();

		statechart->raiseDisable();

		disabled();


	}
	TEST_F(RouteTest, deactivate)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		deactivate();
	}
	TEST_F(RouteTest, finish)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		deactivate();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedFinished());

		EXPECT_TRUE(statechart->isActive());


	}
	TEST_F(RouteTest, timeout)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		deactivate();

		runner->proceed_time(statechart->getEmergency());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedFinished());

		EXPECT_TRUE(statechart->isActive());


	}
	TEST_F(RouteTest, failActive)
	{
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		activate();

		statechart->raiseFailed();

		disabled();


	}
	TEST_F(RouteTest, failTurningSwitches)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		switches();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

		statechart->raiseFailed();

		disabled();


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failTurningSignals)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turningSignals();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

		statechart->raiseFailed();

		disabled();


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failExtendingSignals)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		extendingSignals();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

		statechart->raiseFailed();

		disabled();


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failTurningSections)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		sections();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted1);

		statechart->raiseFailed();

		disabled();


		isCompletedMock->reset();
	}
	void failTurningSwitchesIncomplete()
	{
		switches();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failTurningSwitchesIncomplete)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSwitchesIncomplete();
	}
	void failTurningSignalsIncomplete()
	{
		turningSignals();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failTurningSignalsIncomplete)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSignalsIncomplete();
	}
	TEST_F(RouteTest, failExtendingSignalsIncomplete)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		extendingSignals();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


		isCompletedMock->reset();
	}
	void failTurningSectionsIncomplete()
	{
		sections();

		isCompletedMock->setDefaultBehavior(&IsCompletedMock::isCompleted2);

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Wait));


		isCompletedMock->reset();
	}
	TEST_F(RouteTest, failTurningSectionsIncomplete)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSectionsIncomplete();
	}
	TEST_F(RouteTest, failWaitSwitches)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSwitchesIncomplete();

		statechart->raiseCompleted();

		disabled();


	}
	TEST_F(RouteTest, failWaitSignals)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSignalsIncomplete();

		statechart->raiseCompleted();

		disabled();


	}
	TEST_F(RouteTest, failWaitSections)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSectionsIncomplete();

		statechart->raiseCompleted();

		disabled();


	}
	TEST_F(RouteTest, timeoutWaitSwitches)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSwitchesIncomplete();

		runner->proceed_time(statechart->getEmergency());

		disabled();


	}
	TEST_F(RouteTest, timeoutWaitSignals)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSignalsIncomplete();

		runner->proceed_time(statechart->getEmergency());

		disabled();


	}
	TEST_F(RouteTest, timeoutWaitSections)
	{
		isCompletedMock = new IsCompletedMock();
		isCompletedMock->initializeBehavior();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failTurningSectionsIncomplete();

		runner->proceed_time(statechart->getEmergency());

		disabled();


	}
	TEST_F(RouteTest, timeoutTurningSwitches)
	{
		deactivateSectionsMock = new DeactivateSectionsMock();
		unlockSignalsMock = new UnlockSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		switches();

		runner->proceed_time(statechart->getSwitch_timeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

		EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getEmergency());

		disabled();


		deactivateSectionsMock->reset();
		unlockSignalsMock->reset();
	}
	TEST_F(RouteTest, timeoutTurningSignals)
	{
		deactivateSectionsMock = new DeactivateSectionsMock();
		unlockSignalsMock = new UnlockSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		turningSignals();

		runner->proceed_time(statechart->getSignal_timeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

		EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getEmergency());

		disabled();


		deactivateSectionsMock->reset();
		unlockSignalsMock->reset();
	}
	TEST_F(RouteTest, timeoutExtendingSignals)
	{
		deactivateSectionsMock = new DeactivateSectionsMock();
		unlockSignalsMock = new UnlockSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		extendingSignals();

		runner->proceed_time(statechart->getSignal_timeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

		EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getEmergency());

		disabled();


		deactivateSectionsMock->reset();
		unlockSignalsMock->reset();
	}
	TEST_F(RouteTest, timeoutTurningSections)
	{
		deactivateSectionsMock = new DeactivateSectionsMock();
		unlockSignalsMock = new UnlockSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		activateSectionsMock = new ActivateSectionsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSignalsMock = new TurnSignalsMock();
		resetTransactionMock = new ResetTransactionMock();
		turnSwitchesMock = new TurnSwitchesMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		sections();

		runner->proceed_time(statechart->getSection_timeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::RouteStatechart::State::main_region_Emergency_Shutdown));

		EXPECT_TRUE(deactivateSectionsMock->calledAtLeastOnce());

		EXPECT_TRUE(unlockSignalsMock->calledAtLeastOnce());

		runner->proceed_time(statechart->getEmergency());

		disabled();


		deactivateSectionsMock->reset();
		unlockSignalsMock->reset();
	}

}

