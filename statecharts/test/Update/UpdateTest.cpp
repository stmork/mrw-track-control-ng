/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
#include "gtest/gtest.h"
#include "UpdateStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	void doPing();
	void doReset();
	void doWait();
	void firstFlashRequest();
	void lastFlashRequest();
	void firstCompletePage();
	void nextCompletePage();
	void lastCompletePage();
	void doFlashCheck();
	void okFlashCheck();
	mrw::statechart::UpdateStatechart * statechart;


	class PingMock
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

		void ping()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static PingMock * pingMock;

	class HasControllerMock
	{
		typedef bool (HasControllerMock::*functiontype)();
	public:
		bool (HasControllerMock::*hasControllerBehaviorDefault)();

		bool hasController1()
		{
			return (true);
		}

		bool hasController2()
		{
			return (false);
		}

		bool hasControllerDefault()
		{
			bool defaultValue = false;
			return (defaultValue);
		}

		functiontype getBehavior()
		{
			return hasControllerBehaviorDefault;
		}

		void setDefaultBehavior(bool (HasControllerMock::*defaultBehavior)())
		{
			hasControllerBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&HasControllerMock::hasControllerDefault);
		}

		void reset()
		{
			initializeBehavior();
		}
	};
	static HasControllerMock * hasControllerMock;

	class InitMock
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

		void init()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static InitMock * initMock;

	class BootMock
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

		void boot()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static BootMock * bootMock;

	class FailMock
	{
		struct parameters
		{
			sc::integer code;
			int callCount;
			inline bool operator==(const parameters & other)
			{
				return (this->code == other.code);
			}
		};
	public:
		std::list<FailMock::parameters> paramCount;
		int callCount;

		bool calledAtLeast(const int times)
		{
			return (callCount >= times);
		}

		bool calledAtLeastOnce()
		{
			return (callCount > 0);
		}

		bool calledAtLeast(const int times, const sc::integer code)
		{
			parameters p;
			p.code = code;

			std::list<FailMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount >= times);
			}
			else
			{
				return false;
			}
		}

		bool calledAtLeastOnce(const sc::integer code)
		{
			parameters p;
			p.code = code;

			std::list<FailMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
			if (i != paramCount.end())
			{
				return (i->callCount > 0);
			}
			else
			{
				return false;
			}
		}

		void fail(const sc::integer code)
		{
			++callCount;

			parameters p;
			p.code = code;

			std::list<FailMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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
	static FailMock * failMock;

	class FlashRequestMock
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

		void flashRequest()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FlashRequestMock * flashRequestMock;

	class HasPagesMock
	{
		typedef bool (HasPagesMock::*functiontype)();
	public:
		bool (HasPagesMock::*hasPagesBehaviorDefault)();
		int callCount;

		bool hasPages1()
		{
			return (true);
		}

		bool hasPages2()
		{
			return (false);
		}

		bool hasPagesDefault()
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

		void hasPages()
		{
			++callCount;
		}

		functiontype getBehavior()
		{
			return hasPagesBehaviorDefault;
		}

		void setDefaultBehavior(bool (HasPagesMock::*defaultBehavior)())
		{
			hasPagesBehaviorDefault = defaultBehavior;
		}

		void initializeBehavior()
		{
			setDefaultBehavior(&HasPagesMock::hasPagesDefault);
		}

		void reset()
		{
			initializeBehavior();
			callCount = 0;
		}
	};
	static HasPagesMock * hasPagesMock;

	class FlashCompletePageMock
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

		void flashCompletePage()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FlashCompletePageMock * flashCompletePageMock;

	class FlashRestPageMock
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

		void flashRestPage()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FlashRestPageMock * flashRestPageMock;

	class FlashCheckMock
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

		void flashCheck()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FlashCheckMock * flashCheckMock;

	class QuitMock
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

		void quit()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static QuitMock * quitMock;

	class MockDefault : public mrw::statechart::UpdateStatechart::OperationCallback
	{
	public:
		void init()
		{
			initMock->init();
		}
		void ping()
		{
			pingMock->ping();
		}
		void boot()
		{
			bootMock->boot();
		}
		void flashRequest()
		{
			flashRequestMock->flashRequest();
		}
		void flashCompletePage()
		{
			flashCompletePageMock->flashCompletePage();
		}
		void flashRestPage()
		{
			flashRestPageMock->flashRestPage();
		}
		void flashCheck()
		{
			flashCheckMock->flashCheck();
		}
		void quit()
		{
			quitMock->quit();
		}
		void fail(sc::integer code)
		{
			failMock->fail(code);
		}
		bool hasController()
		{
			return (hasControllerMock->*(hasControllerMock->getBehavior()))();
		}
		bool hasPages()
		{
			hasPagesMock->hasPages();
			return (hasPagesMock->*(hasPagesMock->getBehavior()))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class UpdateTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::UpdateStatechart();
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


	void doPing()
	{
		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Ping));

		EXPECT_TRUE(pingMock->calledAtLeastOnce());


		pingMock->reset();
	}
	TEST_F(UpdateTest, doPing)
	{
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doPing();
	}
	void doReset()
	{
		doPing();

		hasControllerMock->setDefaultBehavior(&HasControllerMock::hasController1);

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Reset));

		EXPECT_TRUE(initMock->calledAtLeastOnce());

		EXPECT_TRUE(bootMock->calledAtLeastOnce());


		hasControllerMock->reset();
		initMock->reset();
		bootMock->reset();
	}
	TEST_F(UpdateTest, doReset)
	{
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doReset();
	}
	TEST_F(UpdateTest, failPing)
	{
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		failMock = new FailMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doPing();

		hasControllerMock->setDefaultBehavior(&HasControllerMock::hasController2);

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getError()) == (1));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		hasControllerMock->reset();
		failMock->reset();
	}
	void doWait()
	{
		doReset();

		statechart->raiseComplete();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Wait_Bootloader));

		EXPECT_TRUE((statechart->getCount()) == (0));


	}
	TEST_F(UpdateTest, doWait)
	{
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doWait();
	}
	TEST_F(UpdateTest, failWait)
	{
		failMock = new FailMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doReset();

		runner->proceed_time(statechart->getDelay_boot());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getError()) == (2));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}
	void firstFlashRequest()
	{
		doWait();

		runner->proceed_time(statechart->getDelay_reset());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Request));

		EXPECT_TRUE(initMock->calledAtLeastOnce());

		EXPECT_TRUE(flashRequestMock->calledAtLeastOnce());

		EXPECT_TRUE((statechart->getCount()) == (0));


		initMock->reset();
		flashRequestMock->reset();
	}
	TEST_F(UpdateTest, firstFlashRequest)
	{
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		firstFlashRequest();
	}
	TEST_F(UpdateTest, flashRequested)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		firstFlashRequest();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

		statechart->raiseComplete();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page));


		hasPagesMock->reset();
	}
	void lastFlashRequest()
	{
		firstFlashRequest();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

		sc::integer i = 0;

		while ((i) < (statechart->getRetry()))
		{
			EXPECT_TRUE((i) == (statechart->getCount()));
			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Request));
			runner->proceed_time(statechart->getDelay_flash_request());
			i++;
		}

		EXPECT_TRUE((statechart->getCount()) == (statechart->getRetry()));


		hasPagesMock->reset();
	}
	TEST_F(UpdateTest, lastFlashRequest)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		lastFlashRequest();
	}
	TEST_F(UpdateTest, noCompletePage)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		lastFlashRequest();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages2);

		runner->proceed_time(statechart->getDelay_flash_request());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest));

		EXPECT_FALSE(flashCompletePageMock->calledAtLeastOnce());

		EXPECT_TRUE(hasPagesMock->calledAtLeastOnce());

		EXPECT_TRUE(flashRestPageMock->calledAtLeastOnce());


		hasPagesMock->reset();
		flashCompletePageMock->reset();
		flashRestPageMock->reset();
	}
	void firstCompletePage()
	{
		lastFlashRequest();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

		runner->proceed_time(statechart->getDelay_flash_request());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page));

		EXPECT_TRUE(hasPagesMock->calledAtLeastOnce());

		EXPECT_TRUE(flashCompletePageMock->calledAtLeastOnce());


		hasPagesMock->reset();
		flashCompletePageMock->reset();
	}
	TEST_F(UpdateTest, firstCompletePage)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		firstCompletePage();
	}
	void nextCompletePage()
	{
		firstCompletePage();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

		runner->proceed_time(statechart->getDelay_flash_page());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page));

		EXPECT_TRUE(hasPagesMock->calledAtLeastOnce());

		EXPECT_TRUE(flashCompletePageMock->calledAtLeastOnce());


		hasPagesMock->reset();
		flashCompletePageMock->reset();
	}
	TEST_F(UpdateTest, nextCompletePage)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		nextCompletePage();
	}
	void lastCompletePage()
	{
		nextCompletePage();

		hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages2);

		runner->proceed_time(statechart->getDelay_flash_page());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Rest));

		EXPECT_TRUE(hasPagesMock->calledAtLeastOnce());

		EXPECT_TRUE(flashRestPageMock->calledAtLeastOnce());


		hasPagesMock->reset();
		flashRestPageMock->reset();
	}
	TEST_F(UpdateTest, lastCompletePage)
	{
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		lastCompletePage();
	}
	void doFlashCheck()
	{
		lastCompletePage();

		runner->proceed_time(statechart->getDelay_flash_page());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Check));

		EXPECT_TRUE(initMock->calledAtLeastOnce());

		EXPECT_TRUE(flashCheckMock->calledAtLeastOnce());


		initMock->reset();
		flashCheckMock->reset();
	}
	TEST_F(UpdateTest, doFlashCheck)
	{
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doFlashCheck();
	}
	void okFlashCheck()
	{
		doFlashCheck();

		statechart->raiseComplete();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Successful));

		EXPECT_TRUE((statechart->getError()) == (0));


	}
	TEST_F(UpdateTest, okFlashCheck)
	{
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okFlashCheck();
	}
	TEST_F(UpdateTest, failFlashCheck)
	{
		failMock = new FailMock();
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doFlashCheck();

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getError()) == (3));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}
	TEST_F(UpdateTest, timeoutFlashCheck)
	{
		failMock = new FailMock();
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		doFlashCheck();

		runner->proceed_time(statechart->getDelay_boot());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getError()) == (4));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}
	TEST_F(UpdateTest, booted)
	{
		quitMock = new QuitMock();
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okFlashCheck();

		statechart->raiseComplete();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Booted));

		EXPECT_TRUE(quitMock->calledAtLeastOnce());


		quitMock->reset();
	}
	TEST_F(UpdateTest, notBooted)
	{
		failMock = new FailMock();
		initMock = new InitMock();
		flashCheckMock = new FlashCheckMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashRestPageMock = new FlashRestPageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		flashCompletePageMock = new FlashCompletePageMock();
		hasPagesMock = new HasPagesMock();
		hasPagesMock->initializeBehavior();
		initMock = new InitMock();
		flashRequestMock = new FlashRequestMock();
		hasControllerMock = new HasControllerMock();
		hasControllerMock->initializeBehavior();
		initMock = new InitMock();
		bootMock = new BootMock();
		pingMock = new PingMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		okFlashCheck();

		runner->proceed_time(statechart->getDelay_boot());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

		EXPECT_TRUE((statechart->getError()) == (5));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}

}

