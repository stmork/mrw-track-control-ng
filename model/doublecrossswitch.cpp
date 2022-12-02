//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/doublecrossswitch.h"
#include "model/regularswitch.h"

using namespace mrw::can;
using namespace mrw::model;

DoubleCrossSwitch::DoubleCrossSwitch(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	AbstractSwitch(model_railway, model_section, element),
	ad_branch(ModelRailway::boolean(element, "adIstAbzweig")),
	bc_branch(ModelRailway::boolean(element, "bcIstAbzweig"))
{
}

void DoubleCrossSwitch::setState(
	const RailPart * left,
	const RailPart * right)
{
	unsigned state = 0;

	if ((b == left) || (b == right))
	{
		state |= B_MASK;
	}

	if ((d == left) || (d == right))
	{
		state |= D_MASK;
	}

	switch_state = static_cast<State>(state);
}

void DoubleCrossSwitch::link()
{
	a = resolve("a");
	b = resolve("b");
	c = resolve("c");
	d = resolve("d");

	if ((a == nullptr) || (b == nullptr) || (c == nullptr) || (d == nullptr))
	{
		part_model->error("Cross switch not completely connected: " + name());
		return;
	}

	advance( aIsDir()).insert(RailInfo(a, false, ad_branch));
	advance( aIsDir()).insert(RailInfo(b, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(c, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(d, false, ad_branch));
}

void DoubleCrossSwitch::findFlankSwitches()
{
	RegularSwitch * a_switch = dynamic_cast<RegularSwitch *>(a);
	RegularSwitch * b_switch = dynamic_cast<RegularSwitch *>(b);
	RegularSwitch * c_switch = dynamic_cast<RegularSwitch *>(c);
	RegularSwitch * d_switch = dynamic_cast<RegularSwitch *>(d);

	if ((a_switch != nullptr) && (a_switch->c == this))
	{
		flank_switches.insert(a_switch);
	}
	if ((b_switch != nullptr) && (b_switch->b == this))
	{
		flank_switches.insert(b_switch);
	}
	if ((c_switch != nullptr) && (c_switch->b == this))
	{
		flank_switches.insert(c_switch);
	}
	if ((d_switch != nullptr) && (d_switch->c == this))
	{
		flank_switches.insert(d_switch);
	}
}

bool DoubleCrossSwitch::valid() const
{
	return
		(inductors() == 2) &&
		((advance(false).size() + advance(true).size()) == 4) &&
		(a != nullptr) && a->contains(this, aIsDir()) &&
		(b != nullptr) && b->contains(this, aIsDir()) &&
		(c != nullptr) && c->contains(this, !aIsDir()) &&
		(d != nullptr) && d->contains(this, !aIsDir());
}

QString DoubleCrossSwitch::toString() const
{
	return QString("      X %1 %2--%3 : [%4] %5").
		arg(valid()  ? "V" : "-").
		arg(aIsDir() ? "cd" : "ab").
		arg(aIsDir() ? "ab" : "cd").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name());
}

QString DoubleCrossSwitch::key() const
{
	return "DKW" + name();
}

SwitchState DoubleCrossSwitch::commandState() const
{
	const bool b_active = unsigned(switch_state) & B_MASK;
	const bool d_active = unsigned(switch_state) & D_MASK;

	return b_active == d_active ?
		SwitchState::SWITCH_STATE_LEFT :
		SwitchState::SWITCH_STATE_RIGHT;
}
