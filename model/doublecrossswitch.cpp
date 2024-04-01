//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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

	// Note: There is no priority definition in the model so prio is false.
	advance( aIsDir()).insert(RailInfo(a, false, ad_branch));
	advance( aIsDir()).insert(RailInfo(b, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(c, false, bc_branch));
	advance(!aIsDir()).insert(RailInfo(d, false, ad_branch));
}

bool DoubleCrossSwitch::valid() const noexcept
{
	return
		(inductors() == 2) &&
		((advance(false).size() + advance(true).size()) == 4) &&
		(a != nullptr) && a->contains(this, aIsDir()) &&
		(b != nullptr) && b->contains(this, aIsDir()) &&
		(c != nullptr) && c->contains(this, !aIsDir()) &&
		(d != nullptr) && d->contains(this, !aIsDir());
}

bool DoubleCrossSwitch::isCurved() const noexcept
{
	return isCurved(switch_state);
}

/*************************************************************************
**                                                                      **
**       State methods                                                  **
**                                                                      **
*************************************************************************/

State DoubleCrossSwitch::state() const
{
#ifdef STATE_VERBOSE
	qDebug().noquote() << "DCS get state: " << state_map.get(switch_state) << name();
#endif

	return switch_state;
}

bool DoubleCrossSwitch::setState(const State state, const bool force)
{
	bool success = true;

	if (state == State::FAIL)
	{
		setLock(LockState::FAIL);
		success = false;
	}
	else if ((lock() == LockState::UNLOCKED) || force)
	{
#ifdef STATE_VERBOSE
		qDebug().noquote() << "########## DCS set state: " << state_map.get(switch_state) << " => " << state_map.get(state) << name();
#endif
		switch_state = state;
	}
	else if (isCurved(switch_state) != isCurved(state))
	{
		qWarning().noquote() << "Switch locked!" << name();
		success = false;
	}
	return success;
}

bool DoubleCrossSwitch::setState(
	const RailPart * prev,
	const RailPart * succ)
{
	const State state = computeState(prev, succ);

	return setState(state);
}

bool DoubleCrossSwitch::isSwitchable(
	const RailPart * prev,
	const RailPart * succ) const noexcept
{
	const State state = computeState(prev, succ);

	return (lock() == LockState::UNLOCKED) || (isCurved(state) == isCurved(switch_state));
}

State DoubleCrossSwitch::computeState(
	const RailPart * prev,
	const RailPart * succ) const
{
	unsigned state = 0;

	if ((prev == nullptr) || (succ == nullptr))
	{
		throw std::invalid_argument("Given rail parts are not defined.");
	}

	if ((b == prev) || (b == succ))
	{
		state |= B_MASK;
	}

	if ((d == prev) || (d == succ))
	{
		state |= D_MASK;
	}

#ifdef STATE_VERBOSE
	qDebug().noquote() << "########## DCS compute state: " << state_map.get(switch_state) << name();
#endif

	return static_cast<State>(state);
}

SwitchState DoubleCrossSwitch::switchState() const noexcept
{
	return isCurved() ?
		SwitchState::SWITCH_STATE_LEFT :
		SwitchState::SWITCH_STATE_RIGHT;
}

/*************************************************************************
**                                                                      **
**       Flank protection methods                                       **
**                                                                      **
*************************************************************************/

void DoubleCrossSwitch::collectFlankSwitches() noexcept
{
	flank_switches.push_back(follow(a,  a_in_dir, false));
	flank_switches.push_back(follow(b,  a_in_dir, true));
	flank_switches.push_back(follow(c, !a_in_dir, true));
	flank_switches.push_back(follow(d, !a_in_dir, false));
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state) const noexcept
{
	return flank(switches, set_state, state());
}

size_t DoubleCrossSwitch::flankCandidates(
	std::vector<RegularSwitch *> & switches,
	const RailPart        *        prev,
	const RailPart        *        succ) const noexcept
{
	State compare = computeState(prev, succ);

	return flank(switches, false, compare);
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state,
	const DoubleCrossSwitch::State compare) const noexcept
{
	const unsigned left  = ((unsigned)compare & B_MASK) ? 0 : 1;
	const unsigned right = ((unsigned)compare & D_MASK) ? 2 : 3;
	size_t         equal = 0;

	equal += flank(switches, set_state, left);
	equal += flank(switches, set_state, right);

	return equal;
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state,
	const unsigned                 index) const noexcept
{
	const std::bitset<8>  bits(index);
	const bool            left     = bits.count() == 1;
	const SwitchState     compare  = left ?  SwitchState::SWITCH_STATE_RIGHT : SwitchState::SWITCH_STATE_LEFT;
	RegularSwitch    *    other    = flank_switches[index];
	size_t                equal    = 0;

	if (other != nullptr)
	{
		switches.push_back(other);

		if (set_state)
		{
			other->setState(left ? RegularSwitch::State::AC : RegularSwitch::State::AB);
		}

		if (other->switchState() == compare)
		{
			equal++;
		}
	}
	return equal;
}

bool DoubleCrossSwitch::isCurved(const DoubleCrossSwitch::State state)
{
	const bool b_active = unsigned(state) & B_MASK;
	const bool d_active = unsigned(state) & D_MASK;

	return b_active == d_active;
}

/*************************************************************************
**                                                                      **
**       Support methods                                                **
**                                                                      **
*************************************************************************/

QString DoubleCrossSwitch::key() const noexcept
{
	return "DKW" + name();
}

QString DoubleCrossSwitch::get(const DoubleCrossSwitch::State & state) noexcept
{
	return state_map.get(state);
}

QString DoubleCrossSwitch::toString() const noexcept
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
