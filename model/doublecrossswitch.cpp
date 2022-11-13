//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/doublecrossswitch.h"
#include "model/regularswitch.h"

using namespace mrw::model;

DoubleCrossSwitch::DoubleCrossSwitch(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	AbstractSwitch(model_railway, element),
	ad_branch(ModelRailway::boolean(element, "adIstAbzweig")),
	bc_branch(ModelRailway::boolean(element, "bcIstAbzweig"))
{
}

void DoubleCrossSwitch::link()
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

	advance(aIsDir()).insert(RailInfo( a, false, ad_branch));
	advance(aIsDir()).insert(RailInfo( b, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(c, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(d, false, ad_branch));

	findFlankSwitches();
}

void mrw::model::DoubleCrossSwitch::findFlankSwitches()
{
	RegularSwitch * a_switch = dynamic_cast<RegularSwitch *>(a);
	RegularSwitch * b_switch = dynamic_cast<RegularSwitch *>(b);
	RegularSwitch * c_switch = dynamic_cast<RegularSwitch *>(c);
	RegularSwitch * d_switch = dynamic_cast<RegularSwitch *>(d);

	if ((a_switch != nullptr) && (a_switch->b == this))
	{
		flank_switches.push_back(a_switch);
	}
	if ((b_switch != nullptr) && (b_switch->a == this))
	{
		flank_switches.push_back(b_switch);
	}
	if ((c_switch != nullptr) && (c_switch->a == this))
	{
		flank_switches.push_back(c_switch);
	}
	if ((d_switch != nullptr) && (d_switch->b == this))
	{
		flank_switches.push_back(d_switch);
	}
}

bool DoubleCrossSwitch::valid() const
{
	return
		(a != nullptr) && a->contains(this, aIsDir()) &&
		(b != nullptr) && b->contains(this, aIsDir()) &&
		(c != nullptr) && c->contains(this, !aIsDir()) &&
		(d != nullptr) && d->contains(this, !aIsDir());
}

QString DoubleCrossSwitch::toString() const
{
	return QString("      X %1 %2--%3 : %4").
		arg(valid()  ? "V" : "-").
		arg(aIsDir() ? "cd" : "ab").
		arg(aIsDir() ? "ab" : "cd").
		arg(name());
}
