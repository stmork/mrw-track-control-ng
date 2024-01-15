//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef BEERMODESERVICE_H
#define BEERMODESERVICE_H

#include <util/singleton.h>
#include <model/rail.h>
#include <model/modelrepository.h>

class ControlledRoute;

class BeerModeService :
	public QObject,
	public mrw::util::Singleton<BeerModeService>
{
	Q_OBJECT

	friend class Singleton<BeerModeService>;

private:

	std::vector<mrw::model::Rail *> main_rails;

	ControlledRoute        *        beer_route   = nullptr;

	BeerModeService();
	~BeerModeService();

public:
	void init(mrw::model::ModelRailway * model);
	ControlledRoute * startBeerMode(const bool dir);

	void clearBeerRoute() noexcept;
	void disableBeerMode(ControlledRoute * route) noexcept;
	operator ControlledRoute * () const noexcept;

signals:
	void disabledBeerMode();

private:
	void findCandidates(
		std::vector<mrw::model::Rail *> & candidates,
		const bool                        dir) const noexcept;

	void findPassthrough(
		std::vector<mrw::model::Rail *> & candidates,
		const mrw::model::Region     *    region,
		const bool                        is_same) const noexcept;

	[[nodiscard]]
	static bool isNeighbourFree(
		const mrw::model::Rail * rail,
		const bool               dir) noexcept;

	[[nodiscard]]
	static bool isPassThrough(
		const mrw::model::Rail * rail) noexcept;

	[[nodiscard]]
	static mrw::model::Rail * isNeighbourOccupied(
		const mrw::model::Rail * rail,
		const bool               dir) noexcept;

	[[nodiscard]]
	mrw::model::Rail * random(const std::vector<mrw::model::Rail *> & rails) const;

	void               dump(const std::vector<mrw::model::Rail *> & rails) const noexcept;

	[[nodiscard]]
	QString            dump(const mrw::model::Rail * rail) const noexcept;
};

#endif
