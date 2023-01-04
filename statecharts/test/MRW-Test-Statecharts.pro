#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

TEMPLATE = subdirs
SUBDIRS  = \
	OperationMode\
	Route \
	Section\
	Switch\
	Signal\
	SignalController\
	Tracker\
	Update

OperationMode.file    = OperatingMode/MRW-Test-OpMode.pro
Route.file            = Route/MRW-Test-Route.pro
Section.file          = Section/MRW-Test-Section.pro
Switch.file           = Switch/MRW-Test-Switch.pro
Signal.file           = Signal/MRW-Test-Signal.pro
SignalController.file = SignalController/MRW-Test-SignalController.pro
Tracker.file          = Tracker/MRW-Test-Tracker.pro
Update.file           = Update/MRW-Test-Update.pro

QMAKE_CLEAN          += gtest-*.xml
