//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <stdexcept>

#include <model/modelrailway.h>
#include <model/regularswitch.h>

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;

using Bending = Position::Bending;
using State   = RegularSwitch::State;

const ConstantEnumerator<State>  RegularSwitch::state_map
{
	CONSTANT(State::AB),
	CONSTANT(State::AC)
};

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
	if (right_branch == left_branch)
	{
		// Ensure that branches are not equal anyway.
		right_branch = !left_branch;
	}

	if (right_branch)
	{
		setBending(Bending::RIGHT);
	}
	if (left_branch)
	{
		setBending(Bending::LEFT);
	}
}

bool RegularSwitch::isRightHanded() const
{
	return right_branch;
}

bool RegularSwitch::isBranch() const
{
	return isRightHanded() == (switch_state == State::AC);
}

RegularSwitch::State RegularSwitch::state() const
{
	return switch_state;
}

void RegularSwitch::setState(const State state, const bool force)
{
	if ((lock() == LockState::UNLOCKED) || force)
	{
		switch_state = state;
	}
	else
	{
		qWarning("Switch locked!");
	}
}

void RegularSwitch::setState(
	const RailPart * left,
	const RailPart * right)
{
	if ((left == nullptr) || (right == nullptr))
	{
		throw std::invalid_argument("Given rail parts are not defined.");
	}

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

		qCritical() << "Left: " << (left != nullptr ? QString(*left) : "<null>");
		qCritical() << "This: " << *this;
		qCritical() << "Right:" << (right != nullptr ? QString(*right) : "<null>");

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
	static const QString R = String::format(String::BOLD_ON + String::RED_ON, "R");

	return QString("      %1 %2%3 %4--%5 : [%6] %7 %8 %9").
		arg(aIsDir() ? ">" : "<").
		arg(valid()  ? "V" : "-").
		arg(reserved() ? R : "-").
		arg(aIsDir() ? "bc" : " a").
		arg(aIsDir() ? "a " : "bc").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(state_map.get(switch_state));
}

QString RegularSwitch::key() const
{
	return "Weiche" + name();
}

SwitchState RegularSwitch::switchState() const
{
	return static_cast<SwitchState>(switch_state);
}

QString RegularSwitch::get(const RegularSwitch::State & state)
{
	return state_map.get(state);
}

bool RegularSwitch::isFlankProtection(const RailPart * other) const
{
	const RegularSwitch * b_switch = follow(b);
	const RegularSwitch * c_switch = follow(c);

	if (other == b_switch)
	{
		if ((b_switch != nullptr) && linked(b_switch->b, this))
		{
			return true;
		}
	}

	if (other == c_switch)
	{
		if ((c_switch != nullptr) && linked(c_switch->c, this))
		{
			return true;
		}
	}

	return false;
}

size_t RegularSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state) const
{
	RegularSwitch * b_switch = follow(b);
	RegularSwitch * c_switch = follow(c);
	RegularSwitch * other    = nullptr;
	size_t          equal    = 0;

	if ((switch_state == State::AB) && isFlankProtection(c_switch))
	{
		other = c_switch;
	}
	else if ((switch_state == State::AC) && isFlankProtection(b_switch))
	{
		other = b_switch;
	}

	if (other != nullptr)
	{
		switches.push_back(other);
		if (set_state)
		{
			other->setState(state());
		}
		if (other->switchState() == switchState())
		{
			equal++;
		}
	}
	return equal;
}
