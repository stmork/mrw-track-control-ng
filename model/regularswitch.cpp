//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/regularswitch.h"

using namespace mrw::model;

RegularSwitch::RegularSwitch(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	AbstractSwitch(model_railway, model_section, element),
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
		part_model->error("Switch not completely connected: " + name());
		return;
	}

	advance( aIsDir()).insert(RailInfo(a));
	advance(!aIsDir()).insert(RailInfo(b, left_prio,  left_branch));
	advance(!aIsDir()).insert(RailInfo(c, right_prio, right_branch));

	findFlankSwitches();
}

void RegularSwitch::findFlankSwitches()
{
	RegularSwitch * b_switch = dynamic_cast<RegularSwitch *>(b);
	RegularSwitch * c_switch = dynamic_cast<RegularSwitch *>(c);

	if ((b_switch != nullptr) && (b_switch->b == this))
	{
		flank_switches.insert(b_switch);
	}
	if ((c_switch != nullptr) && (c_switch->c == this))
	{
		flank_switches.insert(c_switch);
	}
}

bool RegularSwitch::valid() const
{
	return
		(inductors() == 2) &&
		((advance(false).size() + advance(true).size()) == 3) &&
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

QString mrw::model::RegularSwitch::key() const
{
	return "Weiche" + name();
}
