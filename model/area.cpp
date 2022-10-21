//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/area.h"

#include "model/section.h"

using namespace mrw::model;

Area::Area(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	name(element.attribute("name")),
	model(model_railway)
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child = node.toElement();
			const QString       type  = child.attribute("xsi:type");

			if (node.nodeName() == "abschnitt")
			{
				Section * section = new Section(model, child);

				add(section);
			}
			else
			{
				qCritical() << "Unknown group area element" << node.nodeName();
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
