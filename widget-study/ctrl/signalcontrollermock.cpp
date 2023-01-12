//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/signalcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending = Position::Bending;

SignalControllerMock::SignalControllerMock(QObject * parent) :
	SignalController(parent)
{
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

void SignalControllerMock::setExtension(const int new_extension)
{
	extension = new_extension;
	emit extend();
	emit computeConnectors();
	emit update();
}

void SignalControllerMock::setShuntStop()
{
	shunt_state = Signal::Symbol::STOP;
	emit update();
}

void SignalControllerMock::setShuntGo()
{
	shunt_state = Signal::Symbol::GO;
	emit update();
}

void SignalControllerMock::setDistantStop()
{
	distant_state = Signal::Symbol::STOP;
	emit update();
}

void SignalControllerMock::setDistantGo()
{
	distant_state = Signal::Symbol::GO;
	emit update();
}

void SignalControllerMock::setMainStop()
{
	main_state = Signal::Symbol::STOP;
	emit update();
}

void SignalControllerMock::setMainGo()
{
	main_state = Signal::Symbol::GO;
	emit update();
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

QString SignalControllerMock::name() const
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
	return result.join('/');
}
