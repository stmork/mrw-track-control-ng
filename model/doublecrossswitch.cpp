//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/modelrailway.h>
#include <model/doublecrossswitch.h>
#include <model/regularswitch.h>

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;

using State = DoubleCrossSwitch::State;

const ConstantEnumerator<State>  DoubleCrossSwitch::state_map
{
	CONSTANT(State::AC),
	CONSTANT(State::AD),
	CONSTANT(State::BC),
	CONSTANT(State::BD)
};

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
#ifdef STATE_VERBOSE
	qDebug().noquote() << "########## DCS compute state: " << state_map.get(switch_state) << name();
#endif
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
	RegularSwitch * a_switch = follow(a);
	RegularSwitch * b_switch = follow(b);
	RegularSwitch * c_switch = follow(c);
	RegularSwitch * d_switch = follow(d);

	if ((a_switch != nullptr) && linked(a_switch->c, this))
	{
		flank_switches.insert(a_switch);
	}
	if ((b_switch != nullptr) && linked(b_switch->b, this))
	{
		flank_switches.insert(b_switch);
	}
	if ((c_switch != nullptr) && linked(c_switch->b, this))
	{
		flank_switches.insert(c_switch);
	}
	if ((d_switch != nullptr) && linked(d_switch->c, this))
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
	return QString("      X %1%2 %3--%4 : [%5] %6 %7").
		arg(valid()  ? "V" : "-").
		arg(reserved() ? "R" : "-").
		arg(aIsDir() ? "cd" : "ab").
		arg(aIsDir() ? "ab" : "cd").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(state_map.get(switch_state));
}

QString DoubleCrossSwitch::key() const
{
	return "DKW" + name();
}

SwitchState DoubleCrossSwitch::switchState() const
{
	const bool b_active = unsigned(switch_state) & B_MASK;
	const bool d_active = unsigned(switch_state) & D_MASK;

	return b_active == d_active ?
		SwitchState::SWITCH_STATE_LEFT :
		SwitchState::SWITCH_STATE_RIGHT;
}

void DoubleCrossSwitch::setState(const State state)
{
	if (lock() == LockState::UNLOCKED)
	{
#ifdef STATE_VERBOSE
		qDebug().noquote() << "########## DCS set state: " << state_map.get(switch_state) << " => " << state_map.get(state) << name();
#endif
		switch_state = state;
	}
	else
	{
		qWarning("Switch locked!");
	}
}

State DoubleCrossSwitch::state() const
{
#ifdef STATE_VERBOSE
	qDebug().noquote() << "DCS get state: " << state_map.get(switch_state) << name();
#endif

	return switch_state;
}
