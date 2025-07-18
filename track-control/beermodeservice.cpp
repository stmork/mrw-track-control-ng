//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <util/random.h>
#include <model/section.h>

#include "beermodeservice.h"
#include "controlledroute.h"
#include "log.h"

using namespace mrw::util;
using namespace mrw::model;

BeerModeService::BeerModeService()
{
	__METHOD__;
}

BeerModeService::~BeerModeService()
{
	__METHOD__;

	qCInfo(mrw::tools::log, "  Shutting down beer mode service.");
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
		qCDebug(mrw::tools::log, "-----");
		dump(pass_through_rails);

		findPassthrough(end_rails, start->region(), true);
		qCDebug(mrw::tools::log, "-----");
		dump(end_rails);

		Rail * pass_through = random(pass_through_rails);
		Rail * end          = random(end_rails);

		if ((pass_through != nullptr) && (end != nullptr))
		{
			ControlledRoute * route = new ControlledRoute(dir, SectionState::TOUR, start);

			qCDebug(mrw::tools::log, "-----");
			qCDebug(mrw::tools::log).noquote() << "Start: " << dump(start);
			qCDebug(mrw::tools::log).noquote() << "Pass:  " << dump(pass_through);
			qCDebug(mrw::tools::log).noquote() << "End:   " << dump(end);
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

void BeerModeService::clearBeerRoute() noexcept
{
	beer_route = nullptr;
}

void BeerModeService::disableBeerMode(ControlledRoute * route) noexcept
{
	if (route == beer_route)
	{
		qCInfo(mrw::tools::log, "Disabling beer mode");

		beer_route = nullptr;
		emit disabledBeerMode();
	}
}

BeerModeService::operator ControlledRoute * () const noexcept
{
	return beer_route;
}

void BeerModeService::findCandidates(
	std::vector<Rail *> & candidates,
	const bool            dir) const noexcept
{
	// Second: Select all occupied neighbours in wanted direction.
	for (const Rail * rail : main_rails)
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
	const bool            is_same) const noexcept
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

bool BeerModeService::isPassThrough(const Rail * rail) noexcept
{
	return
		isNeighbourFree(rail, false) &&
		!rail->section()->occupation() &&
		isNeighbourFree(rail, true);
}

Rail * BeerModeService::isNeighbourOccupied(const Rail * rail, const bool dir) noexcept
{
	const std::set<RailInfo> & neighbours = rail->advance(dir);

	if (neighbours.size() == 1)
	{
		RailPart * part = *neighbours.begin(); // always valid since size == 1

		if (part->section()->occupation())
		{
			return dynamic_cast<Rail *>(part);
		}
	}
	return nullptr;
}

bool BeerModeService::isNeighbourFree(const Rail * rail, const bool dir) noexcept
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

void BeerModeService::dump(const std::vector<Rail *> & rails) const noexcept
{
	for (const Rail * rail : rails)
	{
		qCDebug(mrw::tools::log).noquote() << dump(rail);
	}
}

QString BeerModeService::dump(const Rail * rail) const noexcept
{
	const Section * section = rail->section();
	const QString & section_descr(*section);
	const QString & rail_descr(*rail);

	return section->region()->name() + "/" + section_descr + "/" + rail_descr;
}
