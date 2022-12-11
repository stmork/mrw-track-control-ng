//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/region.h"
#include "model/section.h"

using namespace mrw::model;

Region::Region(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            station) :
	region_name(element.attribute("name")),
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
				Section * section = new Section(model, this, child);

				add(section);
			}
			else
			{
				model->error("Unknown group region element: " + node.nodeName());
			}
		}
	}
}

Region::~Region()
{
	for (Section * section : sections)
	{
		delete section;
	}
	sections.clear();
}

void Region::parse(QSettings & settings, const bool dir)
{
	direction_view = settings.value(key(), dir).toBool();
}

void Region::add(Section * section)
{
	sections.push_back(section);
}

void Region::link()
{
	for (Section * section : sections)
	{
		section->link();
		section->findSignalPair();
	}
}

QString Region::toString() const
{
	return QString("  Region %1 (%2) %3").
		arg(region_name).
		arg(is_station ? "Station" : "rail road").
		arg(direction_view ? "-->" : "<--");
}

QString Region::key() const
{
	QString result = (is_station ? "Bahnhof" : "Strecke") + region_name;

	return result.replace(" ", "");
}

void Region::write(QSettings & settings) const
{
	settings.setValue(key(), direction_view);
}
