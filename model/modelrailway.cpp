//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QFile>
#include <QDomElement>

#include "util/method.h"
#include "model/modelrailway.h"
#include "model/assemblypart.h"
#include "model/abstractswitch.h"
#include "model/signal.h"

using namespace mrw::can;
using namespace mrw::model;

using SignalType = Signal::SignalType;

Q_LOGGING_CATEGORY(mrw::model::log, "mrw.model")

ModelRailway::ModelRailway(const QString & filename)
{
	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		xml_doc.setContent(&file);
		file.close();

		create();
		link();
		initStatistics();
	}
}

void ModelRailway::xml() const
{
	xml(xml_doc.documentElement());
}

void ModelRailway::create()
{
	__METHOD__;

	const QDomElement  & model = xml_doc.documentElement();
	const QDomNodeList & child_nodes = model.childNodes();

	name = model.attribute("name");

	controllers.reserve(child_nodes.count());
	regions.reserve(child_nodes.count());
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

				qCDebug(log).noquote() << *controller;
			}
			else if (node_name == "gruppe")
			{
				Region * region = new Region(this, child.toElement(), type(child) == "Bahnhof");

				regions.push_back(region);
				qCDebug(log).noquote() << *region;
			}
			else
			{
				error("Unknown node name: " + child.nodeName());
			}
		}
	}
	qCDebug(log) << *this;
}

void ModelRailway::link()
{
	std::vector<AbstractSwitch *> switches;

	for (Controller * controller : controllers)
	{
		controller->link();
	}
	for (Region * region : regions)
	{
		region->link();
	}

	parts<AbstractSwitch>(switches);
	for (AbstractSwitch * as : switches)
	{
		as->collectFlankSwitches();
	}
}

void ModelRailway::initStatistics()
{
	std::vector<AbstractSwitch *> switches;
	std::vector<Signal *>         signal_s;

	parts<AbstractSwitch>(switches);
	parts<Signal>(signal_s);

	model_statistics.region_count      = regions.size();
	model_statistics.switch_count      = switches.size();
	model_statistics.signal_count      = signal_s.size();
	model_statistics.main_signal_count = std::count_if(
			signal_s.begin(), signal_s.end(),
			[](const Signal * signal)
	{
		const SignalType type = signal->type();

		return
			(type == SignalType::MAIN_SIGNAL) ||
			(type == SignalType::MAIN_SHUNT_SIGNAL);
	});

	for (const Region * region : regions)
	{
		model_statistics.section_count += region->sectionCount();

		for (const Section * section : region->sections)
		{
			// Backward direction
			if (section->getSignals(false).size() > 0)
			{
				model_statistics.signal_group_count++;
			}

			// Forward direction
			if (section->getSignals(true).size() > 0)
			{
				model_statistics.signal_group_count++;
			}
		}
	}

	model_statistics.device_count =
		model_statistics.switch_count +
		model_statistics.section_count +
		model_statistics.signal_count;
}

void ModelRailway::info()
{
	qCInfo(log) << *this;

	for (Controller * controller : controllers)
	{
		qCInfo(log) << *controller;
	}
	for (Region * region : regions)
	{
		qCInfo(log) << *region;

		for (size_t s = 0; s < region->sectionCount(); s++)
		{
			Section * section = region->section(s);

			qCInfo(log) << *section;

			for (size_t a = 0; a < section->assemblyPartCount(); a++)
			{
				AssemblyPart * part = section->assemblyPart(a);

				if (part != nullptr)
				{
					qCInfo(log) << *part;
				}
				else
				{
					qCInfo(log, "      <-nil->");
				}
			}
		}
	}
}

Controller * ModelRailway::controller(const size_t index) const
{
	return controllers.at(index);
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

unsigned ModelRailway::value(
	const QDomElement  &  node,
	const char      *     attr,
	const unsigned        default_value)
{
	return node.attribute(attr, QString::number(default_value)).toUInt();
}

QString ModelRailway::string(
	const QDomElement  &  node,
	const char      *     attr)
{
	return node.attribute(attr);
}

void ModelRailway::warning(const QString & message)
{
	model_statistics.warnings++;
	qCWarning(log).noquote() << message;
}

void ModelRailway::error(const QString & message)
{
	model_statistics.errors++;
	qCCritical(log).noquote() << message;
}

bool ModelRailway::valid() const
{
	const bool is_valid = std::all_of(controllers.begin(), controllers.end(), &Controller::isValid);

	return is_valid && (model_statistics.errors == 0);
}

void ModelRailway::add(Controller * controller)
{
	const ControllerId id = controller->id();

	controllers.push_back(controller);
	if (controller_map.find(id) == controller_map.end())
	{
		controller_map.emplace(id, controller);
	}
	else
	{
		error(QString(
				"There is already a registered controller with id: %1!").arg(id));
	}
}

void ModelRailway::add(Device * device)
{
	const DeviceId id = DeviceId(device);
	const auto   it = device_map.find(id);

	if (it == device_map.end())
	{
		device_map.emplace(id, device);
	}
	else
	{
		Device * other = it->second;

		error(QString("Devices %1 and %2/%3 have both the same unit no: %2!").
			arg(other->name()).arg(id.first).arg(id.second));
	}
}

QString ModelRailway::toString() const
{
	return QString("Modelrailway %1 with %2 controllers and %3 region(s), %4 warnings and %5 errors.").
		arg(name).arg(controllers.size()).arg(regions.size()).
		arg(model_statistics.warnings).arg(model_statistics.errors);
}

const MrwStatistic & ModelRailway::statistics() const
{
	return model_statistics;
}

bool ModelRailway::boolean(const QDomElement & node, const char * attr, const bool default_value)
{
	return node.attribute(attr, default_value ? "true" : "false") == "true";
}

void ModelRailway::xml(const QDomNode & node, const QString & indent) const
{
	const QDomNodeList & child_nodes = node.childNodes();

	qCDebug(log).noquote().nospace() << indent << "<" << node.nodeName() << ">";

	if (node.isElement())
	{
		const QDomElement    &   element    = node.toElement();
		const QDomNamedNodeMap & attributes = element.attributes();

		for (int a = 0; a < attributes.size(); a++)
		{
			const QDomAttr & attribute = attributes.item(a).toAttr();
			const QString  & out       = QString("%0    %1 = %2").arg(indent).arg(attribute.name()).arg(attribute.value());

			qCDebug(log).noquote().nospace() << out;
		}
	}

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & child = child_nodes.at(n);

		xml(child, indent + "  ");
	}
	qCDebug(log).noquote().nospace() << indent << "</" << node.nodeName() << ">";
}

Controller * ModelRailway::controllerById(const ControllerId id) const
{
	auto it = controller_map.find(id);

	return it != controller_map.end() ? it->second : nullptr;
}

Device * ModelRailway::deviceById(
	const ControllerId id,
	const UnitNo       unit_no) const
{
	auto it = device_map.find(DeviceKey(id, unit_no));

	return it != device_map.end() ? it->second : nullptr;
}

size_t ModelRailway::controllerCount() const
{
	return controllers.size();
}

Module * ModelRailway::module(const size_t controller_idx, const size_t module_idx) const
{
	return controller(controller_idx)->module(module_idx);
}

MultiplexConnection * ModelRailway::connection(
	const size_t controller_idx,
	const size_t connection_idx) const
{
	return controller(controller_idx)->connection(connection_idx);
}

Region * ModelRailway::region(const size_t index) const
{
	return regions.at(index);
}

size_t ModelRailway::regionCount() const
{
	return regions.size();
}

Section * ModelRailway::section(
	const size_t region_idx,
	const size_t section_idx) const
{
	return region(region_idx)->section(section_idx);
}

AssemblyPart * ModelRailway::assemblyPart(
	const size_t region_idx,
	const size_t section_idx,
	const size_t part_idx) const
{
	return section(region_idx, section_idx)->assemblyPart(part_idx);
}

DeviceId::DeviceId(const Device * device)
{
	const Controller * controller = device->controller();

	first  = controller != nullptr ? controller->id() : 0;
	second = device->unitNo();
}

std::size_t DeviceId::operator ()(const DeviceKey & id) const
{
	size_t result = id.first;

	return (result << 16) | id.second;
}
