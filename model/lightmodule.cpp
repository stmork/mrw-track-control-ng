//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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

	profile_lights.reserve(child_nodes.count());
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

				profile_lights.push_back(light);
			}
			else
			{
				model->error("Unknown light module element: " + node_name);
			}
		}
	}
}

bool LightModule::valid() const
{
	const bool is_valid = std::all_of(
			profile_lights.begin(), profile_lights.end(),
			&Light::hasController);

	return is_valid && (profile_lights.size() <= MAX_LIGHTS);
}

const std::vector<ProfileLight *> & LightModule::lights() const
{
	return profile_lights;
}

void LightModule::configure(
	std::vector<MrwMessage> & messages,
	const unsigned            offset) const
{
	unsigned pin = offset;

	for (ProfileLight * light : profile_lights)
	{
		const MrwMessage msg = light->configMsg(pin);

		messages.emplace_back(msg);
		pin++;
	}
}

void LightModule::link()
{
}
