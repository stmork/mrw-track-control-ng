//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <unordered_set>

#include <QTest>

#include <model/rail.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "testrouting.h"
#include "testroute.h"

using namespace mrw::test;
using namespace mrw::model;

using LockState = Device::LockState;

TestRouting::TestRouting() : TestModelBase("Test-Flank")
{
}

void TestRouting::init()
{
	clear();
}

void TestRouting::testSimple()
{
	Rail * r11 = dynamic_cast<Rail *>(parts[0]);
	Rail * r26 = dynamic_cast<Rail *>(parts[20]);

	QVERIFY(r11 != nullptr);
	QVERIFY(r26 != nullptr);

	Route                    route(true, SectionState::SHUNTING, r11);
	const Route::RailTrack & reserved = route;

	QVERIFY(verify(route));
	testIsUnlockable(route, false);

	QVERIFY(route.append(r26));
	QVERIFY(route.isLastSectionEnded());
	QVERIFY(verify(route));

	testIsUnlockable(route, false);

	route.clear();
	testIsUnlockable(route, true);

	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testCounterPart()
{
	Rail * r11 = dynamic_cast<Rail *>(parts[0]);
	Rail * r21 = dynamic_cast<Rail *>(parts[1]);
	Rail * r16 = dynamic_cast<Rail *>(parts[19]);
	Rail * r26 = dynamic_cast<Rail *>(parts[20]);

	QVERIFY(r11 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r16 != nullptr);
	QVERIFY(r26 != nullptr);

	Route   route_lr(true,  SectionState::SHUNTING, r21);
	Route   route_rl(false, SectionState::SHUNTING, r16);

	QVERIFY(verify( { &route_lr, &route_rl } ));
	testIsUnlockable(route_lr, false);

	QVERIFY(route_lr.append(r26));
	testIsUnlockable(route_lr, false);
	QVERIFY(route_rl.append(r11));
	testIsUnlockable(route_rl, false);
	QVERIFY(verify( { &route_lr, &route_rl } ));

	route_lr.clear();
	route_rl.clear();
	testIsUnlockable(route_lr, true);
	testIsUnlockable(route_rl, true);
	QVERIFY(verify( { &route_lr, &route_rl } ));
	QVERIFY(empty());
}

void TestRouting::testCrossBlocked()
{
	Rail * r11 = dynamic_cast<Rail *>(parts[0]);
	Rail * r21 = dynamic_cast<Rail *>(parts[1]);
	Rail * r16 = dynamic_cast<Rail *>(parts[19]);
	Rail * r26 = dynamic_cast<Rail *>(parts[20]);

	QVERIFY(r11 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r16 != nullptr);
	QVERIFY(r26 != nullptr);

	Route   route_lr(true,  SectionState::TOUR, r21);
	Route   route_rl(false, SectionState::TOUR, r26);

	QVERIFY(verify( { &route_lr, &route_rl } ));

	QVERIFY( route_lr.append(r16));
	QVERIFY(!route_rl.append(r11));
	QVERIFY(verify( { &route_lr, &route_rl } ));

	route_lr.clear();
	route_rl.clear();
	QVERIFY(verify( { &route_lr, &route_rl } ));
	QVERIFY(empty());
}

void TestRouting::testExtension()
{
	RegularSwitch   *   s2  = dynamic_cast<RegularSwitch *>(parts[3]);
	RegularSwitch   *   s3  = dynamic_cast<RegularSwitch *>(parts[7]);
	DoubleCrossSwitch * s5  = dynamic_cast<DoubleCrossSwitch *>(parts[11]);
	RegularSwitch   *   s7  = dynamic_cast<RegularSwitch *>(parts[13]);
	RegularSwitch   *   s9  = dynamic_cast<RegularSwitch *>(parts[18]);
	Rail        *       r11 = dynamic_cast<Rail *>(parts[0]);
	Rail        *       r21 = dynamic_cast<Rail *>(parts[1]);
	Rail        *       rr3 = dynamic_cast<Rail *>(parts[12]);
	Rail        *       r16 = dynamic_cast<Rail *>(parts[19]);

	QVERIFY(s2 != nullptr);
	QVERIFY(s3 != nullptr);
	QVERIFY(s5 != nullptr);
	QVERIFY(s7 != nullptr);
	QVERIFY(s9 != nullptr);

	QVERIFY(r11 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r16 != nullptr);
	QVERIFY(rr3 != nullptr);

	Route                    route(true, SectionState::SHUNTING, r21);
	const Route::RailTrack & reserved = route;

	QVERIFY(verify(route));

	QVERIFY(route.append(rr3));
	QVERIFY(verify(route));
	QVERIFY(!route.isLastSectionEnded());

	QVERIFY(!route.append(r11));
	QVERIFY(verify(route));
	QVERIFY(!route.isLastSectionEnded());

	QVERIFY(route.append(r16));
	QVERIFY(verify(route));
	QVERIFY(route.isLastSectionEnded());

	QCOMPARE(s2->state(), RegularSwitch::State::AC);
	QCOMPARE(s3->state(), RegularSwitch::State::AB);
	QCOMPARE(s5->state(), DoubleCrossSwitch::State::BC);
	QCOMPARE(s7->state(), RegularSwitch::State::AB);
	QCOMPARE(s9->state(), RegularSwitch::State::AB);

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testOccupation()
{
	RegularSwitch   *   s1  = dynamic_cast<RegularSwitch *>(parts[2]);
	RegularSwitch   *   s2  = dynamic_cast<RegularSwitch *>(parts[3]);
	RegularSwitch   *   s4  = dynamic_cast<RegularSwitch *>(parts[4]);
	RegularSwitch   *   s7  = dynamic_cast<RegularSwitch *>(parts[13]);
	RegularSwitch   *   s9  = dynamic_cast<RegularSwitch *>(parts[18]);
	Rail        *       r21 = dynamic_cast<Rail *>(parts[1]);
	Rail        *       rr3 = dynamic_cast<Rail *>(parts[12]);
	Rail        *       r16 = dynamic_cast<Rail *>(parts[19]);

	QVERIFY(s2 != nullptr);
	QVERIFY(s1 != nullptr);
	QVERIFY(s4 != nullptr);
	QVERIFY(s7 != nullptr);
	QVERIFY(s9 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(rr3 != nullptr);
	QVERIFY(r16 != nullptr);

	Route                    route(true, SectionState::SHUNTING, r21);
	const Route::RailTrack & reserved = route;

	QVERIFY(verify(route));

	rr3->section()->setOccupation();

	QVERIFY(route.append(r16));
	QVERIFY(verify(route));

	QCOMPARE(s2->state(),  RegularSwitch::State::AB);
	QCOMPARE(s1->state(),  RegularSwitch::State::AB);
	QCOMPARE(s4->state(),  RegularSwitch::State::AB);
	QCOMPARE(s7->state(),  RegularSwitch::State::AC);
	QCOMPARE(s9->state(), RegularSwitch::State::AB);

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testInverseFail()
{
	Rail * r21 = dynamic_cast<Rail *>(parts[1]);
	Rail * r16 = dynamic_cast<Rail *>(parts[19]);

	QVERIFY(r21 != nullptr);
	QVERIFY(r16 != nullptr);

	Route                    route(false, SectionState::SHUNTING, r21);
	const Route::RailTrack & reserved = route;

	QVERIFY(verify(route));

	QVERIFY(!route.append(r16));
	QVERIFY(verify(route));

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testFailedSwitch()
{
	RegularSwitch * s1  = dynamic_cast<RegularSwitch *>(parts[2]);
	Rail      *     r21 = dynamic_cast<Rail *>(parts[1]);
	Rail      *     r26 = dynamic_cast<Rail *>(parts[20]);

	QVERIFY(s1 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r26 != nullptr);

	Route           route(true, SectionState::SHUNTING, r21);

	s1->setLock(LockState::FAIL);

	QVERIFY(verify(route));
	QVERIFY(!route.append(r26));
	QVERIFY(verify(route));
}

void TestRouting::testFlank()
{
	Rail * r21 = dynamic_cast<Rail *>(parts[1]);
	Rail * rr3 = dynamic_cast<Rail *>(parts[12]);
	Rail * r16 = dynamic_cast<Rail *>(parts[19]);

	QVERIFY(r21 != nullptr);
	QVERIFY(rr3 != nullptr);
	QVERIFY(r16 != nullptr);

	TestRoute  route(true, SectionState::TOUR, r21);

	QVERIFY(verify(route));

	QVERIFY(route.append(rr3));
	QVERIFY(route.append(r16));
	QVERIFY(verify(route));

	const std::vector<RegularSwitch *> & flanks = route.doFlank();

	QCOMPARE(flanks.size(), 4u);

	const Route::RailTrack & reserved = route;
	const size_t             curved   = std::count_if(
			reserved.begin(), reserved.end(),
			[](const RailPart * part)
	{
		return part->isCurved();
	});
	QVERIFY(curved > 0);
}

void TestRouting::testFlankLocked()
{
	RegularSwitch * s2  = dynamic_cast<RegularSwitch *>(parts[ 3]);
	RegularSwitch * s9  = dynamic_cast<RegularSwitch *>(parts[18]);
	Rail      *     r21 = dynamic_cast<Rail *>(parts[1]);
	Rail      *     r26 = dynamic_cast<Rail *>(parts[20]);

	QVERIFY(s2 != nullptr);
	QVERIFY(s9 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r26 != nullptr);

	TestRoute       route(true, SectionState::TOUR, r21);
	const Route::RailTrack & reserved = route;

	QVERIFY(s2 != nullptr);
	QVERIFY(s9 != nullptr);

	s2->setState(RegularSwitch::State::AB);
	s9->setState(RegularSwitch::State::AC);
	s2->setLock(LockState::LOCKED);
	s9->setLock(LockState::LOCKED);

	QVERIFY(verify(route));
	QVERIFY(!route.append(r26));
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 1u);

	const std::vector<RegularSwitch *> & flanks = route.doFlank();

	QCOMPARE(flanks.size(), 0u);

	s2->setState(RegularSwitch::State::AC, true);
	s9->setState(RegularSwitch::State::AB, true);
	QVERIFY(route.append(r26));
	QVERIFY(verify(route));
	QVERIFY(reserved.size() > 1u);
	route.doFlank();
	QCOMPARE(flanks.size(), 2u);
}

bool TestRouting::verify(const Route & route) const
{
	return verify( { & route } );
}

bool TestRouting::verify(std::initializer_list<const Route *> routes) const
{
	std::unordered_set<RailPart *> unreserved(parts.begin(), parts.end());
	std::unordered_set<RailPart *> reserved;

	// Collect all reserved rail parts.
	for (const Route * route : routes)
	{
		const Route::RailTrack & track = *route;

		reserved.insert(track.begin(), track.end());
		testIsUnlockable(track, false);
	}

	// Remove reserved rail parts from unreserved.
	for (RailPart * part : reserved)
	{
		unreserved.erase(part);
	}

	const bool is_reserved = std::all_of(reserved.begin(), reserved.end(), [&](RailPart * part)
	{
		return part->reserved();
	});
	const bool is_unreserved = std::all_of(unreserved.begin(), unreserved.end(), [&](RailPart * part)
	{
		return !part->reserved();
	});

	return
		is_reserved && is_unreserved &&
		((unreserved.size() + reserved.size()) == parts.size());
}

bool TestRouting::empty() const
{
	return std::all_of(parts.begin(), parts.end(), [&](RailPart * part)
	{
		return !part->reserved() && part->section()->isUnlockable();
	});
}

void TestRouting::testIsUnlockable(
	const Route::RailTrack & track,
	const bool               is_unlockable) const
{
	for (RailPart * part : track)
	{
		Device * device = dynamic_cast<Device *>(part);

		if (device != nullptr)
		{
			QCOMPARE(device->isUnlockable(), is_unlockable);
		}
		QCOMPARE(part->section()->isUnlockable(), is_unlockable);
		QCOMPARE(part->section()->anyReserved(), !is_unlockable);
	}
}
