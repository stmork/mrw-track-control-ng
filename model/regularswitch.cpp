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

bool RegularSwitch::isBranch() const
{
	return right_branch == (switch_state == State::AC);
}

/*************************************************************************
**                                                                      **
**       State methods                                                  **
**                                                                      **
*************************************************************************/

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
		qWarning().noquote() << "Switch locked!" << name();
	}

#ifdef STATE_VERBOSE
	qDebug().noquote() << "####>" << toString() << switchState();
#endif
}

void RegularSwitch::setState(
	const RailPart * prev,
	const RailPart * succ)
{
	switch_state = computeState(prev, succ);

#ifdef STATE_VERBOSE
	qDebug().noquote() << "####C" << toString() << switchState();
#endif
}

State RegularSwitch::computeState(
	const RailPart * prev,
	const RailPart * succ) const
{
	if ((prev == nullptr) || (succ == nullptr))
	{
		throw std::invalid_argument("Given rail parts are not defined.");
	}

	if ((b == prev) || (b == succ))
	{
		return State::AB;
	}
	else if ((c == prev) || (c == succ))
	{
		return State::AC;
	}
	else
	{
		qCritical() << "a: " << *a;
		qCritical() << "b: " << *b;
		qCritical() << "c: " << *c;

		qCritical() << "Left: " << (prev != nullptr ? QString(*prev) : "<null>");
		qCritical() << "This: " << *this;
		qCritical() << "Right:" << (succ != nullptr ? QString(*succ) : "<null>");

		throw std::invalid_argument("Given rail parts are not neighbours.");
	}
}

SwitchState RegularSwitch::switchState() const
{
	return static_cast<SwitchState>(switch_state);
}

/*************************************************************************
**                                                                      **
**       Flank protection methods                                       **
**                                                                      **
*************************************************************************/

bool RegularSwitch::isFlankProtection(const AbstractSwitch * other) const
{
	const RegularSwitch * b_switch = follow(b);
	const RegularSwitch * c_switch = follow(c);

	return
		((b_switch == other) && isFlankCandidate(b_switch, true)) ||
		((c_switch == other) && isFlankCandidate(c_switch, false));
}

size_t RegularSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state) const
{
	return flank(switches, set_state, state());
}

size_t RegularSwitch::flankCandidates(
	std::vector<RegularSwitch *> & switches,
	const RailPart        *        prev,
	const RailPart        *        succ) const
{
	State compare = computeState(prev, succ);

	return flank(switches, false, compare);
}

size_t RegularSwitch::flank(
	std::vector<mrw::model::RegularSwitch *> & switches,
	const bool                                 set_state,
	const State                                compare) const
{
	RegularSwitch * b_switch = follow(b);
	RegularSwitch * c_switch = follow(c);
	RegularSwitch * other    = nullptr;
	size_t          equal    = 0;

	if ((compare == State::AB) && isFlankCandidate(c_switch, false))
	{
		other = c_switch;
	}
	else if ((compare == State::AC) && isFlankCandidate(b_switch, true))
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
		if (other->state() == compare)
		{
			equal++;
		}
	}
	return equal;
}

/*************************************************************************
**                                                                      **
**       Support methods                                                **
**                                                                      **
*************************************************************************/

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

QString RegularSwitch::key() const
{
	return "Weiche" + name();
}

QString RegularSwitch::get(const RegularSwitch::State & state)
{
	return state_map.get(state);
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
