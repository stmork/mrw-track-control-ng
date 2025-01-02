//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QStringList>

#include "model/modelrailway.h"
#include "model/module.h"
#include "model/assemblypart.h"

using namespace mrw::model;

Module::Module(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	module_id(ModelRailway::value(element, "nummer")),
	model(model_railway),
	reference(element)
{
}
