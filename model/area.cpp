//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/area.h"
#include "model/section.h"

using namespace mrw::model;

Area::Area(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            station) :
	name(element.attribute("name")),
	is_station(station),
	model(model_railway)
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child = node.toElement();

			if (node.nodeName() == "abschnitt")
			{
				Section * section = new Section(model, child);

				add(section);
			}
			else
			{
				model->error("Unknown group area element: " + node.nodeName());
			}
		}
	}
}

Area::~Area()
{
	for (Section * section : sections)
	{
		delete section;
	}
	sections.clear();
}

void Area::add(Section * section)
{
	sections.push_back(section);
}

void mrw::model::Area::link()
{
	for (Section * section : sections)
	{
		section->link();
	}
}

QString mrw::model::Area::toString() const
{
	return QString("  Area %1 (%2)").arg(name).arg(is_station ? "Station" : "rail road");
}
