//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <stdexcept>

#include <model/modelrailway.h>
#include <model/regularswitch.h>

using namespace mrw::can;
using namespace mrw::model;

using Bending = Position::Bending;

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
	if (right_branch)
	{
		setBending(Bending::RIGHT);
	}
	if (left_branch)
	{
		setBending(Bending::LEFT);
	}
}

void mrw::model::RegularSwitch::setState(const mrw::model::RegularSwitch::State state)
{
	switch_state = state;
}

void RegularSwitch::setState(
	const RailPart * left,
	const RailPart * right)
{
	if ((b == left) || (b == right))
	{
		switch_state = State::AB;
	}
	else if ((c == left) || (c == right))
	{
		switch_state = State::AC;
	}
	else
	{
		qCritical() << "a: " << *a;
		qCritical() << "b: " << *b;
		qCritical() << "c: " << *c;

		qCritical() << "Left: " << *left;
		qCritical() << "This: " << *this;
		qCritical() << "Right:" << *right;

		throw std::invalid_argument("Given rail parts are not neighbours.");
	}
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
	RegularSwitch * b_switch = follow(b);
	RegularSwitch * c_switch = follow(c);

	if ((b_switch != nullptr) && linked(b_switch->b, this))
	{
		flank_switches.insert(b_switch);
	}
	if ((c_switch != nullptr) && linked(c_switch->c, this))
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
	return QString("      %1 %2%3 %4--%5 : [%6] %7").
		arg(aIsDir() ? ">" : "<").
		arg(valid()  ? "V" : "-").
		arg(reserved() ? "R" : "-").
		arg(aIsDir() ? "bc" : " a").
		arg(aIsDir() ? "a " : "bc").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name());
}

QString RegularSwitch::key() const
{
	return "Weiche" + name();
}

SwitchState RegularSwitch::switchState() const
{
	return static_cast<SwitchState>(switch_state);
}
