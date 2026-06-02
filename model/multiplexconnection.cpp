//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QStringList>

#include <util/stringutil.h>
#include <model/modelrailway.h>
#include <model/multiplexconnection.h>
#include <model/light.h>
#include <model/lightsignal.h>

using namespace mrw::can;
using namespace mrw::model;

MultiplexConnection::MultiplexConnection(
	ModelRailway     *    model_railway,
	Controller      *     controller,
	const QDomElement  &  element) :
	model(model_railway),
	reference(element),
	connection_id(ModelRailway::value(element, "nummer"))
{
	const QDomNodeList & child_nodes = element.childNodes();

	simple_light_vector.reserve(child_nodes.count());
	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child     = node.toElement();
			const QString   &   node_name = child.nodeName();

			if (node_name == "lichter")
			{
				Light * light = new Light(model, controller, child);

				simple_light_vector.push_back(light);
			}
			else if (node_name == "crossing")
			{
				Crossing * crossing = new Crossing(model, controller, child);

				crossing_vector.push_back(crossing);
			}
			else
			{
				model->error("Unknown multiplex element: " + node_name);
			}
		}
	}
}

bool MultiplexConnection::valid() const
{
	size_t pins = simple_light_vector.size();

	for (LightSignal * signal : light_signal_vector)
	{
		if (signal == nullptr)
		{
			return false;
		}
		if (!signal->valid())
		{
			return false;
		}
		pins += signal->usedPins();
	}

	for (Crossing * crossing : crossing_vector)
	{
		if (!crossing->valid())
		{
			return false;
		}
		pins++;
	}

	return pins <= MAX_PINS;
}

bool MultiplexConnection::isValid(const MultiplexConnection * conn) noexcept
{
	return (conn != nullptr) && (conn->valid());
}

const std::vector<Light *> & MultiplexConnection::lights() const
{
	return simple_light_vector;
}

const std::vector<Crossing *> & mrw::model::MultiplexConnection::crossings() const
{
	return crossing_vector;
}

void MultiplexConnection::configure(
	std::vector<MrwMessage> & messages,
	const unsigned            offset) const
{
	unsigned pin = offset;

	for (const LightSignal * light_signal : light_signal_vector)
	{
		const MrwMessage msg = light_signal->configMsg(pin);

		messages.emplace_back(msg);
		pin += light_signal->usedPins();
	}

	for (const Light * light : simple_light_vector)
	{
		const MrwMessage msg = light->configMsg(pin);

		messages.emplace_back(msg);
		pin++;
	}

	for (const Crossing * crossing : crossing_vector)
	{
		const MrwMessage msg = crossing->configMsg(pin);

		messages.emplace_back(msg);
		pin++;
	}
}

void MultiplexConnection::link()
{
	QStringList references = reference.attribute("lichtsignale").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & signal_reference : references)
	{
		LightSignal * signal = dynamic_cast<LightSignal *>(AssemblyPart::resolve(model, signal_reference));

		light_signal_vector.push_back(signal);
	}
}
