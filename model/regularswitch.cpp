//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/regularswitch.h"

using namespace mrw::model;

RegularSwitch::RegularSwitch(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	AbstractSwitch(model_railway, element),
	left_branch( ModelRailway::boolean(element, "bIstAbzweig", false)),
	right_branch(ModelRailway::boolean(element, "cIstAbzweig", false)),
	left_prio(   ModelRailway::boolean(element, "bIstBevorzugt", false)),
	right_prio(  ModelRailway::boolean(element, "cIstBevorzugt", false))
{
}

void RegularSwitch::link()
{
	a = resolve("a");
	b = resolve("b");
	c = resolve("c");

	if ((a == nullptr) || (b == nullptr) || (c == nullptr))
	{
		model->error("Switch not completely connected: " + name());
		return;
	}

	advance( aIsDir()).insert(RailInfo(a));
	advance(!aIsDir()).insert(RailInfo(b, left_prio,  left_branch));
	advance(!aIsDir()).insert(RailInfo(c, right_prio, right_branch));

	findFlankSwitches();
}

void mrw::model::RegularSwitch::findFlankSwitches()
{
	RegularSwitch * left  = dynamic_cast<RegularSwitch *>(a);
	RegularSwitch * right = dynamic_cast<RegularSwitch *>(b);

	if ((left != nullptr) && (left->a == this))
	{
		flank_switches.push_back(left);
	}
	if ((right != nullptr) && (right->b == this))
	{
		flank_switches.push_back(right);
	}
}

bool RegularSwitch::valid() const
{
	return
		(inductors() == 2) &&
		(a != nullptr) && a->contains(this,  aIsDir()) &&
		(b != nullptr) && b->contains(this, !aIsDir()) &&
		(c != nullptr) && c->contains(this, !aIsDir());
}

QString RegularSwitch::toString() const
{
	return QString("      %1 %2 %3--%4 : %5").
		arg(aIsDir() ? ">" : "<").
		arg(valid()  ? "V" : "-").
		arg(aIsDir() ? "bc" : " a").
		arg(aIsDir() ? "a " : "bc").
		arg(name());
}
