//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <model/rail.h>

#include "route.h"

using namespace mrw::model;

Route::Route(
	const bool         dir,
	const SectionState wanted_state,
	RailPart     *     first,
	QObject      *     parent) :
	QObject(parent), direction(dir), state(wanted_state)
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
}

bool Route::extend(RailPart * target)
{
	__METHOD__;

	qDebug().noquote() << "---------->" << *target;

	return extend(track.back(), target);
}

bool Route::extend(RailPart * rail, RailPart * target)
{
	if (rail == target)
	{
		rail->reserve();
		return true;
	}

	for (const RailInfo & info : rail->advance(direction))
	{
		RailPart * next = info;

		if (qualified(next))
		{
			next->reserve();
			track.push_back(next);
			if (extend(next, target))
			{
				return true;
			}
			track.remove(next);
			next->reserve(false);
		}
	}

	return false;
}

void Route::prepare()
{
	__METHOD__;

	std::vector<RailPart *> vector(track.begin(), track.end());
	Section        *        prev = nullptr;

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
		act->setState(state);

		// Prepare switch turning.
		vector[i]->setState(
			i > 0 ? vector[i - 1] : nullptr,
			(i + 1) < vector.size() ? vector[i + 1] : nullptr);

		qDebug().noquote() << "     " << vector[i]->toString();
	}
	qDebug() << "---";
	for (Section * section : sections)
	{
		qDebug().noquote() << "     " << section->toString();
	}
}

void mrw::model::Route::clear()
{
	for (RailPart * rail : track)
	{
		rail->reserve(false);
		rail->section()->setState(SectionState::FREE);
	}
	track.clear();
}

bool Route::qualified(RailPart * rail) const
{
	const QString   indent(track.size(), ' ');
	const Section * section = rail->section();

	qDebug().noquote() << indent << rail->toString();

	if (rail->reserved())
	{
		qDebug().noquote() << indent << "      Rail already reserved:";
		return false;
	}

	if (track.size() > 50)
	{
		qDebug().noquote() << indent << "      Recursion depth reached.";
		return false;
	}

	if (section != first_section)
	{
		if ((state == SectionState::SHUNTING) &&
			(first_section->region() != section->region()))
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
