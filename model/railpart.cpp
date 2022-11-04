//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/railpart.h"

using namespace mrw::model;

RailPart::RailPart(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	AssemblyPart(model_railway, element),
	a_in_dir(ModelRailway::boolean(element, "aInZaehlrichtung"))
{
}

RailPart * RailPart::resolve(const char * attr) const
{
	const QString & value = reference.attribute(attr, "");

	return dynamic_cast<RailPart *>(AssemblyPart::resolve(model, value));
}
