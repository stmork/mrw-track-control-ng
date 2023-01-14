//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/lightmodule.h"
#include "model/profilelight.h"

using namespace mrw::can;
using namespace mrw::model;

LightModule::LightModule(
	ModelRailway     *    model_railway,
	Controller      *     controller,
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
				ProfileLight * light = new ProfileLight(model, controller, child);

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

bool LightModule::valid() const
{
	return std::all_of(lights.begin(), lights.end(), [] (const Light * light)
	{
		return light->controller() != nullptr;
	}) &&
	(lights.size() <= MAX_LIGHTS);
}

void LightModule::configure(
	std::vector<mrw::can::MrwMessage> & messages,
	const unsigned                      offset) const
{
	unsigned pin = offset;

	for (ProfileLight * light : lights)
	{
		const MrwMessage msg = light->configMsg(pin);

		messages.emplace_back(msg);
		pin++;
	}
}

void LightModule::link()
{
}
