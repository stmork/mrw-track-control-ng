//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QStringList>

#include "util/stringutil.h"
#include "model/switchmodule.h"
#include "model/assemblypart.h"
#include "model/switchmodulereference.h"

using namespace mrw::can;
using namespace mrw::model;

SwitchModule::SwitchModule(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) : Module(model_railway, element)
{
}

size_t SwitchModule::ports() const
{
	for (const AssemblyPart * part : rail_parts)
	{
		const SwitchModuleReference * actor = dynamic_cast<const SwitchModuleReference *>(part);

		if ((actor != nullptr) && (actor->hasCutOff()))
		{
			return 2;
		}
	}
	return 1;
}

bool SwitchModule::valid() const
{
	size_t inductors = 0;

	for (const AssemblyPart * part : rail_parts)
	{
		const SwitchModuleReference * actor = dynamic_cast<const SwitchModuleReference *>(part);

		if (actor == nullptr)
		{
			return false;
		}
		inductors += actor->inductors();
	}

	return inductors <= 8;
}

void SwitchModule::configure(
	std::vector<MrwMessage> & messages,
	const unsigned            offset) const
{
	unsigned pin = offset;

	for (const AssemblyPart * part : rail_parts)
	{
		const SwitchModuleReference * ref = dynamic_cast<const SwitchModuleReference *>(part);

		if (ref != nullptr)
		{
			const MrwMessage msg = ref->configMsg(pin);

			messages.emplace_back(msg);
			pin += ref->inductors();
		}
	}
}

void SwitchModule::link()
{
	QStringList references = reference.attribute("magnetartikel").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & part_reference : references)
	{
		AssemblyPart * part = AssemblyPart::resolve(model, part_reference);

		rail_parts.push_back(part);
	}
}
