/** *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */
#include <string>
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
	void operational();
	void enabling();
	void disabling();
	void enabledLocked();
	void disabledLocked();
	void sectionFree();
	void disableAfterEnabled();
	void leave();
	void passedState();
	void enablingAfterDisabled();
	mrw::statechart::SectionStatechart * statechart;


	class FailMock
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

		void fail()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FailMock * failMock;

	class IncMock
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

		void inc()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static IncMock * incMock;

	class FreeMock
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

		void free()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static FreeMock * freeMock;

	class LockMock
	{
		struct parameters
		{
			bool do_it;
			int callCount;
			inline bool operator==(const parameters & other)
			{
				return (this->do_it == other.do_it);
			}
		};
	public:
		std::list<LockMock::parameters> paramCount;
		int callCount;

		bool calledAtLeast(const int times)
		{
			return (callCount >= times);
		}

		bool calledAtLeastOnce()
		{
			return (callCount > 0);
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
		void reset()
		{
			callCount = 0;
			paramCount.clear();
		}
	};
	static LockMock * lockMock;

	class DecMock
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

		void dec()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static DecMock * decMock;

	class PendingMock
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

		void pending()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static PendingMock * pendingMock;

	class OnMock
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

		void on()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static OnMock * onMock;

	class OffMock
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

		void off()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static OffMock * offMock;

	class LeftBeforeMock
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

		void leftBefore()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static LeftBeforeMock * leftBeforeMock;

	class PassedMock
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

		void passed()
		{
			++callCount;
		}
		void reset()
		{
			callCount = 0;
		}
	};
	static PassedMock * passedMock;

	class MockDefault : public mrw::statechart::SectionStatechart::OperationCallback
	{
	public:
		void inc()
		{
			incMock->inc();
		}
		void dec()
		{
			decMock->dec();
		}
		void off()
		{
			offMock->off();
		}
		void on()
		{
			onMock->on();
		}
		void request()
		{
		}
		void passed()
		{
			passedMock->passed();
		}
		void free()
		{
			freeMock->free();
		}
		void leftBefore()
		{
			leftBeforeMock->leftBefore();
		}
		void fail()
		{
			failMock->fail();
		}
		void pending()
		{
			pendingMock->pending();
		}
		void lock(bool do_it)
		{
			lockMock->lock(do_it);
		}
	};

//! The timers are managed by a timer service. */
	static TimedSctUnitRunner * runner;

	class SectionTest : public ::testing::Test
	{
	protected:
		virtual void SetUp()
		{
			statechart = new mrw::statechart::SectionStatechart();
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

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Wait_for_Start));


	}
	TEST_F(SectionTest, waitForStart)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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
	TEST_F(SectionTest, failAfterStart)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		failAfterStart();
	}
	TEST_F(SectionTest, timeoutAfterStart)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		initial();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	void operational()
	{
		initial();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_relais_Wait));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Init_Init_Process_Requesting_state_Occupation));

		statechart->raiseStateResponse(false);

		unlocked();

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}
	TEST_F(SectionTest, operational)
	{
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();
	}
	TEST_F(SectionTest, restart)
	{
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();

		statechart->raiseStart();

		initing();


	}
	TEST_F(SectionTest, clearing)
	{

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());


		incMock->reset();
		pendingMock->reset();
		onMock->reset();
	}
	TEST_F(SectionTest, enabling)
	{
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enabling();
	}
	void disabling()
	{
		operational();

		statechart->raiseEnable(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Disabling));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());


		incMock->reset();
		pendingMock->reset();
		offMock->reset();
	}
	TEST_F(SectionTest, disabling)
	{
		incMock = new IncMock();
		pendingMock = new PendingMock();
		offMock = new OffMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disabling();
	}
	void enabledLocked()
	{
		enabling();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

		EXPECT_TRUE(!statechart->getOccupied());


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SectionTest, enabledLocked)
	{
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enabledLocked();
	}
	void disabledLocked()
	{
		disabling();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Disabled));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

		EXPECT_TRUE(!statechart->getOccupied());


		decMock->reset();
		lockMock->reset();
	}
	TEST_F(SectionTest, disabledLocked)
	{
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		offMock = new OffMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disabledLocked();
	}
	void sectionFree()
	{
		operational();

		statechart->setOccupied(false);

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Free));


		incMock->reset();
		pendingMock->reset();
		onMock->reset();
	}
	TEST_F(SectionTest, sectionFree)
	{
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		sectionFree();
	}
	TEST_F(SectionTest, sectionOccupied)
	{
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		operational();

		statechart->setOccupied(true);

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Pending_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(pendingMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Occupation_Occupied));


		incMock->reset();
		pendingMock->reset();
		onMock->reset();
	}
	TEST_F(SectionTest, failAfterEnable)
	{
		failMock = new FailMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enabling();

		statechart->raiseFailed();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}
	TEST_F(SectionTest, timeoutAfterEnable)
	{
		failMock = new FailMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enabling();

		runner->proceed_time(statechart->getTimeout());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Failed));

		EXPECT_TRUE(failMock->calledAtLeastOnce());


		failMock->reset();
	}
	void disableAfterEnabled()
	{
		enabledLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());


		incMock->reset();
		offMock->reset();
	}
	TEST_F(SectionTest, disableAfterEnabled)
	{
		incMock = new IncMock();
		offMock = new OffMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableAfterEnabled();
	}
	TEST_F(SectionTest, disableAfterDisabled)
	{
		freeMock = new FreeMock();
		lockMock = new LockMock();
		decMock = new DecMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		offMock = new OffMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disabledLocked();

		statechart->raiseDisable();

		EXPECT_TRUE(freeMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		freeMock->reset();
		lockMock->reset();
		decMock->reset();
	}
	TEST_F(SectionTest, unlock)
	{
		decMock = new DecMock();
		incMock = new IncMock();
		offMock = new OffMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableAfterEnabled();

		statechart->raiseRelaisResponse();

		unlocked();

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}
	TEST_F(SectionTest, failAfterDisable)
	{
		incMock = new IncMock();
		offMock = new OffMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableAfterEnabled();

		statechart->raiseFailed();

		failState();


	}
	TEST_F(SectionTest, timeAfterDisable)
	{
		incMock = new IncMock();
		offMock = new OffMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		disableAfterEnabled();

		runner->proceed_time(statechart->getTimeout());

		failState();


	}
	void leave()
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

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(!statechart->getOccupied());

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeaving());


		incMock->reset();
		offMock->reset();
	}
	TEST_F(SectionTest, leave)
	{
		incMock = new IncMock();
		offMock = new OffMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		leave();
	}
	TEST_F(SectionTest, leaveBeforeNext)
	{
		incMock = new IncMock();
		offMock = new OffMock();
		leftBeforeMock = new LeftBeforeMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
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

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(offMock->calledAtLeastOnce());

		EXPECT_TRUE(leftBeforeMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeaving());


		incMock->reset();
		offMock->reset();
		leftBeforeMock->reset();
	}
	TEST_F(SectionTest, stay)
	{
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enabledLocked();

		statechart->setAuto_off(false);

		statechart->raiseStateResponse(false);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));


	}
	TEST_F(SectionTest, autoUnlock)
	{
		decMock = new DecMock();
		incMock = new IncMock();
		offMock = new OffMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		leave();

		statechart->setAuto_unlock(true);

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeft());


		decMock->reset();
	}
	void passedState()
	{
		leave();

		statechart->setAuto_unlock(false);

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Passed));

		EXPECT_TRUE(decMock->calledAtLeastOnce());

		EXPECT_TRUE(passedMock->calledAtLeastOnce());

		EXPECT_TRUE(statechart->isRaisedLeft());

		EXPECT_TRUE(statechart->isRaisedTryUnblock());


		decMock->reset();
		passedMock->reset();
	}
	TEST_F(SectionTest, passedState)
	{
		decMock = new DecMock();
		passedMock = new PassedMock();
		incMock = new IncMock();
		offMock = new OffMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		passedState();
	}
	TEST_F(SectionTest, disableAfterPassed)
	{
		freeMock = new FreeMock();
		lockMock = new LockMock();
		decMock = new DecMock();
		passedMock = new PassedMock();
		incMock = new IncMock();
		offMock = new OffMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		onMock = new OnMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		passedState();

		statechart->raiseDisable();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Unlocked));

		EXPECT_TRUE(freeMock->calledAtLeastOnce());

		EXPECT_TRUE(lockMock->calledAtLeastOnce());


		freeMock->reset();
		lockMock->reset();
	}
	void enablingAfterDisabled()
	{
		disabledLocked();

		statechart->raiseEnable(true);

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling));

		EXPECT_TRUE(incMock->calledAtLeastOnce());

		EXPECT_TRUE(onMock->calledAtLeastOnce());


		incMock->reset();
		onMock->reset();
	}
	TEST_F(SectionTest, enablingAfterDisabled)
	{
		incMock = new IncMock();
		onMock = new OnMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		offMock = new OffMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enablingAfterDisabled();
	}
	TEST_F(SectionTest, enabledAfterDisabled)
	{
		decMock = new DecMock();
		incMock = new IncMock();
		onMock = new OnMock();
		decMock = new DecMock();
		lockMock = new LockMock();
		incMock = new IncMock();
		pendingMock = new PendingMock();
		offMock = new OffMock();
		decMock = new DecMock();

		MockDefault defaultMock;
		statechart->setOperationCallback(&defaultMock);
		enablingAfterDisabled();

		statechart->raiseRelaisResponse();

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked));

		EXPECT_TRUE(statechart->isStateActive(mrw::statechart::SectionStatechart::State::main_region_Operating_Processing_Locked_Route_active_Enabled));

		EXPECT_TRUE(decMock->calledAtLeastOnce());


		decMock->reset();
	}

}

