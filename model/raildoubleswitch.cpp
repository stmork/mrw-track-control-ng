//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/raildoubleswitch.h"

using namespace mrw::model;

RailDoubleSwitch::RailDoubleSwitch(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	RailPart(model_railway, element),
	Device(model_railway, element),
	SwitchModuleReference(model_railway, element),
	ad_branch(ModelRailway::boolean(element, "adIstAbzweig")),
	bc_branch(ModelRailway::boolean(element, "bcIstAbzweig"))
{
}

void RailDoubleSwitch::link()
{
	a = resolve("a");
	b = resolve("b");
	c = resolve("c");
	d = resolve("d");

	if ((a == nullptr) || (b == nullptr) || (c == nullptr) || (d == nullptr))
	{
		model->error("Cross switch not completely connected: " + name());
		return;
	}

	advance(aIsDir()).insert(a);
	advance(aIsDir()).insert(b);
	advance(!aIsDir()).insert(c);
	advance(!aIsDir()).insert(d);
}

bool RailDoubleSwitch::valid() const
{
	return
		a->contains(this, aIsDir()) &&
		b->contains(this, aIsDir()) &&
		c->contains(this, !aIsDir()) &&
		d->contains(this, !aIsDir());
}

QString RailDoubleSwitch::toString() const
{
	return QString("      X %1 %2--%3: %4").
			arg(valid() ? "V" : "-").
			arg(aIsDir() ? "cd" : "ab").
			arg(aIsDir() ? "ab" : "cd").
			arg(name());
}
