//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/rail.h>

#include "route.h"

using namespace mrw::model;

Route::Route(RailPart * first,
	bool     dir,
	QObject * parent) : QObject(parent), direction(dir)
{
	Section * section = first->section();
	bool      loop = true;

	do
	{
		const std::set<RailInfo> & back = first->advance(!dir);

		if (back.size() == 1)
		{
			RailPart * part = *back.begin();
			Rail   *   rail = dynamic_cast<Rail *>(part);

			if ((rail == nullptr) || (part->section() != section))
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
}

void Route::extend(RailPart * rail)
{
	Q_UNUSED(rail);
}
