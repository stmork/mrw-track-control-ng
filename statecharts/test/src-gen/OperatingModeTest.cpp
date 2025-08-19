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
#include "OperatingModeStatechart.h"
#include "sc_runner_timed.h"
#include "sc_types.h"

#define SC_UNUSED(P) (void)P

namespace
{

	class OperatingModeTest : public ::testing::Test
	{
	public:
		void wait();
		void blank();
		void initial();
		void failAfterStart();
		void doOperating();
		void initWhileOperatingWithRoutes();
		void doEdit();
		void doManual();
		void doQuitManual();
		void doQuitWithRoute();

	protected:
		mrw::statechart::OperatingModeStatechart * statechart;


	public:
		class HasActiveRoutesMock
		{
			typedef bool (HasActiveRoutesMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			bool (HasActiveRoutesMock::*hasActiveRoutesBehaviorDefault)();
			int callCount;

			HasActiveRoutesMock(OperatingModeTest * owner) :
				owner(owner),
				hasActiveRoutesBehaviorDefault(0),
				callCount(0)
			{}


			bool hasActiveRoutes1()
			{
				return (false);
			}

			bool hasActiveRoutes2()
			{
				return (true);
			}

			bool hasActiveRoutesDefault()
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

			void hasActiveRoutes()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return hasActiveRoutesBehaviorDefault;
			}

			void setDefaultBehavior(bool (HasActiveRoutesMock::*defaultBehavior)())
			{
				hasActiveRoutesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutesDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		HasActiveRoutesMock * hasActiveRoutesMock;

		class IsManualValidMock
		{
			typedef bool (IsManualValidMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			bool (IsManualValidMock::*isManualValidBehaviorDefault)();

			IsManualValidMock(OperatingModeTest * owner) :
				owner(owner),
				isManualValidBehaviorDefault(0)
			{}


			bool isManualValid1()
			{
				return (true);
			}

			bool isManualValidDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return isManualValidBehaviorDefault;
			}

			void setDefaultBehavior(bool (IsManualValidMock::*defaultBehavior)())
			{
				isManualValidBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&IsManualValidMock::isManualValidDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		IsManualValidMock * isManualValidMock;

		class DisableRoutesMock
		{
			typedef void (DisableRoutesMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			void (DisableRoutesMock::*disableRoutesBehaviorDefault)();
			int callCount;

			DisableRoutesMock(OperatingModeTest * owner) :
				owner(owner),
				disableRoutesBehaviorDefault(0),
				callCount(0)
			{}


			void disableRoutes1()
			{
			}

			void disableRoutesDefault()
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

			void disableRoutes()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return disableRoutesBehaviorDefault;
			}

			void setDefaultBehavior(void (DisableRoutesMock::*defaultBehavior)())
			{
				disableRoutesBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&DisableRoutesMock::disableRoutesDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		DisableRoutesMock * disableRoutesMock;

		class ResetTransactionMock
		{
			typedef void (ResetTransactionMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			void (ResetTransactionMock::*resetTransactionBehaviorDefault)();
			int callCount;

			ResetTransactionMock(OperatingModeTest * owner) :
				owner(owner),
				resetTransactionBehaviorDefault(0),
				callCount(0)
			{}


			void resetTransaction1()
			{
			}

			void resetTransactionDefault()
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

			void resetTransaction()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return resetTransactionBehaviorDefault;
			}

			void setDefaultBehavior(void (ResetTransactionMock::*defaultBehavior)())
			{
				resetTransactionBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ResetTransactionMock::resetTransactionDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		ResetTransactionMock * resetTransactionMock;

		class ActivateManualMock
		{
			typedef void (ActivateManualMock::*functiontype)();
			struct parameters
			{
				bool active;
				void (ActivateManualMock::*behavior)();
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->active == other.active);
				}
			};
		public:
			OperatingModeTest * owner;
			std::list<ActivateManualMock::parameters> mocks;
			std::list<ActivateManualMock::parameters> paramCount;
			void (ActivateManualMock::*activateManualBehaviorDefault)();
			int callCount;

			ActivateManualMock(OperatingModeTest * owner) :
				owner(owner),
				activateManualBehaviorDefault(0),
				callCount(0)
			{}


			void activateManual1()
			{
			}

			void activateManualDefault()
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

			void setActivateManualBehavior(const bool active, void (ActivateManualMock::*func)())
			{
				parameters p;
				p.active = active;
				p.behavior = func;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					mocks.erase(i);
				}
				mocks.push_back(p);
			}

			bool calledAtLeast(const int times, const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void activateManual(const bool active)
			{
				++callCount;

				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

			functiontype getBehavior(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ActivateManualMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					return  i->behavior;
				}
				else
				{
					return activateManualBehaviorDefault;
				}
			}

			void setDefaultBehavior(void (ActivateManualMock::*defaultBehavior)())
			{
				activateManualBehaviorDefault = defaultBehavior;
				mocks.clear();
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ActivateManualMock::activateManualDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
				paramCount.clear();
				mocks.clear();
			}
		};
		ActivateManualMock * activateManualMock;

		class KeepAliveMock
		{
			typedef void (KeepAliveMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			void (KeepAliveMock::*keepAliveBehaviorDefault)();
			int callCount;

			KeepAliveMock(OperatingModeTest * owner) :
				owner(owner),
				keepAliveBehaviorDefault(0),
				callCount(0)
			{}


			void keepAlive1()
			{
			}

			void keepAliveDefault()
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

			void keepAlive()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return keepAliveBehaviorDefault;
			}

			void setDefaultBehavior(void (KeepAliveMock::*defaultBehavior)())
			{
				keepAliveBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&KeepAliveMock::keepAliveDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		KeepAliveMock * keepAliveMock;

		class ScreenResetBlankMock
		{
			typedef void (ScreenResetBlankMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			void (ScreenResetBlankMock::*screenResetBlankBehaviorDefault)();
			int callCount;

			ScreenResetBlankMock(OperatingModeTest * owner) :
				owner(owner),
				screenResetBlankBehaviorDefault(0),
				callCount(0)
			{}


			void resetBlank1()
			{
			}

			void resetBlankDefault()
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

			void resetBlank()
			{
				++callCount;
			}

			functiontype getBehavior()
			{
				return screenResetBlankBehaviorDefault;
			}

			void setDefaultBehavior(void (ScreenResetBlankMock::*defaultBehavior)())
			{
				screenResetBlankBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ScreenResetBlankMock::resetBlankDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
			}
		};
		ScreenResetBlankMock * screenResetBlankMock;

		class ScreenBlankMock
		{
			typedef void (ScreenBlankMock::*functiontype)();
			struct parameters
			{
				bool active;
				void (ScreenBlankMock::*behavior)();
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->active == other.active);
				}
			};
		public:
			OperatingModeTest * owner;
			std::list<ScreenBlankMock::parameters> mocks;
			std::list<ScreenBlankMock::parameters> paramCount;
			void (ScreenBlankMock::*screenBlankBehaviorDefault)();
			int callCount;

			ScreenBlankMock(OperatingModeTest * owner) :
				owner(owner),
				screenBlankBehaviorDefault(0),
				callCount(0)
			{}


			void blank1()
			{
			}

			void blankDefault()
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

			void setBlankBehavior(const bool active, void (ScreenBlankMock::*func)())
			{
				parameters p;
				p.active = active;
				p.behavior = func;

				std::list<ScreenBlankMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					mocks.erase(i);
				}
				mocks.push_back(p);
			}

			bool calledAtLeast(const int times, const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ScreenBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ScreenBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void blank(const bool active)
			{
				++callCount;

				parameters p;
				p.active = active;

				std::list<ScreenBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

			functiontype getBehavior(const bool active)
			{
				parameters p;
				p.active = active;

				std::list<ScreenBlankMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					return  i->behavior;
				}
				else
				{
					return screenBlankBehaviorDefault;
				}
			}

			void setDefaultBehavior(void (ScreenBlankMock::*defaultBehavior)())
			{
				screenBlankBehaviorDefault = defaultBehavior;
				mocks.clear();
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ScreenBlankMock::blankDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
				paramCount.clear();
				mocks.clear();
			}
		};
		ScreenBlankMock * screenBlankMock;

		class ScreenAutoBlankMock
		{
			typedef void (ScreenAutoBlankMock::*functiontype)();
			struct parameters
			{
				bool enable;
				void (ScreenAutoBlankMock::*behavior)();
				int callCount;
				inline bool operator==(const parameters & other)
				{
					return (this->enable == other.enable);
				}
			};
		public:
			OperatingModeTest * owner;
			std::list<ScreenAutoBlankMock::parameters> mocks;
			std::list<ScreenAutoBlankMock::parameters> paramCount;
			void (ScreenAutoBlankMock::*screenAutoBlankBehaviorDefault)();
			int callCount;

			ScreenAutoBlankMock(OperatingModeTest * owner) :
				owner(owner),
				screenAutoBlankBehaviorDefault(0),
				callCount(0)
			{}


			void autoBlank1()
			{
			}

			void autoBlankDefault()
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

			void setAutoBlankBehavior(const bool enable, void (ScreenAutoBlankMock::*func)())
			{
				parameters p;
				p.enable = enable;
				p.behavior = func;

				std::list<ScreenAutoBlankMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					mocks.erase(i);
				}
				mocks.push_back(p);
			}

			bool calledAtLeast(const int times, const bool enable)
			{
				parameters p;
				p.enable = enable;

				std::list<ScreenAutoBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount >= times);
				}
				else
				{
					return false;
				}
			}

			bool calledAtLeastOnce(const bool enable)
			{
				parameters p;
				p.enable = enable;

				std::list<ScreenAutoBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
				if (i != paramCount.end())
				{
					return (i->callCount > 0);
				}
				else
				{
					return false;
				}
			}

			void autoBlank(const bool enable)
			{
				++callCount;

				parameters p;
				p.enable = enable;

				std::list<ScreenAutoBlankMock::parameters>::iterator i = std::find(paramCount.begin(), paramCount.end(), p);
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

			functiontype getBehavior(const bool enable)
			{
				parameters p;
				p.enable = enable;

				std::list<ScreenAutoBlankMock::parameters>::iterator i = std::find(mocks.begin(), mocks.end(), p);
				if (i != mocks.end())
				{
					return  i->behavior;
				}
				else
				{
					return screenAutoBlankBehaviorDefault;
				}
			}

			void setDefaultBehavior(void (ScreenAutoBlankMock::*defaultBehavior)())
			{
				screenAutoBlankBehaviorDefault = defaultBehavior;
				mocks.clear();
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&ScreenAutoBlankMock::autoBlankDefault);
			}

			void reset()
			{
				initializeBehavior();
				callCount = 0;
				paramCount.clear();
				mocks.clear();
			}
		};
		ScreenAutoBlankMock * screenAutoBlankMock;

		class CanConnectBusMock
		{
		public:
			OperatingModeTest * owner;
			int callCount;

			CanConnectBusMock(OperatingModeTest * owner) :
				owner(owner),
				callCount(0)
			{}


			bool calledAtLeast(const int times)
			{
				return (callCount >= times);
			}

			bool calledAtLeastOnce()
			{
				return (callCount > 0);
			}

			void connectBus()
			{
				++callCount;
			}
			void reset()
			{
				callCount = 0;
			}
		};
		CanConnectBusMock * canConnectBusMock;

		class CanIsConnectedMock
		{
			typedef bool (CanIsConnectedMock::*functiontype)();
		public:
			OperatingModeTest * owner;
			bool (CanIsConnectedMock::*canIsConnectedBehaviorDefault)();

			CanIsConnectedMock(OperatingModeTest * owner) :
				owner(owner),
				canIsConnectedBehaviorDefault(0)
			{}


			bool isConnected1()
			{
				return (true);
			}

			bool isConnected2()
			{
				return (false);
			}

			bool isConnectedDefault()
			{
				bool defaultValue = false;
				return (defaultValue);
			}

			functiontype getBehavior()
			{
				return canIsConnectedBehaviorDefault;
			}

			void setDefaultBehavior(bool (CanIsConnectedMock::*defaultBehavior)())
			{
				canIsConnectedBehaviorDefault = defaultBehavior;
			}

			void initializeBehavior()
			{
				setDefaultBehavior(&CanIsConnectedMock::isConnectedDefault);
			}

			void reset()
			{
				initializeBehavior();
			}
		};
		CanIsConnectedMock * canIsConnectedMock;

		class MockDefault : public mrw::statechart::OperatingModeStatechart::OperationCallback
		{
		public:
			OperatingModeTest * owner;
			MockDefault(OperatingModeTest * owner) : owner(owner) {}
			void keepAlive()
			{
				owner->keepAliveMock->keepAlive();
				return (owner->keepAliveMock->*(owner->keepAliveMock->getBehavior()))();
			}
			void resetTransaction()
			{
				owner->resetTransactionMock->resetTransaction();
				return (owner->resetTransactionMock->*(owner->resetTransactionMock->getBehavior()))();
			}
			bool isManualValid()
			{
				return (owner->isManualValidMock->*(owner->isManualValidMock->getBehavior()))();
			}
			bool hasActiveRoutes()
			{
				owner->hasActiveRoutesMock->hasActiveRoutes();
				return (owner->hasActiveRoutesMock->*(owner->hasActiveRoutesMock->getBehavior()))();
			}
			void disableRoutes()
			{
				owner->disableRoutesMock->disableRoutes();
				return (owner->disableRoutesMock->*(owner->disableRoutesMock->getBehavior()))();
			}
			void activateManual(bool active)
			{
				owner->activateManualMock->activateManual(active);
				return (owner->activateManualMock->*(owner->activateManualMock->getBehavior(active)))();
			}
		};
		class MockCan : public mrw::statechart::OperatingModeStatechart::Can::OperationCallback
		{
		public:
			OperatingModeTest * owner;
			MockCan(OperatingModeTest * owner) : owner(owner) {}
			void connectBus()
			{
				owner->canConnectBusMock->connectBus();
			}
			bool isConnected()
			{
				return (owner->canIsConnectedMock->*(owner->canIsConnectedMock->getBehavior()))();
			}
		};
		class MockScreen : public mrw::statechart::OperatingModeStatechart::Screen::OperationCallback
		{
		public:
			OperatingModeTest * owner;
			MockScreen(OperatingModeTest * owner) : owner(owner) {}
			void resetBlank()
			{
				owner->screenResetBlankMock->resetBlank();
				return (owner->screenResetBlankMock->*(owner->screenResetBlankMock->getBehavior()))();
			}
			void blank(bool active)
			{
				owner->screenBlankMock->blank(active);
				return (owner->screenBlankMock->*(owner->screenBlankMock->getBehavior(active)))();
			}
			void autoBlank(bool enable)
			{
				owner->screenAutoBlankMock->autoBlank(enable);
				return (owner->screenAutoBlankMock->*(owner->screenAutoBlankMock->getBehavior(enable)))();
			}
		};

		//! The timers are managed by a timer service. */
		TimedSctUnitRunner * runner;

		MockDefault * defaultMock;
		MockCan * canMock;
		MockScreen * screenMock;

		virtual void SetUp()
		{
			statechart = new mrw::statechart::OperatingModeStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			hasActiveRoutesMock = new HasActiveRoutesMock(this);
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock(this);
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock(this);
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock(this);
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock(this);
			activateManualMock->initializeBehavior();
			keepAliveMock = new KeepAliveMock(this);
			keepAliveMock->initializeBehavior();
			screenResetBlankMock = new ScreenResetBlankMock(this);
			screenResetBlankMock->initializeBehavior();
			screenBlankMock = new ScreenBlankMock(this);
			screenBlankMock->initializeBehavior();
			screenAutoBlankMock = new ScreenAutoBlankMock(this);
			screenAutoBlankMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock(this);
			canIsConnectedMock = new CanIsConnectedMock(this);
			canIsConnectedMock->initializeBehavior();
			defaultMock = new MockDefault(this);
			canMock = new MockCan(this);
			screenMock = new MockScreen(this);
			statechart->setOperationCallback(defaultMock);
			statechart->can().setOperationCallback(canMock);
			statechart->screen().setOperationCallback(screenMock);
		}
		virtual void TearDown()
		{
			delete canIsConnectedMock;
			delete canConnectBusMock;
			delete screenAutoBlankMock;
			delete screenBlankMock;
			delete screenResetBlankMock;
			delete keepAliveMock;
			delete activateManualMock;
			delete resetTransactionMock;
			delete disableRoutesMock;
			delete isManualValidMock;
			delete hasActiveRoutesMock;
			delete statechart;
			delete defaultMock;
			defaultMock = 0;
			delete canMock;
			canMock = 0;
			delete screenMock;
			screenMock = 0;
			delete runner;
		}
	};



	void OperatingModeTest::wait()
	{
		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		isManualValidMock->setDefaultBehavior(&IsManualValidMock::isManualValid1);








		statechart->enter();

		EXPECT_TRUE(statechart->isActive());

		EXPECT_TRUE(!statechart->isFinal());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Prepare_Bus));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_blanking_On));

		EXPECT_TRUE(canConnectBusMock->calledAtLeastOnce());

		EXPECT_TRUE(screenBlankMock->calledAtLeastOnce());

	}
	TEST_F(OperatingModeTest, wait)
	{
		wait();
	}
	TEST_F(OperatingModeTest, keepAliveTest)
	{
		wait();

		runner->proceed_time(statechart->watchdog
			().getTimeout() * 1000);

		EXPECT_TRUE(keepAliveMock->calledAtLeastOnce());

	}
	TEST_F(OperatingModeTest, timeoutAfterWait)
	{
		wait();

		runner->proceed_time(statechart->can
			().getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

		EXPECT_TRUE(statechart->isRaisedFailing());

	}
	TEST_F(OperatingModeTest, routeChanged)
	{
		wait();

		statechart->raiseRoutesChanged();

		EXPECT_TRUE(hasActiveRoutesMock->calledAtLeastOnce());

		EXPECT_TRUE(screenAutoBlankMock->calledAtLeastOnce());

		EXPECT_TRUE(screenResetBlankMock->calledAtLeastOnce());

	}
	void OperatingModeTest::blank()
	{
		wait();

		runner->proceed_time(statechart->screen
			().getTimeout() * 1000);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_blanking_Off));

		EXPECT_TRUE(screenBlankMock->calledAtLeastOnce());

	}
	TEST_F(OperatingModeTest, blank)
	{
		blank();
	}
	TEST_F(OperatingModeTest, noBlank)
	{
		wait();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

		runner->proceed_time(statechart->screen
			().getTimeout() * 1000);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_blanking_On));

	}
	TEST_F(OperatingModeTest, unblank)
	{
		blank();

		statechart->screen
		().raiseUserInput();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_blanking_On));

		EXPECT_TRUE(screenBlankMock->calledAtLeastOnce());

	}
	void OperatingModeTest::initial()
	{
		wait();

		statechart->can
		().raiseConnected();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStart());

	}
	TEST_F(OperatingModeTest, initial)
	{
		initial();
	}
	void OperatingModeTest::failAfterStart()
	{
		initial();

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

		EXPECT_TRUE(statechart->isRaisedFailing());

	}
	TEST_F(OperatingModeTest, failAfterStart)
	{
		failAfterStart();
	}
	TEST_F(OperatingModeTest, timeoutAfterStart)
	{
		initial();

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

		EXPECT_TRUE(statechart->isRaisedFailing());

	}
	TEST_F(OperatingModeTest, clearFailState)
	{
		failAfterStart();

		canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected1);

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

		EXPECT_TRUE(statechart->isRaisedCleared());

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStart());

	}
	TEST_F(OperatingModeTest, stayFailState)
	{
		failAfterStart();

		canIsConnectedMock->setDefaultBehavior(&CanIsConnectedMock::isConnected2);

		statechart->raiseClear();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Failed));

	}
	TEST_F(OperatingModeTest, editFailed)
	{
		failAfterStart();

		statechart->raiseEdit();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing));

		EXPECT_TRUE(statechart->isRaisedEditing());

		EXPECT_TRUE((statechart->getEditingValue()) == (true));

	}
	void OperatingModeTest::doOperating()
	{
		initial();

		statechart->raiseStarted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (true));

	}
	TEST_F(OperatingModeTest, doOperating)
	{
		doOperating();
	}
	TEST_F(OperatingModeTest, failWhileOperating)
	{
		doOperating();

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));

	}
	void OperatingModeTest::initWhileOperatingWithRoutes()
	{
		doOperating();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

		statechart->raiseInit();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(disableRoutesMock->calledAtLeastOnce());

	}
	TEST_F(OperatingModeTest, initWhileOperatingWithRoutes)
	{
		initWhileOperatingWithRoutes();
	}
	TEST_F(OperatingModeTest, initWhileOperatingWithoutRoutes)
	{
		doOperating();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseInit();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStart());

	}
	TEST_F(OperatingModeTest, disableCompletedWithRoutes)
	{
		initWhileOperatingWithRoutes();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Disable));

	}
	TEST_F(OperatingModeTest, disableCompletedWithoutRoutes)
	{
		initWhileOperatingWithRoutes();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStart());

	}
	void OperatingModeTest::doEdit()
	{
		doOperating();

		statechart->raiseEdit();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Editing));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedEditing());

		EXPECT_TRUE((statechart->getEditingValue()) == (true));

	}
	TEST_F(OperatingModeTest, doEdit)
	{
		doEdit();
	}
	void OperatingModeTest::doManual()
	{
		doOperating();

		isManualValidMock->setDefaultBehavior(&IsManualValidMock::isManualValid1);

		statechart->raiseManual(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Manual));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedPlaying());

		EXPECT_TRUE((statechart->getPlayingValue()) == (true));

		EXPECT_TRUE(activateManualMock->calledAtLeastOnce(true));

	}
	TEST_F(OperatingModeTest, doManual)
	{
		doManual();
	}
	TEST_F(OperatingModeTest, doLeaveManual)
	{
		doManual();

		statechart->raiseManual(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Init));

		EXPECT_TRUE(statechart->isRaisedPlaying());

		EXPECT_TRUE((statechart->getPlayingValue()) == (false));

		EXPECT_TRUE(resetTransactionMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedStart());

	}
	void OperatingModeTest::doQuitManual()
	{
		doManual();

		statechart->raiseFinalize();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Wait));

		EXPECT_TRUE(statechart->isRaisedPlaying());

		EXPECT_TRUE((statechart->getPlayingValue()) == (false));

		EXPECT_TRUE(activateManualMock->calledAtLeastOnce(false));

		EXPECT_TRUE(screenBlankMock->calledAtLeastOnce());

	}
	TEST_F(OperatingModeTest, doQuitManual)
	{
		doQuitManual();
	}
	TEST_F(OperatingModeTest, doWaitManual)
	{
		doQuitManual();

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedQuit());

	}
	TEST_F(OperatingModeTest, operateAfterEdit)
	{
		doEdit();

		statechart->raiseOperate();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Running_operating_Operating));

		EXPECT_TRUE(statechart->isRaisedEditing());

		EXPECT_TRUE((statechart->getEditingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (true));

	}
	TEST_F(OperatingModeTest, doQuitOperating)
	{
		doOperating();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseFinalize();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedQuit());

	}
	TEST_F(OperatingModeTest, doQuitEditing)
	{
		doEdit();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseFinalize();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedEditing());

		EXPECT_TRUE((statechart->getEditingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedQuit());

	}
	TEST_F(OperatingModeTest, doQuitFailed)
	{
		failAfterStart();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseFinalize();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedQuit());

	}
	void OperatingModeTest::doQuitWithRoute()
	{
		doOperating();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes2);

		statechart->raiseFinalize();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region_Exit));

		EXPECT_TRUE(disableRoutesMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedOperating());

		EXPECT_TRUE((statechart->getOperatingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedQuitting());

		EXPECT_TRUE((statechart->getQuittingValue()) == (true));

	}
	TEST_F(OperatingModeTest, doQuitWithRoute)
	{
		doQuitWithRoute();
	}
	TEST_F(OperatingModeTest, doCompleteQuit)
	{
		doQuitWithRoute();

		hasActiveRoutesMock->setDefaultBehavior(&HasActiveRoutesMock::hasActiveRoutes1);

		statechart->raiseCompleted();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::OperatingModeStatechart::State::main_region__final_));

		EXPECT_TRUE(statechart->isRaisedQuitting());

		EXPECT_TRUE((statechart->getQuittingValue()) == (false));

		EXPECT_TRUE(statechart->isRaisedQuit());

	}

}

