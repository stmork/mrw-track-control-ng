//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	if (ad_branch == bc_branch)
	{
		// Ensure that branches are not equal anyway.
		bc_branch = !ad_branch;
	}
}

void DoubleCrossSwitch::setState(
	const RailPart * left,
	const RailPart * right)
{
	unsigned state = 0;

	if ((left == nullptr) || (right == nullptr))
	{
		throw std::invalid_argument("Given rail parts are not defined.");
	}

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

QString DoubleCrossSwitch::get(const DoubleCrossSwitch::State & state)
{
	return state_map.get(state);
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

bool DoubleCrossSwitch::isFlankProtection(const RailPart * other) const
{
	const RegularSwitch * a_switch = follow(a);
	const RegularSwitch * b_switch = follow(b);
	const RegularSwitch * c_switch = follow(c);
	const RegularSwitch * d_switch = follow(d);

	if (other == a_switch)
	{
		if ((a_switch != nullptr) && linked(a_switch->c, this))
		{
			return true;
		}
	}

	if (other == b_switch)
	{
		if ((b_switch != nullptr) && linked(b_switch->b, this))
		{
			return true;
		}
	}

	if (other == c_switch)
	{
		if ((c_switch != nullptr) && linked(c_switch->b, this))
		{
			return true;
		}
	}

	if (other == d_switch)
	{
		if ((d_switch != nullptr) && linked(d_switch->c, this))
		{
			return true;
		}
	}
	return false;
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state) const
{
	RegularSwitch * a_switch = follow(a);
	RegularSwitch * b_switch = follow(b);
	RegularSwitch * c_switch = follow(c);
	RegularSwitch * d_switch = follow(d);
	size_t          equal    = 0;

	if ((unsigned)state() & B_MASK)
	{
		if (isFlankProtection(a_switch))
		{
			switches.push_back(a_switch);
			if (set_state)
			{
				a_switch->setState(RegularSwitch::State::AB);
			}
			if (a_switch->switchState() == SWITCH_STATE_LEFT)
			{
				equal++;
			}
		}
	}
	else
	{
		if (isFlankProtection(b_switch))
		{
			switches.push_back(b_switch);
			if (set_state)
			{
				b_switch->setState(RegularSwitch::State::AC);
			}
			if (b_switch->switchState() == SWITCH_STATE_RIGHT)
			{
				equal++;
			}
		}
	}

	if ((unsigned)state() & D_MASK)
	{
		if (isFlankProtection(c_switch))
		{
			switches.push_back(c_switch);
			if (set_state)
			{
				c_switch->setState(RegularSwitch::State::AC);
			}
			if (c_switch->switchState() == SWITCH_STATE_RIGHT)
			{
				equal++;
			}
		}
	}
	else
	{
		if (isFlankProtection(d_switch))
		{
			switches.push_back(d_switch);
			if (set_state)
			{
				d_switch->setState(RegularSwitch::State::AB);
			}
			if (d_switch->switchState() == SWITCH_STATE_LEFT)
			{
				equal++;
			}
		}
	}
	return equal;
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
	static const QString R = String::format(String::BOLD_ON + String::RED_ON, "R");

	return QString("      X %1%2 %3--%4 : [%5] %6 %7 %8").
		arg(valid()  ? "V" : "-").
		arg(reserved() ? R : "-").
		arg(aIsDir() ? "cd" : "ab").
		arg(aIsDir() ? "ab" : "cd").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(state_map.get(switch_state));
}

QString DoubleCrossSwitch::key() const
{
	return "DKW" + name();
}

SwitchState DoubleCrossSwitch::switchState() const
{
	return isBranch() ?
		SwitchState::SWITCH_STATE_LEFT :
		SwitchState::SWITCH_STATE_RIGHT;
}

bool DoubleCrossSwitch::isBranch() const
{
	const bool b_active = unsigned(switch_state) & B_MASK;
	const bool d_active = unsigned(switch_state) & D_MASK;

	return b_active == d_active;
}

void DoubleCrossSwitch::setState(const State state, const bool force)
{
	if ((lock() == LockState::UNLOCKED) || force)
	{
#ifdef STATE_VERBOSE
		qDebug().noquote() << "########## DCS set state: " << state_map.get(switch_state) << " => " << state_map.get(state) << name();
#endif
		switch_state = state;
	}
	else
	{
		qWarning().noquote() << "Switch locked!" << name();
	}
}

State DoubleCrossSwitch::state() const
{
#ifdef STATE_VERBOSE
	qDebug().noquote() << "DCS get state: " << state_map.get(switch_state) << name();
#endif

	return switch_state;
}
