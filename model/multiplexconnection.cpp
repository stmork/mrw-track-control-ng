//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QStringList>

#include "util/stringutil.h"
#include "model/mrwmodel.h"
#include "model/multiplexconnection.h"
#include "model/light.h"
#include "model/lightsignal.h"

using namespace mrw::model;

MultiplexConnection::MultiplexConnection(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	model(model_railway),
	reference(element),
	connection_id(ModelRailway::value(element, "nummer"))
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child     = node.toElement();
			const QString   &   node_name = child.nodeName();

			if (node_name == "lichter")
			{
				Light *light = new Light(child);

				lights.push_back(light);
			}
			else
			{
				model->error("Unknown controller element: " + node_name);
			}
		}
	}
}

MultiplexConnection::~MultiplexConnection()
{
	for(Light * light : lights)
	{
		delete light;
	}
	lights.clear();
}

void mrw::model::MultiplexConnection::link()
{
	QStringList references = reference.attribute("lichtsignale").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & signal_reference : references)
	{
		LightSignal * signal = static_cast<LightSignal *>(RailPart::resolve(model, signal_reference));

		light_signals.push_back(signal);
	}
}
