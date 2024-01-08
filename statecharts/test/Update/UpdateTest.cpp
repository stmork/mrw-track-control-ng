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
#include "UpdateStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace mrw
{
	namespace test
	{

		void doStart();
		void doReset();
		void doPing();
		void noController();
		void firstFlashRequest();
		void firstFlashRequestMismatch();
		void flashRequested();
		void hardwareMismatch();
		void lastFlashRequest();
		void firstCompletePage();
		void nextCompletePage();
		void lastCompletePage();
		void doFlashCheck();
		void booted();
		void failFlashCheck();
		mrw::statechart::UpdateStatechart * statechart;


		class InitMock
		{
			typedef void (InitMock::*functiontype)();
			struct parameters
			{
				sc::integer count;
				void (InitMock::*behavior)();
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->count == other.count);
				}
			};
		public:
			std::list<InitMock::parameters> mocks;
			std::list<InitMock::parameters> paramCount;
			void (InitMock::*initBehaviorDefault)();
			int callCount;

			void init1()
			{
			}

			void initDefault()
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

			void setInitBehavior(const sc::integer count, void (InitMock::*func)())
			{
				parameters p;
				p.count = count;
				p.behavior = func;

				std::list<InitMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					mocks.erase(i);
				}
				mocks.push_back(p);
			}

			bool calledAtLeast(const int times, const sc::integer count)
			{
				parameters p;
				p.count = count;

				std::list<InitMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const sc::integer count)
			{
				parameters p;
				p.count = count;

				std::list<InitMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void init(const sc::integer count)
			{
				++callCount;

				parameters p;
				p.count = count;

				std::list<InitMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

			functiontype getBehavior(const sc::integer count)
			{
				parameters p;
				p.count = count;

				std::list<InitMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					return  i->behavior;
				}
				else
				{
					return initBehaviorDefault;
				}
			}

			void setDefaultBehavior(void (InitMock::*defaultBehavior)())
			{
				initBehaviorDefault = defaultBehavior;
				mocks.clear();
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&InitMock::initDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
				paramCount.clear();
				mocks.clear();
			}
		};
		static InitMock * initMock;

		class PingMock
		{
			typedef void (PingMock::*functiontype)();
		public:
			void (PingMock::*pingBehaviorDefault)();
			int callCount;

			void ping1()
			{
			}

			void pingDefault()
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

			void ping()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return pingBehaviorDefault;
			}

			void setDefaultBehavior(void (PingMock::*defaultBehavior)())
			{
				pingBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&PingMock::pingDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static PingMock * pingMock;

		class BootMock
		{
			typedef void (BootMock::*functiontype)();
		public:
			void (BootMock::*bootBehaviorDefault)();
			int callCount;

			void boot1()
			{
			}

			void bootDefault()
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

			void boot()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return bootBehaviorDefault;
			}

			void setDefaultBehavior(void (BootMock::*defaultBehavior)())
			{
				bootBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&BootMock::bootDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static BootMock * bootMock;

		class FlashRequestMock
		{
			typedef void (FlashRequestMock::*functiontype)();
		public:
			void (FlashRequestMock::*flashRequestBehaviorDefault)();
			int callCount;

			void flashRequest1()
			{
			}

			void flashRequestDefault()
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

			void flashRequest()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return flashRequestBehaviorDefault;
			}

			void setDefaultBehavior(void (FlashRequestMock::*defaultBehavior)())
			{
				flashRequestBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&FlashRequestMock::flashRequestDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static FlashRequestMock * flashRequestMock;

		class FlashCompletePageMock
		{
			typedef void (FlashCompletePageMock::*functiontype)();
		public:
			void (FlashCompletePageMock::*flashCompletePageBehaviorDefault)();
			int callCount;

			void flashCompletePage1()
			{
			}

			void flashCompletePageDefault()
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

			void flashCompletePage()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return flashCompletePageBehaviorDefault;
			}

			void setDefaultBehavior(void (FlashCompletePageMock::*defaultBehavior)())
			{
				flashCompletePageBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&FlashCompletePageMock::flashCompletePageDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static FlashCompletePageMock * flashCompletePageMock;

		class FlashRestPageMock
		{
			typedef void (FlashRestPageMock::*functiontype)();
		public:
			void (FlashRestPageMock::*flashRestPageBehaviorDefault)();
			int callCount;

			void flashRestPage1()
			{
			}

			void flashRestPageDefault()
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

			void flashRestPage()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return flashRestPageBehaviorDefault;
			}

			void setDefaultBehavior(void (FlashRestPageMock::*defaultBehavior)())
			{
				flashRestPageBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&FlashRestPageMock::flashRestPageDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static FlashRestPageMock * flashRestPageMock;

		class FlashCheckMock
		{
			typedef void (FlashCheckMock::*functiontype)();
		public:
			void (FlashCheckMock::*flashCheckBehaviorDefault)();
			int callCount;

			void flashCheck1()
			{
			}

			void flashCheckDefault()
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

			void flashCheck()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return flashCheckBehaviorDefault;
			}

			void setDefaultBehavior(void (FlashCheckMock::*defaultBehavior)())
			{
				flashCheckBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&FlashCheckMock::flashCheckDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		static FlashCheckMock * flashCheckMock;

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
			void init(sc::integer count)
			{
				initMock->init(count);
				return (initMock->*(initMock->getBehavior(count)))();
			}
			void ping()
			{
				pingMock->ping();
				return (pingMock->*(pingMock->getBehavior()))();
			}
			void boot()
			{
				bootMock->boot();
				return (bootMock->*(bootMock->getBehavior()))();
			}
			void flashRequest()
			{
				flashRequestMock->flashRequest();
				return (flashRequestMock->*(flashRequestMock->getBehavior()))();
			}
			void flashCompletePage()
			{
				flashCompletePageMock->flashCompletePage();
				return (flashCompletePageMock->*(flashCompletePageMock->getBehavior()))();
			}
			void flashRestPage()
			{
				flashRestPageMock->flashRestPage();
				return (flashRestPageMock->*(flashRestPageMock->getBehavior()))();
			}
			void flashCheck()
			{
				flashCheckMock->flashCheck();
				return (flashCheckMock->*(flashCheckMock->getBehavior()))();
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
			MockDefault defaultMock;
			virtual void SetUp()
			{
				statechart = new mrw::statechart::UpdateStatechart();
				size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
				runner = new TimedSctUnitRunner(
					maximalParallelTimeEvents
				);
				statechart->setTimerService(runner);
				initMock = new InitMock();
				initMock->initializeBehavior();
				pingMock = new PingMock();
				pingMock->initializeBehavior();
				bootMock = new BootMock();
				bootMock->initializeBehavior();
				flashRequestMock = new FlashRequestMock();
				flashRequestMock->initializeBehavior();
				flashCompletePageMock = new FlashCompletePageMock();
				flashCompletePageMock->initializeBehavior();
				flashRestPageMock = new FlashRestPageMock();
				flashRestPageMock->initializeBehavior();
				flashCheckMock = new FlashCheckMock();
				flashCheckMock->initializeBehavior();
				hasControllerMock = new HasControllerMock();
				hasControllerMock->initializeBehavior();
				hasPagesMock = new HasPagesMock();
				hasPagesMock->initializeBehavior();
				failMock = new FailMock();
				quitMock = new QuitMock();
				statechart->setOperationCallback(&defaultMock);
			}
			virtual void TearDown()
			{
				delete quitMock;
				delete failMock;
				delete hasPagesMock;
				delete hasControllerMock;
				delete flashCheckMock;
				delete flashRestPageMock;
				delete flashCompletePageMock;
				delete flashRequestMock;
				delete bootMock;
				delete pingMock;
				delete initMock;
				delete statechart;
				delete runner;
			}
		};


		void doStart()
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			EXPECT_TRUE(!statechart->isFinal());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Wait_for_Connect));








			hasControllerMock->setDefaultBehavior(&HasControllerMock::hasController1);

			hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

		}
		TEST_F(UpdateTest, doStart)
		{
			doStart();
		}
		TEST_F(UpdateTest, timeoutConnect)
		{
			doStart();

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (7));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		void doReset()
		{
			doStart();

			statechart->raiseConnected();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Reset));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, doReset)
		{
			doReset();
		}
		void doPing()
		{
			doReset();

			runner->proceed_time(statechart->getDelay_reset());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Ping));

			EXPECT_TRUE(pingMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, doPing)
		{
			doPing();
		}
		void noController()
		{
			doPing();

			hasControllerMock->setDefaultBehavior(&HasControllerMock::hasController2);

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (1));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, noController)
		{
			noController();
		}
		void firstFlashRequest()
		{
			doPing();

			hasControllerMock->setDefaultBehavior(&HasControllerMock::hasController1);

			runner->proceed_time(statechart->getTimeout());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Request));

			EXPECT_TRUE(initMock->calledAtLeastOnce());

			EXPECT_TRUE(flashRequestMock->calledAtLeastOnce());

			EXPECT_TRUE((statechart->getCount()) == (0));

		}
		TEST_F(UpdateTest, firstFlashRequest)
		{
			firstFlashRequest();
		}
		void firstFlashRequestMismatch()
		{
			firstFlashRequest();

			hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

			statechart->raiseMismatch();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (8));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, firstFlashRequestMismatch)
		{
			firstFlashRequestMismatch();
		}
		void flashRequested()
		{
			firstFlashRequest();

			hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

			statechart->raiseComplete();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Test_Hardware_Mismatch));

			EXPECT_TRUE(flashRequestMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, flashRequested)
		{
			flashRequested();
		}
		void hardwareMismatch()
		{
			flashRequested();

			statechart->raiseMismatch();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (8));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, hardwareMismatch)
		{
			hardwareMismatch();
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

		}
		TEST_F(UpdateTest, lastFlashRequest)
		{
			lastFlashRequest();
		}
		TEST_F(UpdateTest, noCompletePage)
		{
			lastFlashRequest();

			hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages2);

			runner->proceed_time(statechart->getDelay_flash_request());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (6));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		void firstCompletePage()
		{
			flashRequested();

			hasPagesMock->setDefaultBehavior(&HasPagesMock::hasPages1);

			runner->proceed_time(statechart->getDelay_flash_request());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Complete_Page));

			EXPECT_TRUE(hasPagesMock->calledAtLeastOnce());

			EXPECT_TRUE(flashCompletePageMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, firstCompletePage)
		{
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

		}
		TEST_F(UpdateTest, nextCompletePage)
		{
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

		}
		TEST_F(UpdateTest, lastCompletePage)
		{
			lastCompletePage();
		}
		void doFlashCheck()
		{
			lastCompletePage();

			runner->proceed_time(statechart->getDelay_flash_page());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Flash_Check));

			EXPECT_TRUE(initMock->calledAtLeastOnce());

			EXPECT_TRUE(flashCheckMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, doFlashCheck)
		{
			doFlashCheck();
		}
		void booted()
		{
			doFlashCheck();

			statechart->raiseComplete();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Booted));

			EXPECT_TRUE(quitMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, booted)
		{
			booted();
		}
		void failFlashCheck()
		{
			doFlashCheck();

			statechart->raiseFailed();

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (3));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, failFlashCheck)
		{
			failFlashCheck();
		}
		TEST_F(UpdateTest, timeoutFlashCheck)
		{
			doFlashCheck();

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Leave_Bootloader));

			EXPECT_TRUE((statechart->getError()) == (4));

			EXPECT_TRUE(bootMock->calledAtLeastOnce());

			runner->proceed_time(statechart->getDelay_boot());

			EXPECT_TRUE(statechart->isStateActive(mrw::statechart::UpdateStatechart::State::main_region_Failed));

			EXPECT_TRUE(failMock->calledAtLeastOnce());

		}
		TEST_F(UpdateTest, doExit)
		{
			statechart->enter();

			EXPECT_TRUE(statechart->isActive());

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doStart();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doReset();

			statechart->exit();

			doPing();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			noController();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			firstFlashRequest();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			firstFlashRequestMismatch();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			flashRequested();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			hardwareMismatch();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			lastCompletePage();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			doFlashCheck();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			booted();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

			failFlashCheck();

			statechart->exit();

			EXPECT_TRUE(!statechart->isActive());

		}

	}
}

