//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/rail.h"

using namespace mrw::model;

Rail::Rail(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	RailPart(model_railway, element),
	is_main(  ModelRailway::boolean(element, "istHauptgleis")),
	is_branch(ModelRailway::boolean(element, "istAbzweig"))
{
}

void Rail::link()
{
	a = resolve("a");
	b = resolve("b");

	if ((a == nullptr) && (b == nullptr))
	{
		model->error("Rail not connected: " + name());
	}

	if (a != nullptr)
	{
		advance(aIsDir()).insert(a);
	}

	if (b != nullptr)
	{
		advance(!aIsDir()).insert(b);
	}
}

bool Rail::valid() const
{
	return (
			((a != nullptr) && a->contains(this,  aIsDir())) ||
			((b != nullptr) && b->contains(this, !aIsDir()))
		);
}

QString Rail::toString() const
{
	return QString("      # %1 %2--%3: %4").
		arg(valid() ? "V" : "-").
		arg(a == nullptr ? " |" : aIsDir() ? " a" : " b").
		arg(b == nullptr ? "| " : aIsDir() ? "b " : "a ").
		arg(name());
}
