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
	qInfo().noquote() << "## First way point:" << first->toString();
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

	qInfo().noquote() << "## Next way point: " << target->toString();

	last_valid_part    = track.back();

	Region * search_region = findSearchRegion(target);
	bool     success       = append(last_valid_part, target, search_region);

	if (success)
	{
		success = prepare();
		if (!success)
		{
			unreserveTail(last_valid_part);
		}
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
				if (hasFlankProtection(rail, next))
				{
					return true;
				}
				else
				{
					unreserveTail(next);
				}
			}
			track.remove(next);
			next->reserve(false);
		}
	}

	return false;
}

bool Route::hasFlankProtection(
	const RailPart * prev,
	const RailPart * rail) const
{
	const AbstractSwitch * actual_switch = dynamic_cast<const AbstractSwitch *>(rail);

	if ((state == SectionState::TOUR) && (actual_switch != nullptr))
	{
		const QString                indent(track.size(), ' ');
		auto                         it   = std::find(track.begin(), track.end(), rail);
		const RailPart       *       succ = *(++it);
		std::vector<RegularSwitch *> flank_switch_candidates;

		// Collect flank switches depending on wanted switch state. Do not turn
		// neither this switch nor flank switches. Flank switches not in correct
		// state will not counted as return parameter. So if return value and
		// result vector size differ any flank switch has to be turned later.
		const size_t  count = actual_switch->flankCandidates(flank_switch_candidates, prev, succ);

		// Count unlocked flank switches.
		const size_t  unlock_count = std::count_if(
				flank_switch_candidates.begin(), flank_switch_candidates.end(),
				[this](RegularSwitch * flank_switch)
		{
			return flank_switch->lock() == LockState::UNLOCKED;
		});

		// If all flank switches are unlocked everything is fine. Otherwise the
		// switch state has to be corrected.
		if ((unlock_count != flank_switch_candidates.size()) &&
			(count != flank_switch_candidates.size()))
		{
			qDebug().noquote() << indent << "      Flank protection not granted:";
			return false;
		}
	}

	return true;
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
		qDebug().noquote() << indent << "      Rail in failed state.";
		return false;
	}
	if (rail->reserved())
	{
		qDebug().noquote() << indent << "      Rail already reserved.";
		return false;
	}
	if (track.size() > MAX_DEPTH)
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
		else if (section->occupation())
		{
			qDebug().noquote() << indent << "      Section occupied.";
			return false;
		}
	}

	return true;
}

void Route::unreserveTail(const RailPart * actual)
{
	auto                    it = std::find(track.begin(), track.end(), actual);
	std::vector<RailPart *> removables(++it, track.end());

	for (RailPart * part : removables)
	{
		part->reserve(false);
		track.remove(part);
	}
}

bool Route::prepare()
{
	__METHOD__;

	std::vector<RailPart *> vector(track.begin(), track.end());
	Section        *        prev = nullptr;

	if (!isSwitchable(vector))
	{
		return false;
	}

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
	return true;
}

bool mrw::model::Route::isSwitchable(const std::vector<RailPart *> & vector) const
{
	for (size_t i = 1; i < vector.size() - 1; i++)
	{
		AbstractSwitch * device = dynamic_cast<AbstractSwitch *>(vector[i]);

		if ((device != nullptr) &&
			(!device->isSwitchable(vector[i - 1], vector[i + 1])))
		{
			return false;
		}
	}
	return true;
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
