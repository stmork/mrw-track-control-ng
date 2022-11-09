//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QStringList>

#include "util/stringutil.h"
#include "model/switchmodule.h"
#include "model/assemblypart.h"
#include "model/switchmodulereference.h"

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

		if ((actor == nullptr) && (actor->isNew()))
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

void SwitchModule::link()
{
	QStringList references = reference.attribute("magnetartikel").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & part_reference : references)
	{
		AssemblyPart * part = AssemblyPart::resolve(model, part_reference);

		rail_parts.push_back(part);
	}
}
