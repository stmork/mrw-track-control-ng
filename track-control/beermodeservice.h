//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef BEERMODESERVICE_H
#define BEERMODESERVICE_H

#include <util/singleton.h>
#include <model/rail.h>
#include <model/modelrepository.h>

class WidgetRoute;

class BeerModeService :
	public mrw::util::Singleton<BeerModeService>
{
	friend class Singleton<BeerModeService>;

	std::vector<mrw::model::Rail *> main_rails;

	BeerModeService();
	~BeerModeService();

	static bool isNeighbourFree(
		const mrw::model::Rail * rail,
		const bool               dir);

public:
	void init(mrw::model::ModelRailway * model);
	WidgetRoute * startBeerMode(const bool dir);

private:
	void findCandidates(
		std::vector<mrw::model::Rail *> & candidates,
		const bool                        dir) const;

	void findPassthrough(
		std::vector<mrw::model::Rail *> & candidates,
		const mrw::model::Region     *    region,
		const bool                        is_same) const;

	static bool isPassThrough(
		const mrw::model::Rail * rail);

	static mrw::model::Rail * isNeighbourOccupied(
		const mrw::model::Rail * rail,
		const bool               dir);

	mrw::model::Rail * random(const std::vector<mrw::model::Rail *> & rails) const;
	void               dump(const std::vector<mrw::model::Rail *> & rails) const;
	QString            dump(const mrw::model::Rail * rail) const;
};

#endif
