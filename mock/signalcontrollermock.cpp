//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <mock/signalcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

SignalControllerMock::SignalControllerMock(QObject * parent) :
	SignalController(parent)
{
}

void SignalControllerMock::reset()
{
	setDirection();
	setExtension(0);
	setSectionState(SectionState::FREE);
	setLock(LockState::UNLOCKED);
	setBending(Bending::STRAIGHT);

	setShunting(false);
	setDistant(false);
	setMain(true);

	setShuntStop();
	setDistantStop();
	setMainStop();
}

void SignalControllerMock::setShunting(const bool shunt)
{
	shunting = shunt;
	emit update();
}

void SignalControllerMock::setDistant(const bool distant)
{
	distant_signal = distant;
	emit update();
}

void SignalControllerMock::setMain(const bool main)
{
	main_signal = main;
	emit update();
}

void SignalControllerMock::setDirection(const bool dir)
{
	direction = dir;
	emit computeConnectors();
	emit update();
}

void SignalControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

void SignalControllerMock::setLock(const Device::LockState lock)
{
	lock_state = lock;
	emit update();
}

void SignalControllerMock::setBending(const Bending bending)
{
	bending_state = bending;
	emit computeConnectors();
	emit update();
}

void SignalControllerMock::setMainSymbol(const Symbol symbol)
{
	main_state = symbol;
	emit update();
}

void SignalControllerMock::setDistantSymbol(const Symbol symbol)
{
	distant_state = symbol;
	emit update();
}

void SignalControllerMock::setShuntSymbol(const Symbol symbol)
{
	shunt_state = symbol;
	emit update();
}

void SignalControllerMock::setExtension(const int new_extension)
{
	extension = new_extension;
	emit extend();
	emit computeConnectors();
	emit update();
}

void SignalControllerMock::setShuntStop()
{
	setShuntSymbol(Symbol::STOP);
}

void SignalControllerMock::setShuntGo()
{
	setShuntSymbol(Symbol::GO);
}

void SignalControllerMock::setDistantStop()
{
	setDistantSymbol(Symbol::STOP);
}

void SignalControllerMock::setDistantGo()
{
	setDistantSymbol(Symbol::GO);
}

void SignalControllerMock::setMainStop()
{
	setMainSymbol(Symbol::STOP);
}

void SignalControllerMock::setMainGo()
{
	setMainSymbol(Symbol::GO);
}

bool SignalControllerMock::isDirection() const
{
	return direction;
}

bool SignalControllerMock::hasShunting() const
{
	return shunting;
}

bool SignalControllerMock::hasDistant() const
{
	return distant_signal;
}

bool SignalControllerMock::hasMain() const
{
	return main_signal;
}

Symbol SignalControllerMock::distant() const
{
	return distant_state;
}

Symbol SignalControllerMock::shunt() const
{
	return shunt_state;
}

Symbol SignalControllerMock::main() const
{
	return main_state;
}

SectionState SignalControllerMock::state() const
{
	return section_state;
}

Device::LockState SignalControllerMock::lock() const
{
	return lock_state;
}

Position::Bending SignalControllerMock::bending() const
{
	return bending_state;
}

float SignalControllerMock::extensions() const
{
	return extension;
}

const QString & SignalControllerMock::name() const
{
	QStringList result;

	if (hasMain())
	{
		result += isDirection() ? "N3" : "P3";
	}

	if (hasDistant())
	{
		result += isDirection() ? "n203" : "p203";
	}

	if (result.isEmpty())
	{
		result += "i3";
	}

	signal_name = result.join('/');
	return signal_name;
}
