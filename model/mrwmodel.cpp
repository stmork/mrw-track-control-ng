//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QFile>
#include <QDebug>
#include <QDomElement>

#include "model/mrwmodel.h"
#include "util/method.h"

using namespace mrw::model;

ModelRailway::ModelRailway(const QString & filename)
{
	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		xml.setContent(&file);
		file.close();

		create();
		link();
	}
}

ModelRailway::~ModelRailway()
{
	for (Controller * controller : controllers)
	{
		delete controller;
	}
	for (Area * area : areas)
	{
		delete area;
	}

	controllers.clear();
	areas.clear();
}

void ModelRailway::dump() const
{
	dump(xml.documentElement());
}

void ModelRailway::create()
{
	__METHOD__;

	const QDomElement  & model = xml.documentElement();
	const QDomNodeList & child_nodes = model.childNodes();

	name = model.attribute("name");
	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child = node.toElement();
			const QString   &   node_name = child.nodeName();

			if (node_name == "controller")
			{
				Controller * controller = new Controller(this, child.toElement());

				add(controller);
				qDebug().noquote() << *controller;
			}
			else if (node_name == "gruppe")
			{
				Area * area = new Area(this, child.toElement(), type(child) == "Bahnhof");

				add(area);
				qDebug().noquote() << *area;
			}
			else
			{
				error("Unknown node name: " + child.nodeName());
			}
		}
	}
	qDebug() << *this;
}

void mrw::model::ModelRailway::link()
{
	for (Controller * controller : controllers)
	{
		controller->link();
	}
	for (Area * area : areas)
	{
		area->link();
	}
}

void ModelRailway::traverse() const
{
	traverse(xml.documentElement());
}

void ModelRailway::add(Controller * controller)
{
	controllers.push_back(controller);
}

void ModelRailway::add(Area * area)
{
	areas.push_back(area);
}

QString ModelRailway::type(const QDomElement & element)
{
	const QStringList type_list = element.attribute("xsi:type").split(":");

	if (type_list.size() >= 2)
	{
		Q_ASSERT(type_list.first() == "Modelrailway");

		return type_list[1];
	}
	return "";
}

unsigned ModelRailway::value(const QDomElement & node, const char * attr, const unsigned default_value)
{
	return node.attribute(attr, QString::number(default_value)).toUInt();
}

QString mrw::model::ModelRailway::string(const QDomElement & node, const char * attr)
{
	return node.attribute(attr);
}

void ModelRailway::warning(const QString & message)
{
	warnings++;
	qWarning().noquote() << message;
}

void ModelRailway::error(const QString & message)
{
	errors++;
	qCritical().noquote() << message;
}

QString mrw::model::ModelRailway::toString() const
{
	return QString("Modelrailway %1 with %2 controllers and %3 area(s).").arg(name).arg(controllers.size()).arg(areas.size());
}

bool ModelRailway::boolean(const QDomElement & node, const char * attr, const bool default_value)
{
	return node.attribute(attr, default_value ? "true" : "false") == "true";
}

void ModelRailway::dump(const QDomNode & node, const QString & indent) const
{
	const QDomNodeList & child_nodes = node.childNodes();

	qDebug().noquote().nospace() << indent << "<" << node.nodeName() << ">";

	if (node.isElement())
	{
		const QDomElement    &   element    = node.toElement();
		const QDomNamedNodeMap & attributes = element.attributes();

		for (int a = 0; a < attributes.size(); a++)
		{
			const QDomAttr & attribute = attributes.item(a).toAttr();
			const QString  & out       = QString("%0    %1 = %2").arg(indent).arg(attribute.name()).arg(attribute.value());

			qDebug().noquote().nospace() << out;
		}
	}

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & child = child_nodes.at(n);

		dump(child, indent + "  ");
	}
	qDebug().noquote().nospace() << indent << "</" << node.nodeName() << ">";
}

void ModelRailway::traverse(const QDomNode & node, const size_t level) const
{
	const QDomNodeList & child_nodes = node.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & child = child_nodes.at(n);

		traverse(child, level + 1);
	}
}
