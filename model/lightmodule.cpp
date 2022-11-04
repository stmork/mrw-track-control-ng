//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/lightmodule.h"
#include "model/profilelight.h"

using namespace mrw::model;

LightModule::LightModule(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) : Module(model_railway, element)
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child     = node.toElement();
			const QString   &   node_name = child.nodeName();

			if (node_name == "lampen")
			{
				ProfileLight * light = new ProfileLight(model, child);

				lights.push_back(light);
			}
			else
			{
				model->error("Unknown light module element: " + node_name);
			}
		}
	}
}

LightModule::~LightModule()
{
	for (ProfileLight * light : lights)
	{
		delete light;
	}
	lights.clear();
}

void LightModule::link()
{
}
