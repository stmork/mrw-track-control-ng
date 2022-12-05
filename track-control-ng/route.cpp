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
	QObject      *     parent) : QObject(parent), direction(dir), state(wanted_state)
{
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

	track.push_back(first);
	list_item.setText(first_section->key());
	list_item.setData(USER_ROLE, QVariant::fromValue(this));
}

Route::operator QListWidgetItem * ()
{
	return &list_item;
}

bool Route::extend(RailPart * rail)
{
	__METHOD__;

	Section * section = rail->section();

	if (section != first_section)
	{
		if (section->state() != FREE)
		{
			return false;
		}
	}

	qDebug().noquote() << QString(track.size(), ' ') << rail->toString();

	section->setState(state);
	track.push_back(rail);
	for (RailPart * next : rail->advance(direction))
	{
		if (extend(next))
		{
			return true;
		}
	}
	track.remove(rail);
	section->setState(FREE);

	return false;
}

void Route::prepare()
{
	__METHOD__;

	std::vector<RailPart *> vector(track.begin(), track.end());

	for (size_t i = 0; i < vector.size(); i++)
	{
		vector[i]->setState(
			i > 0 ? vector[i - 1] : nullptr,
			i < vector.size() ? vector[i + 1] : nullptr);

		qDebug().noquote() << "     " << vector[i]->toString();
	}
}
