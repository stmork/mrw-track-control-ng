//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <unordered_set>

#include <QTest>

#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "testrouting.h"

using namespace mrw::test;
using namespace mrw::model;

TestRouting::TestRouting() : TestModelBase("Test-Flank")
{
}

void TestRouting::init()
{
	for (RailPart * part : parts)
	{
		part->reserve(false);
		part->section()->setState(SectionState::FREE);
	}
}

void TestRouting::testSimple()
{
	Route                    route(true, SectionState::SHUNTING, parts[0]);
	const Route::Track   &   reserved = route;

	QVERIFY(verify(route));

	QVERIFY(route.extend(parts[20]));
	QVERIFY(verify(route));

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testCounterPart()
{
	Route   route_lr(true,  SectionState::SHUNTING, parts[1]);
	Route   route_rl(false, SectionState::SHUNTING, parts[19]);

	QVERIFY(verify( { &route_lr, &route_rl } ));

	QVERIFY(route_lr.extend(parts[20]));
	QVERIFY(route_rl.extend(parts[0]));
	QVERIFY(verify( { &route_lr, &route_rl } ));

	route_lr.clear();
	route_rl.clear();
	QVERIFY(verify( { &route_lr, &route_rl } ));
	QVERIFY(empty());
}

void TestRouting::testCrossBlocked()
{
	Route   route_lr(true,  SectionState::TOUR, parts[1]);
	Route   route_rl(false, SectionState::TOUR, parts[20]);

	QVERIFY(verify( { &route_lr, &route_rl } ));

	QVERIFY(route_lr.extend(parts[19]));
	QVERIFY(!route_rl.extend(parts[0]));
	QVERIFY(verify( { &route_lr, &route_rl } ));

	route_lr.clear();
	route_rl.clear();
	QVERIFY(verify( { &route_lr, &route_rl } ));
	QVERIFY(empty());
}

void TestRouting::testExtension()
{
	Route                    route(true, SectionState::SHUNTING, parts[1]);
	const Route::Track   &   reserved = route;

	QVERIFY(verify(route));

	QVERIFY(route.extend(parts[14]));
	QVERIFY(verify(route));

	QVERIFY(!route.extend(parts[0]));
	QVERIFY(verify(route));

	QVERIFY(route.extend(parts[19]));
	QVERIFY(verify(route));

	QCOMPARE(static_cast<RegularSwitch *>(parts[2])->state(), RegularSwitch::State::AC);
	QCOMPARE(static_cast<RegularSwitch *>(parts[9])->state(), RegularSwitch::State::AB);
	QCOMPARE(static_cast<DoubleCrossSwitch *>(parts[13])->state(), DoubleCrossSwitch::State::BC);
	QCOMPARE(static_cast<RegularSwitch *>(parts[8])->state(),  RegularSwitch::State::AB);
	QCOMPARE(static_cast<RegularSwitch *>(parts[18])->state(), RegularSwitch::State::AB);

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testOccupation()
{
	Route                    route(true, SectionState::SHUNTING, parts[1]);
	const Route::Track   &   reserved = route;

	QVERIFY(verify(route));

	parts[14]->section()->setOccupation();

	QVERIFY(route.extend(parts[19]));
	QVERIFY(verify(route));

	QCOMPARE(static_cast<RegularSwitch *>(parts[2])->state(),  RegularSwitch::State::AB);
	QCOMPARE(static_cast<RegularSwitch *>(parts[3])->state(),  RegularSwitch::State::AB);
	QCOMPARE(static_cast<RegularSwitch *>(parts[6])->state(),  RegularSwitch::State::AB);
	QCOMPARE(static_cast<RegularSwitch *>(parts[8])->state(),  RegularSwitch::State::AC);
	QCOMPARE(static_cast<RegularSwitch *>(parts[18])->state(), RegularSwitch::State::AB);

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
}

void TestRouting::testInverseFail()
{
	Route                    route(false, SectionState::SHUNTING, parts[1]);
	const Route::Track   &   reserved = route;

	QVERIFY(verify(route));

	QVERIFY(!route.extend(parts[19]));
	QVERIFY(verify(route));

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0u);
	QVERIFY(empty());
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
		const Route::Track & track = *route;

		reserved.insert(track.begin(), track.end());
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
		return !part->reserved() && part->section()->unlockable();
	});
}
