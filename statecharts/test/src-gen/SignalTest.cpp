/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */
#include <string>
#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "SignalStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void failState();
	void idle();
	void turning();
	void pending();
	void turnCompleted();
	void failSending();
	mrw::statechart::SignalStatechart * statechart;


	class PrepareMock
	{
		typedef bool (PrepareMock::*functiontype)();
	public:
		bool (PrepareMock::*prepareBehaviorDefault)();
		int callCount;

		bool prepare1()
		{
			return (true);
		}

		bool prepare2()
		{
			return (false);
		}

		bool prepareDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		bool calledAtLeast(const int times)
		{
			return (callCount >= times);
		}

		bool calledAtLeastOnce()
		{
			return (callCount > 0);
		}

		void prepare()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return prepareBehaviorDefault;
		}

		void setDefaultBehavior(bool (PrepareMock::*defaultBehavior)())
		{
			prepareBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&PrepareMock::prepareDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static PrepareMock * prepareMock;

	class SendMock
	{
		typedef void (SendMock::*functiontype)();
	public:
		void (SendMock::*sendBehaviorDefault)();
		int callCount;

		void send1()
		{
		}

		void sendDefault()
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

		void send()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return sendBehaviorDefault;
		}

		void setDefaultBehavior(void (SendMock::*defaultBehavior)())
		{
			sendBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&SendMock::sendDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static SendMock * sendMock;

	class HasSignalMock
	{
		typedef bool (HasSignalMock::*functiontype)();
	public:
		bool (HasSignalMock::*hasSignalBehaviorDefault)();

		bool hasSignal1()
		{
			return (true);
		}

		bool hasSignal2()
		{
			return (false);
		}

		bool hasSignalDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return hasSignalBehaviorDefault;
		}

		void setDefaultBehavior(bool (HasSignalMock::*defaultBehavior)())
		{
			hasSignalBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&HasSignalMock::hasSignalDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static HasSignalMock * hasSignalMock;

	class DumpMock
	{
		typedef void (DumpMock::*functiontype)();
	public:
		void (DumpMock::*dumpBehaviorDefault)();

		void dump1()
		{
		}

		void dumpDefault()
		{
		}

		functiontype getBehavior()
		{
			return dumpBehaviorDefault;
		}

		void setDefaultBehavior(void (DumpMock::*defaultBehavior)())
		{
			dumpBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&DumpMock::dumpDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static DumpMock * dumpMock;

	class MockDefault : public mrw::statechart::SignalStatechart::OperationCallback
	{
	public:
		bool hasSignal()
		{
			return (hasSignalMock->*(hasSignalMock->getBehavior()))();
		}
		bool prepare()
		{
			prepareMock->prepare();
			return (prepareMock->*(prepareMock->getBehavior()))();
		}
		void send()
		{
			sendMock->send();
			return (sendMock->*(sendMock->getBehavior()))();
		}
		void dump()
		{
			return (dumpMock->*(dumpMock->getBehavior()))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class SignalTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SignalStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			prepareMock = new PrepareMock();
			prepareMock->initializeBehavior();
			sendMock = new SendMock();
			sendMock->initializeBehavior();
			hasSignalMock = new HasSignalMock();
			hasSignalMock->initializeBehavior();
			dumpMock = new DumpMock();
			dumpMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete dumpMock;
			delete hasSignalMock;
			delete sendMock;
			delete prepareMock;
			delete statechart;
			delete runner;
		}
	};

	void failState()
	{
		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Fail));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getOFF()));

		EXPECT_TRUE(prepareMock->calledAtLeastOnce());

		EXPECT_TRUE(sendMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedFailed());

	}

	void idle()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

		prepareMock->setDefaultBehavior(&PrepareMock::prepare1);



	}
	TEST_F(SignalTest, idle)
	{
		idle();
	}
	TEST_F(SignalTest, failWithSignal)
	{
		idle();

		hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

		statechart->raiseFail();

		failState();

	}
	TEST_F(SignalTest, failWithoutSignal)
	{
		idle();

		hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal2);

		statechart->raiseFail();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

	}
	TEST_F(SignalTest, turnWithoutSignal)
	{
		idle();

		hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal2);

		statechart->raiseTurn(statechart->getGO());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

		EXPECT_FALSE(prepareMock->calledAtLeastOnce());

	}
	void turning()
	{
		idle();

		hasSignalMock->setDefaultBehavior(&HasSignalMock::hasSignal1);

		prepareMock->setDefaultBehavior(&PrepareMock::prepare1);

		statechart->raiseTurn(statechart->getGO());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Send));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getGO()));

		EXPECT_TRUE(prepareMock->calledAtLeastOnce());

		EXPECT_TRUE(sendMock->calledAtLeastOnce());

	}
	TEST_F(SignalTest, turning)
	{
		turning();
	}
	TEST_F(SignalTest, turningNoChange)
	{
		idle();

		prepareMock->setDefaultBehavior(&PrepareMock::prepare2);

		statechart->raiseTurn(statechart->getSTOP());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getSTOP()));

	}
	void pending()
	{
		turning();

		statechart->raiseQueued();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Turning_Turn_processing_Pending));

	}
	TEST_F(SignalTest, pending)
	{
		pending();
	}
	void turnCompleted()
	{
		turning();

		statechart->raiseOk();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE(statechart->isRaisedCompleted());

	}
	TEST_F(SignalTest, turnCompleted)
	{
		turnCompleted();
	}
	TEST_F(SignalTest, turningNoExtend)
	{
		turnCompleted();

		prepareMock->setDefaultBehavior(&PrepareMock::prepare2);

		statechart->raiseTurn(statechart->getGO());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE((statechart->getSymbol()) == (statechart->getGO()));

	}
	TEST_F(SignalTest, turnCompletedAfterPending)
	{
		pending();

		statechart->raiseOk();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

		EXPECT_TRUE(statechart->isRaisedCompleted());

	}
	void failSending()
	{
		turning();

		statechart->raiseFail();

		failState();

	}
	TEST_F(SignalTest, failSending)
	{
		failSending();
	}
	TEST_F(SignalTest, failPending)
	{
		pending();

		statechart->raiseFail();

		failState();

	}
	TEST_F(SignalTest, timeoutSending)
	{
		turning();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalTest, timeoutPending)
	{
		pending();

		runner->proceed_time(statechart->getTimeout());

		failState();

	}
	TEST_F(SignalTest, clearing)
	{
		failSending();

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SignalStatechart::State::main_region_Idle));

	}
	TEST_F(SignalTest, doExit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		idle();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		turning();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		pending();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		failSending();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

