//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/random.h>
#include <model/section.h>

#include "beermodeservice.h"
#include "controlledroute.h"

using namespace mrw::util;
using namespace mrw::model;

BeerModeService::BeerModeService()
{
	__METHOD__;
}

BeerModeService::~BeerModeService()
{
	__METHOD__;

	qInfo("  Shutting down beer mode service.");
}

void BeerModeService::init(ModelRailway * model)
{
	// First: Find all main rails.
	main_rails.clear();
	model->parts<Rail>(main_rails, &Rail::isMain);
}

ControlledRoute * BeerModeService::startBeerMode(const bool dir)
{
	std::vector<Rail *> candidates;

	findCandidates(candidates, dir);
	Rail * start = random(candidates);

	dump(candidates);
	if (start != nullptr)
	{
		std::vector<Rail *> pass_through_rails;
		std::vector<Rail *> end_rails;

		findPassthrough(pass_through_rails, start->region(), false);
		qDebug("-----");
		dump(pass_through_rails);

		findPassthrough(end_rails, start->region(), true);
		qDebug("-----");
		dump(end_rails);

		Rail * pass_through = random(pass_through_rails);
		Rail * end          = random(end_rails);

		if ((pass_through != nullptr) && (end != nullptr))
		{
			ControlledRoute * route = new ControlledRoute(dir, SectionState::TOUR, start);

			qDebug("-----");
			qDebug().noquote() << "Start: " << dump(start);
			qDebug().noquote() << "Pass:  " << dump(pass_through);
			qDebug().noquote() << "End:   " << dump(end);
			if (route->append(pass_through) && route->append(*end->advance(dir).begin()))
			{
				beer_route = route;
				return route;
			}
			else
			{
				delete route;
				emit disabledBeerMode();
			}
		}
	}
	return nullptr;
}

void BeerModeService::clearBeerRoute()
{
	beer_route = nullptr;
}

void BeerModeService::disableBeerMode(ControlledRoute * route)
{
	if (route == beer_route)
	{
		qInfo("Disabling beer mode");

		beer_route = nullptr;
		emit disabledBeerMode();
	}
}

BeerModeService::operator ControlledRoute * () const
{
	return beer_route;
}

void BeerModeService::findCandidates(
	std::vector<Rail *> & candidates,
	const bool            dir) const
{

	// Second: Select all occupied neighbours in wanted direction.
	for (Rail * rail : main_rails)
	{
		Rail * part = BeerModeService::isNeighbourOccupied(rail, dir);

		if (part != nullptr)
		{
			candidates.push_back(part);
		}
	}
}

void BeerModeService::findPassthrough(
	std::vector<Rail *> & candidates,
	const Region     *    region,
	const bool            is_same) const
{
	// Second: Select all free neighbours in both directions.
	for (Rail * rail : main_rails)
	{
		const bool same = rail->region() == region;

		if (isPassThrough(rail) && (same == is_same))
		{
			// Third: Push back if found region and search region have wanted
			// difference.
			candidates.push_back(rail);
		}
	}
}

bool BeerModeService::isPassThrough(const Rail * rail)
{
	return
		isNeighbourFree(rail, false) &&
		!rail->section()->occupation() &&
		isNeighbourFree(rail, true);
}

Rail * BeerModeService::isNeighbourOccupied(const Rail * rail, const bool dir)
{
	const std::set<RailInfo> & neighbours = rail->advance(dir);

	if (neighbours.size() == 1)
	{
		RailPart * part = *neighbours.begin();

		if (part->section()->occupation())
		{
			return dynamic_cast<Rail *>(part);
		}
	}
	return nullptr;
}

bool BeerModeService::isNeighbourFree(const Rail * rail, const bool dir)
{
	const std::set<RailInfo> & neighbours = rail->advance(dir);

	return (neighbours.size() > 0) &&
		std::all_of(neighbours.begin(), neighbours.end(), [](const RailInfo & info)
	{
		const RailPart * part = info;

		return !part->section()->occupation();
	});
}

/*************************************************************************
**                                                                      **
**       Utilities                                                      **
**                                                                      **
*************************************************************************/

Rail * BeerModeService::random(const std::vector<Rail *> & rails) const
{
	const size_t size = rails.size();

	return size > 0 ? rails.at(Random::random(size - 1)) : nullptr;
}

void BeerModeService::dump(const std::vector<Rail *> & rails) const
{
	for (const Rail * rail : rails)
	{
		qDebug().noquote() << dump(rail);
	}
}

QString BeerModeService::dump(const Rail * rail) const
{
	const Section * section = rail->section();

	return section->region()->name() + "/" + *section + "/" + *rail;
}
