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

void DoubleCrossSwitch::setState(
	const RailPart * prev,
	const RailPart * succ)
{
	switch_state = computeState(prev, succ);
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

SwitchState DoubleCrossSwitch::switchState() const
{
	return isBranch() ?
		SwitchState::SWITCH_STATE_LEFT :
		SwitchState::SWITCH_STATE_RIGHT;
}

/*************************************************************************
**                                                                      **
**       Flank protection methods                                       **
**                                                                      **
*************************************************************************/

bool DoubleCrossSwitch::isFlankProtection(const AbstractSwitch * other) const
{
	const std::vector<RailPart *> candidates
	{
		a, b, c, d
	};

	for (unsigned i = 0; i < candidates.size(); i++)
	{
		const RegularSwitch * o_switch = follow(candidates[i]);
		const std::bitset<8>  bits(i);

		if ((o_switch == other) && isFlankCandidate(o_switch, bits.count() == 1))
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
	return flank(switches, set_state, state());
}

size_t DoubleCrossSwitch::flankCandidates(
	std::vector<RegularSwitch *> & switches,
	const RailPart        *        prev,
	const RailPart        *        succ) const
{
	State compare = computeState(prev, succ);

	return flank(switches, false, compare);
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state,
	const DoubleCrossSwitch::State compare) const
{
	size_t equal    = 0;

	if ((unsigned)compare & B_MASK)
	{
		equal += flank(switches, set_state, false, a);
	}
	else
	{
		equal += flank(switches, set_state, true, b);
	}

	if ((unsigned)compare & D_MASK)
	{
		equal += flank(switches, set_state, true, c);
	}
	else
	{
		equal += flank(switches, set_state, false, d);
	}
	return equal;
}

size_t DoubleCrossSwitch::flank(
	std::vector<RegularSwitch *> & switches,
	const bool                     set_state,
	const bool                     left,
	RailPart           *           other) const
{
	RegularSwitch  *  o_switch = follow(other);
	const SwitchState compare  = left ?  SWITCH_STATE_RIGHT : SWITCH_STATE_LEFT;
	size_t            equal    = 0;

	if (isFlankCandidate(o_switch, left))
	{
		switches.push_back(o_switch);

		if (set_state)
		{
			o_switch->setState(left ? RegularSwitch::State::AC : RegularSwitch::State::AB);
		}

		if (o_switch->switchState() == compare)
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

QString DoubleCrossSwitch::key() const
{
	return "DKW" + name();
}

QString DoubleCrossSwitch::get(const DoubleCrossSwitch::State & state)
{
	return state_map.get(state);
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

bool DoubleCrossSwitch::isBranch() const
{
	const bool b_active = unsigned(switch_state) & B_MASK;
	const bool d_active = unsigned(switch_state) & D_MASK;

	return b_active == d_active;
}
