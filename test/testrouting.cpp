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

void TestRouting::initTestCase()
{
	model = new ModelRailway(filename);
	QVERIFY(model != nullptr);

	model->parts<RailPart>(parts);
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
	Route route(true, SectionState::SHUNTING, parts[0]);

	QVERIFY(route.extend(parts[20]));
	QVERIFY(verify(route));
}

bool TestRouting::verify(const Route & route) const
{
	std::unordered_set<RailPart *> unreserved(parts.begin(), parts.end());

	const std::list<RailPart *>    reserved = route;

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

	return is_reserved && is_unreserved;
}
