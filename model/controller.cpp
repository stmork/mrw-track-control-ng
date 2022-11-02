//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDomDocument>
#include <QDebug>

#include "model/mrwmodel.h"

#include "model/controller.h"

#include "model/module.h"
#include "model/multiplexconnection.h"

#include "model/sectionmodule.h"
#include "model/switchmodule.h"
#include "model/lightmodule.h"

using namespace mrw::model;

Controller::Controller(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	controller_id(ModelRailway::value(element, "id")),
	model(model_railway)
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child     = node.toElement();
			const QString   &   node_name = child.nodeName();
			const QString       type      = ModelRailway::type(child);

			if (node_name == "module")
			{
				Module * module = nullptr;

				if (type == "Gleismodul")
				{
					module = new SectionModule(model, child);
				}
				else if (type == "Impulsmodul")
				{
					module = new SwitchModule(model, child);
				}
				else if (type == "Beleuchtungsmodul")
				{
					module = new LightModule(model, child);
				}
				else
				{
					model->error("Unknown module type: " + type);
				}
				add(module);
			}
			else if (node_name == "anschluesse")
			{
				MultiplexConnection * connection = new MultiplexConnection(model, child);

				add(connection);
			}
			else
			{
				model->error("Unknown controller element: " + node_name);
			}
		}
	}
}

Controller::~Controller()
{
	for (Module * module : modules)
	{
		delete module;
	}

	for (MultiplexConnection * connection : connections)
	{
		delete connection;
	}

	modules.clear();
	connections.clear();
}

void Controller::add(Module * module)
{
	if (module != nullptr)
	{
		modules.push_back(module);
	}
}

void Controller::add(MultiplexConnection * connection)
{
	connections.push_back(connection);
}

void Controller::link()
{
	for (Module * module : modules)
	{
		module->link();
	}

	for (MultiplexConnection * connection : connections)
	{
		connection->link();
	}
}

QString Controller::toString() const
{
	return QString::asprintf("  Controller %03d", controller_id);
}
