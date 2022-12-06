//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <unordered_set>

#include <QTest>

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
	QCOMPARE(reserved.size(), 0);
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

	route.clear();
	QVERIFY(verify(route));
	QCOMPARE(reserved.size(), 0);
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
		return !part->reserved();
	});
}
