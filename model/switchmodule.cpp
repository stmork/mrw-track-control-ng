//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QStringList>

#include "model/switchmodule.h"
#include "model/railpart.h"

using namespace mrw::model;

SwitchModule::SwitchModule(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) : Module(model_railway, element)
{
}

void SwitchModule::link()
{
	QStringList references = reference.attribute("magnetartikel").split(' ', QString::SkipEmptyParts);

	for (const QString & part_reference : references)
	{
		RailPart * part = RailPart::resolve(model, part_reference);

		rail_parts.push_back(part);
	}
}
