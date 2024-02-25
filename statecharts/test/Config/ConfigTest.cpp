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
#include "ConfigStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void doStart();
	void firstController();
	void secondController();
	void lastController();
	void booted();
	void timeoutConfig();
	mrw::statechart::ConfigStatechart * statechart;

	sc::integer dvc_count_1 = 22;
	sc::integer dvc_count_2 = 11;

	class BootingMock
	{
		typedef void (BootingMock::*functiontype)();
	public:
		void (BootingMock::*bootingBehaviorDefault)();
		int callCount;

		void booting1()
		{
		}

		void bootingDefault()
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

		void booting()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return bootingBehaviorDefault;
		}

		void setDefaultBehavior(void (BootingMock::*defaultBehavior)())
		{
			bootingBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&BootingMock::bootingDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static BootingMock * bootingMock;

	class QuitMock
	{
		typedef void (QuitMock::*functiontype)();
	public:
		void (QuitMock::*quitBehaviorDefault)();
		int callCount;

		void quit1()
		{
		}

		void quitDefault()
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

		void quit()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return quitBehaviorDefault;
		}

		void setDefaultBehavior(void (QuitMock::*defaultBehavior)())
		{
			quitBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&QuitMock::quitDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static QuitMock * quitMock;

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

	class ConfigureMock
	{
		typedef sc::integer (ConfigureMock::*functiontype)();
		struct parameters
		{
			sc::integer idx;
			sc::integer (ConfigureMock::*behavior)();
			int callCount;
			inline bool operator==(const parameters & other)
			{
				return (this->idx == other.idx);
			}
		};
	public:
		std::list<ConfigureMock::parameters> mocks;
		std::list<ConfigureMock::parameters> paramCount;
		sc::integer (ConfigureMock::*configureBehaviorDefault)();
		int callCount;

		sc::integer configure1()
		{
			return (0);
		}

		sc::integer configure2()
		{
			return (dvc_count_1);
		}

		sc::integer configure3()
		{
			return (dvc_count_2);
		}

		sc::integer configureDefault()
		{
			sc::integer defaultValue = 0;
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

		void setConfigureBehavior(const sc::integer idx, sc::integer (ConfigureMock::*func)())
		{
			parameters p;
			p.idx = idx;
			p.behavior = func;

			std::list<ConfigureMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				mocks.erase(i);
			}
			mocks.push_back(p);
		}

		bool calledAtLeast(const int times, const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<ConfigureMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount >= times);
			}
			else
			{
				return false;
			}
		}

		bool calledAtLeastOnce(const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<ConfigureMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount > 0);
			}
			else
			{
				return false;
			}
		}

		void configure(const sc::integer idx)
		{
			++callCount;

			parameters p;
			p.idx = idx;

			std::list<ConfigureMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

		functiontype getBehavior(const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<ConfigureMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				return  i->behavior;
			}
			else
			{
				return configureBehaviorDefault;
			}
		}

		void setDefaultBehavior(sc::integer (ConfigureMock::*defaultBehavior)())
		{
			configureBehaviorDefault = defaultBehavior;
			mocks.clear();
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&ConfigureMock::configureDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
			paramCount.clear();
			mocks.clear();
		}
	};
	static ConfigureMock * configureMock;

	class HasMoreMock
	{
		typedef bool (HasMoreMock::*functiontype)();
		struct parameters
		{
			sc::integer idx;
			bool (HasMoreMock::*behavior)();
			int callCount;
			inline bool operator==(const parameters & other)
			{
				return (this->idx == other.idx);
			}
		};
	public:
		std::list<HasMoreMock::parameters> mocks;
		std::list<HasMoreMock::parameters> paramCount;
		bool (HasMoreMock::*hasMoreBehaviorDefault)();
		int callCount;

		bool hasMore1()
		{
			return (true);
		}

		bool hasMore2()
		{
			return (false);
		}

		bool hasMoreDefault()
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

		void setHasMoreBehavior(const sc::integer idx, bool (HasMoreMock::*func)())
		{
			parameters p;
			p.idx = idx;
			p.behavior = func;

			std::list<HasMoreMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				mocks.erase(i);
			}
			mocks.push_back(p);
		}

		bool calledAtLeast(const int times, const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<HasMoreMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount >= times);
			}
			else
			{
				return false;
			}
		}

		bool calledAtLeastOnce(const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<HasMoreMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount > 0);
			}
			else
			{
				return false;
			}
		}

		void hasMore(const sc::integer idx)
		{
			++callCount;

			parameters p;
			p.idx = idx;

			std::list<HasMoreMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

		functiontype getBehavior(const sc::integer idx)
		{
			parameters p;
			p.idx = idx;

			std::list<HasMoreMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
			if (i != mocks.end())
			{
				return  i->behavior;
			}
			else
			{
				return hasMoreBehaviorDefault;
			}
		}

		void setDefaultBehavior(bool (HasMoreMock::*defaultBehavior)())
		{
			hasMoreBehaviorDefault = defaultBehavior;
			mocks.clear();
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&HasMoreMock::hasMoreDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
			paramCount.clear();
			mocks.clear();
		}
	};
	static HasMoreMock * hasMoreMock;

	class MockDefault : public mrw::statechart::ConfigStatechart::OperationCallback
	{
	public:
		sc::integer configure(sc::integer idx)
		{
			configureMock->configure(idx);
			return (configureMock->*(configureMock->getBehavior(idx)))();
		}
		bool hasMore(sc::integer idx)
		{
			hasMoreMock->hasMore(idx);
			return (hasMoreMock->*(hasMoreMock->getBehavior(idx)))();
		}
		void booting()
		{
			bootingMock->booting();
			return (bootingMock->*(bootingMock->getBehavior()))();
		}
		void quit()
		{
			quitMock->quit();
			return (quitMock->*(quitMock->getBehavior()))();
		}
		void fail()
		{
			failMock->fail();
			return (failMock->*(failMock->getBehavior()))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class ConfigTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::ConfigStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			bootingMock = new BootingMock();
			bootingMock->initializeBehavior();
			quitMock = new QuitMock();
			quitMock->initializeBehavior();
			failMock = new FailMock();
			failMock->initializeBehavior();
			configureMock = new ConfigureMock();
			configureMock->initializeBehavior();
			hasMoreMock = new HasMoreMock();
			hasMoreMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
		}
		virtual void TearDown()
		{
			delete hasMoreMock;
			delete configureMock;
			delete failMock;
			delete quitMock;
			delete bootingMock;
			delete statechart;
			delete runner;
		}
	};


	void doStart()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Connect));




		configureMock->setDefaultBehavior(&ConfigureMock::configure1);

		hasMoreMock->setHasMoreBehavior(statechart->getIdx(), &HasMoreMock::hasMore1);

	}
	TEST_F(ConfigTest, doStart)
	{
		doStart();
	}
	TEST_F(ConfigTest, timeoutConnect)
	{
		doStart();

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());

	}
	void firstController()
	{
		doStart();

		hasMoreMock->setDefaultBehavior(&HasMoreMock::hasMore1);

		configureMock->setDefaultBehavior(&ConfigureMock::configure2);

		statechart->raiseConnected();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Configure));

		EXPECT_TRUE((statechart->getIdx()) == (0));

		EXPECT_TRUE((statechart->getMax()) == (dvc_count_1));

		EXPECT_TRUE(hasMoreMock->calledAtLeastOnce());

		EXPECT_TRUE(configureMock->calledAtLeastOnce());

	}
	TEST_F(ConfigTest, firstController)
	{
		firstController();
	}
	void secondController()
	{
		firstController();

		hasMoreMock->setDefaultBehavior(&HasMoreMock::hasMore1);

		configureMock->setDefaultBehavior(&ConfigureMock::configure3);

		runner->proceed_time(statechart->getWritetime());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Configure));

		EXPECT_TRUE((statechart->getIdx()) == (1));

		EXPECT_TRUE((statechart->getMax()) == (dvc_count_1));

		EXPECT_TRUE(hasMoreMock->calledAtLeastOnce());

		EXPECT_TRUE(configureMock->calledAtLeastOnce());

	}
	TEST_F(ConfigTest, secondController)
	{
		secondController();
	}
	void lastController()
	{
		secondController();

		hasMoreMock->setDefaultBehavior(&HasMoreMock::hasMore2);

		runner->proceed_time(statechart->getWritetime());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot));

		EXPECT_TRUE((statechart->getIdx()) == (2));

		EXPECT_TRUE(hasMoreMock->calledAtLeastOnce());

		EXPECT_TRUE(bootingMock->calledAtLeastOnce());

	}
	TEST_F(ConfigTest, lastController)
	{
		lastController();
	}
	void booted()
	{
		lastController();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Booted));

		EXPECT_TRUE((statechart->getIdx()) == (2));

		EXPECT_TRUE(quitMock->calledAtLeastOnce());

		EXPECT_FALSE(failMock->calledAtLeastOnce());

	}
	TEST_F(ConfigTest, booted)
	{
		booted();
	}
	void timeoutConfig()
	{
		lastController();

		runner->proceed_time(statechart->getResettime());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Wait_for_Boot));

		runner->proceed_time(statechart->getMax()*statechart->getFlashtime());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::ConfigStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getIdx()) == (2));

		EXPECT_FALSE(quitMock->calledAtLeastOnce());

		EXPECT_TRUE(failMock->calledAtLeastOnce());

	}
	TEST_F(ConfigTest, timeoutConfig)
	{
		timeoutConfig();
	}
	TEST_F(ConfigTest, doQuit)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		doStart();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		firstController();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		secondController();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		lastController();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		booted();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}
	TEST_F(ConfigTest, doFail)
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

		timeoutConfig();

		statechart->exit();

		EXPECT_TRUE(!statechart->isActive());

	}

}

