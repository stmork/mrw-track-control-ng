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
	mrw::statechart::OperatingModeStatechart * statechart;


	class HasActiveRoutesMock
	{
		typedef bool (HasActiveRoutesMock::*functiontype)();
	public:
		bool (HasActiveRoutesMock::*hasActiveRoutesBehaviorDefault)();
		int callCount;

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
	static HasActiveRoutesMock * hasActiveRoutesMock;

	class IsManualValidMock
	{
		typedef bool (IsManualValidMock::*functiontype)();
	public:
		bool (IsManualValidMock::*isManualValidBehaviorDefault)();

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
	static IsManualValidMock * isManualValidMock;

	class DisableRoutesMock
	{
		typedef void (DisableRoutesMock::*functiontype)();
	public:
		void (DisableRoutesMock::*disableRoutesBehaviorDefault)();
		int callCount;

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
	static DisableRoutesMock * disableRoutesMock;

	class ResetTransactionMock
	{
		typedef void (ResetTransactionMock::*functiontype)();
	public:
		void (ResetTransactionMock::*resetTransactionBehaviorDefault)();
		int callCount;

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
	static ResetTransactionMock * resetTransactionMock;

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
		std::list<ActivateManualMock::parameters> mocks;
		std::list<ActivateManualMock::parameters> paramCount;
		void (ActivateManualMock::*activateManualBehaviorDefault)();
		int callCount;

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
	static ActivateManualMock * activateManualMock;

	class KeepAliveMock
	{
		typedef void (KeepAliveMock::*functiontype)();
	public:
		void (KeepAliveMock::*keepAliveBehaviorDefault)();
		int callCount;

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
	static KeepAliveMock * keepAliveMock;

	class ScreenResetBlankMock
	{
		typedef void (ScreenResetBlankMock::*functiontype)();
	public:
		void (ScreenResetBlankMock::*screenResetBlankBehaviorDefault)();
		int callCount;

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
	static ScreenResetBlankMock * screenResetBlankMock;

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
		std::list<ScreenBlankMock::parameters> mocks;
		std::list<ScreenBlankMock::parameters> paramCount;
		void (ScreenBlankMock::*screenBlankBehaviorDefault)();
		int callCount;

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
	static ScreenBlankMock * screenBlankMock;

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
		std::list<ScreenAutoBlankMock::parameters> mocks;
		std::list<ScreenAutoBlankMock::parameters> paramCount;
		void (ScreenAutoBlankMock::*screenAutoBlankBehaviorDefault)();
		int callCount;

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
	static ScreenAutoBlankMock * screenAutoBlankMock;

	class CanConnectBusMock
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

		void connectBus()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static CanConnectBusMock * canConnectBusMock;

	class CanIsConnectedMock
	{
		typedef bool (CanIsConnectedMock::*functiontype)();
	public:
		bool (CanIsConnectedMock::*canIsConnectedBehaviorDefault)();

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
	static CanIsConnectedMock * canIsConnectedMock;

	class MockDefault : public mrw::statechart::OperatingModeStatechart::OperationCallback
	{
	public:
		void keepAlive()
		{
			keepAliveMock->keepAlive();
			return (keepAliveMock->*(keepAliveMock->getBehavior()))();
		}
		void resetTransaction()
		{
			resetTransactionMock->resetTransaction();
			return (resetTransactionMock->*(resetTransactionMock->getBehavior()))();
		}
		bool isManualValid()
		{
			return (isManualValidMock->*(isManualValidMock->getBehavior()))();
		}
		bool hasActiveRoutes()
		{
			hasActiveRoutesMock->hasActiveRoutes();
			return (hasActiveRoutesMock->*(hasActiveRoutesMock->getBehavior()))();
		}
		void disableRoutes()
		{
			disableRoutesMock->disableRoutes();
			return (disableRoutesMock->*(disableRoutesMock->getBehavior()))();
		}
		void activateManual(bool active)
		{
			activateManualMock->activateManual(active);
			return (activateManualMock->*(activateManualMock->getBehavior(active)))();
		}
	};
	class MockCan : public mrw::statechart::OperatingModeStatechart::Can::OperationCallback
	{
	public:
		void connectBus()
		{
			canConnectBusMock->connectBus();
		}
		bool isConnected()
		{
			return (canIsConnectedMock->*(canIsConnectedMock->getBehavior()))();
		}
	};
	class MockScreen : public mrw::statechart::OperatingModeStatechart::Screen::OperationCallback
	{
	public:
		void resetBlank()
		{
			screenResetBlankMock->resetBlank();
			return (screenResetBlankMock->*(screenResetBlankMock->getBehavior()))();
		}
		void blank(bool active)
		{
			screenBlankMock->blank(active);
			return (screenBlankMock->*(screenBlankMock->getBehavior(active)))();
		}
		void autoBlank(bool enable)
		{
			screenAutoBlankMock->autoBlank(enable);
			return (screenAutoBlankMock->*(screenAutoBlankMock->getBehavior(enable)))();
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class OperatingModeTest : public ::testing::Test
	{
	protected:
		MockDefault defaultMock;
		MockCan canMock;
		MockScreen screenMock;
		virtual void SetUp()
		{
			statechart = new mrw::statechart::OperatingModeStatechart();
			size_t maximalParallelTimeEvents = (size_t)statechart->getNumberOfParallelTimeEvents();
			runner = new TimedSctUnitRunner(
				maximalParallelTimeEvents
			);
			statechart->setTimerService(runner);
			hasActiveRoutesMock = new HasActiveRoutesMock();
			hasActiveRoutesMock->initializeBehavior();
			isManualValidMock = new IsManualValidMock();
			isManualValidMock->initializeBehavior();
			disableRoutesMock = new DisableRoutesMock();
			disableRoutesMock->initializeBehavior();
			resetTransactionMock = new ResetTransactionMock();
			resetTransactionMock->initializeBehavior();
			activateManualMock = new ActivateManualMock();
			activateManualMock->initializeBehavior();
			keepAliveMock = new KeepAliveMock();
			keepAliveMock->initializeBehavior();
			screenResetBlankMock = new ScreenResetBlankMock();
			screenResetBlankMock->initializeBehavior();
			screenBlankMock = new ScreenBlankMock();
			screenBlankMock->initializeBehavior();
			screenAutoBlankMock = new ScreenAutoBlankMock();
			screenAutoBlankMock->initializeBehavior();
			canConnectBusMock = new CanConnectBusMock();
			canIsConnectedMock = new CanIsConnectedMock();
			canIsConnectedMock->initializeBehavior();
			statechart->setOperationCallback(&defaultMock);
			statechart->can().setOperationCallback(&canMock);
			statechart->screen().setOperationCallback(&screenMock);
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
			delete runner;
		}
	};


	void wait()
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

		runner->proceed_time(statechart->getWd_timeout() * 1000);

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
	void blank()
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
	void initial()
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
	void failAfterStart()
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
	void doOperating()
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
	void initWhileOperatingWithRoutes()
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
	void doEdit()
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
	void doManual()
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
	void doQuitManual()
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
	void doQuitWithRoute()
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

