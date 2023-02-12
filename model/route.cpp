//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <model/rail.h>
#include <model/abstractswitch.h>
#include <model/regularswitch.h>
#include <model/route.h>

using namespace mrw::model;

using LockState = Device::LockState;

Route::Route(
	const bool         dir,
	const SectionState wanted_state,
	RailPart     *     first,
	QObject      *     parent) :
	QObject(parent),
	state(wanted_state),
	direction(dir),
	auto_unblock(wanted_state != SectionState::TOUR)
{
	__METHOD__;

	bool      loop = true;

	first_section = first->section();
	do
	{
		const std::set<RailInfo> & back = first->advance(!dir);

		if (back.size() == 1)
		{
			RailPart * part = *back.begin();
			Rail   *   rail = dynamic_cast<Rail *>(part);

			if ((rail == nullptr) || (part->section() != first_section))
			{
				loop = false;
			}
			else
			{
				first = rail;
			}
		}
		else
		{
			loop = false;
		}
	}
	while (loop);

	first->reserve();
	track.push_back(first);
}

Route::~Route()
{
	__METHOD__;

	clear();
	Route::dump();
}

bool Route::append(RailPart * target)
{
	__METHOD__;

	qDebug().noquote() << "---------->" << *target;

	last_valid_part    = track.back();

	Region  *  search_region = findSearchRegion(target);
	const bool success       = append(last_valid_part, target, search_region);

	if (success)
	{
		prepare();
	}

	return success;
}

Region * Route::findSearchRegion(const RailPart * target) const
{
	if (state == SectionState::SHUNTING)
	{
		return first_section->region();
	}
	else
	{
		const Region * start_region  = last_valid_part->section()->region();
		const Region * target_region = target->section()->region();

		return start_region == target_region ?
			target->section()->region() : nullptr;
	};
}

bool Route::append(RailPart * rail, RailPart * target, Region * search_region)
{
	if (rail == target)
	{
		rail->reserve();
		return true;
	}

	for (const RailInfo & info : rail->advance(direction))
	{
		RailPart * next = info;

		if (qualified(next, search_region))
		{
			next->reserve();
			track.push_back(next);
			if (append(next, target, search_region))
			{
				return true;
			}
			track.remove(next);
			next->reserve(false);
		}
	}

	return false;
}

bool Route::qualified(
	const RailPart * rail,
	const Region  *  search_region) const
{
	const QString   indent(track.size(), ' ');
	const Section * section = rail->section();
	const Device  * device  = dynamic_cast<const Device *>(rail);

	qDebug().noquote() << indent << rail->toString();

	if ((device != nullptr) && (device->lock() == LockState::FAIL))
	{
		qDebug().noquote() << indent << "      Rail in failed state:";
		return false;
	}

#if 0
	if ((device != nullptr) && (state == SectionState::TOUR))
	{
		std::vector<RegularSwitch *> flank_switches_cand;
		const AbstractSwitch    *    actual_switch = dynamic_cast<const AbstractSwitch *>(device);

		if (actual_switch != nullptr)
		{
			// FIXME: At this point we can not decide which branch will be selected.
			// So we cannot check the correct flank protection to verify
			// locking and correct direction state. Since this will is not a
			// concerning issue the code is disabled.

			const size_t count      = actual_switch->flank(flank_switches_cand);
			const size_t any_locked = std::count_if(flank_switches_cand.begin(), flank_switches_cand.end(),
						[this](RegularSwitch * flank_switch)
			{
				qDebug().noquote() << *flank_switch;
				return flank_switch->lock() == LockState::UNLOCKED;
			});

			qDebug().noquote() << rail->toString() << count << flank_switches_cand.size() << any_locked;
			if ((any_locked != flank_switches_cand.size()) && (count != flank_switches_cand.size()))
			{
				qDebug().noquote() << indent << "      Flank protection not granted:";
				return false;
			}
		}
	}
#endif

	if (rail->reserved())
	{
		qDebug().noquote() << indent << "      Rail already reserved:";
		return false;
	}

	if (track.size() > 100)
	{
		qDebug().noquote() << indent << "      Recursion depth reached.";
		return false;
	}

	if (section != first_section)
	{
		if ((search_region != nullptr) && (section->region() != search_region))
		{
			qDebug().noquote() << indent << "      Shunting left region.";
			return false;
		}
		if (section->occupation())
		{
			qDebug().noquote() << indent << "      Section occupied.";
			return false;
		}
	}

	return true;
}

void Route::prepare()
{
	__METHOD__;

	std::vector<RailPart *> vector(track.begin(), track.end());
	Section        *        prev = nullptr;

	sections.clear();
	for (size_t i = 0; i < vector.size(); i++)
	{
		// Collect sections in a unique manner.
		Section * act = vector[i]->section();

		if (prev != act)
		{
			sections.push_back(act);
			prev = act;
		}

		// Set track mode.
		if (act->isFree())
		{
			act->setState(state);
		}

		// Prepare switch turning.
		if ((i > 0) && ((i + 1) < vector.size()))
		{
			vector[i]->setState(vector[i - 1], vector[i + 1]);
		}
	}

	const bool last_on = isLastSectionEnded();
	const auto it      = sections.rbegin();

	last_section = last_on ? nullptr : *it;
	dump();
}

void Route::prepareFlank()
{
	// Collect and set new state for flank switches.
	flank_switches.clear();
	for (RailPart * part : track)
	{
		AbstractSwitch * flank_switch = dynamic_cast<AbstractSwitch *>(part);

		if (flank_switch != nullptr)
		{
			flank_switch->flank(flank_switches, true);
		}
	}
}

bool Route::isLastSectionEnded() const
{
	std::unordered_set<Section *> neighbours;

	isLastSectionEnded(neighbours, track.back(), direction);
	return neighbours.empty();
}

void Route::isLastSectionEnded(
	std::unordered_set<Section *> & neighbours,
	const RailPart         *        part,
	const bool                      direction)
{
	for (RailPart * next : part->advance(direction))
	{
		if (next->section() != part->section())
		{
			neighbours.emplace(next->section());
		}
		else
		{
			isLastSectionEnded(neighbours, next, direction);
		}
	}
}

void Route::clear()
{
	for (Section * section : sections)
	{
		section->free();
	}
	for (RailPart * part : track)
	{
		part->reserve(false);
	}
	first_section = nullptr;
	sections.clear();
	track.clear();
}

void Route::dump() const
{
	for (RailPart * part : track)
	{
		qDebug().noquote() << "     " << part->toString();
	}
	qDebug() << "---";
	for (Section * section : sections)
	{
		qDebug().noquote() << "     " << section->toString();
	}
}
