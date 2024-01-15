//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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

bool RegularSwitch::valid() const noexcept
{
	return
		(inductors() == 2) &&
		((advance(false).size() + advance(true).size()) == 3) &&
		(a != nullptr) && a->contains(this,  aIsDir()) &&
		(b != nullptr) && b->contains(this, !aIsDir()) &&
		(c != nullptr) && c->contains(this, !aIsDir());
}

bool RegularSwitch::isCurved() const noexcept
{
	return right_branch == (switch_state == State::AC);
}

/*************************************************************************
**                                                                      **
**       State methods                                                  **
**                                                                      **
*************************************************************************/

RegularSwitch::State RegularSwitch::state() const noexcept
{
	return switch_state;
}

bool RegularSwitch::setState(const State state, const bool force) noexcept
{
	bool success = true;

	if (state == State::FAIL)
	{
		setLock(LockState::FAIL);
		success = false;
	}
	else if ((lock() == LockState::UNLOCKED) || force)
	{
		switch_state = state;
	}
	else if (switch_state != state)
	{
		qWarning().noquote() << "Switch locked!" << name();
		success = false;
	}

#ifdef STATE_VERBOSE
	qDebug().noquote() << "####>" << toString() << switchState();
#endif
	return success;
}

bool RegularSwitch::isSwitchable(
	const RailPart * prev,
	const RailPart * succ) const noexcept
{
	const State state = computeState(prev, succ);

	return (lock() == LockState::UNLOCKED) || (state == switch_state);
}

bool RegularSwitch::setState(
	const RailPart * prev,
	const RailPart * succ)
{
	const State state = computeState(prev, succ);

	return setState(state);

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

SwitchState RegularSwitch::switchState() const noexcept
{
	return static_cast<SwitchState>(switch_state);
}

/*************************************************************************
**                                                                      **
**       Flank protection methods                                       **
**                                                                      **
*************************************************************************/

void RegularSwitch::collectFlankSwitches() noexcept
{
	flank_switches.push_back(nullptr);
	flank_switches.push_back(follow(b, !a_in_dir, true));
	flank_switches.push_back(follow(c, !a_in_dir, false));
}

size_t RegularSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state) const noexcept
{
	return flank(switches, set_state, state());
}

size_t RegularSwitch::flankCandidates(
	std::vector<RegularSwitch *> & switches,
	const RailPart        *        prev,
	const RailPart        *        succ) const noexcept
{
	State compare = computeState(prev, succ);

	return flank(switches, false, compare);
}

size_t RegularSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state,
	const State                    compare) const noexcept
{
	RegularSwitch * other = nullptr;
	size_t          equal = 0;

	if (compare == State::AB)
	{
		other = flank_switches[(unsigned)State::AC];
	}
	else if (compare == State::AC)
	{
		other = flank_switches[(unsigned)State::AB];
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

QString RegularSwitch::key() const noexcept
{
	return "Weiche" + name();
}

QString RegularSwitch::get(const RegularSwitch::State & state)
{
	return state_map.get(state);
}

QString RegularSwitch::toString() const noexcept
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
