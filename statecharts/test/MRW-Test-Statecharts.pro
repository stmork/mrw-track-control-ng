#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

#TEMPLATE = subdirs
#SUBDIRS  = \
#	OperationMode \
#	Route \
#	Section \
#	Switch \
#	Signal \
#	SignalController \
#	Tracker \
#	Config \
#	Update

#OperationMode.file    = OperatingMode/MRW-Test-OpMode.pro
#Route.file            = Route/MRW-Test-Route.pro
#Section.file          = Section/MRW-Test-Section.pro
#Switch.file           = Switch/MRW-Test-Switch.pro
#Signal.file           = Signal/MRW-Test-Signal.pro
#SignalController.file = SignalController/MRW-Test-SignalController.pro
#Tracker.file          = Tracker/MRW-Test-Tracker.pro
#Config.file           = Config/MRW-Test-Config.pro
#Update.file           = Update/MRW-Test-Update.pro

# include(test-common.pri)

QT              -= gui core

CONFIG          += c++17
CONFIG          += link_pkgconfig

PKGCONFIG       += gtest
PKGCONFIG       += gtest_main

INCLUDEPATH = $$PWD/common

QMAKE_CLEAN          += gtest-*.xml

HEADERS += \
	Config/ConfigStatechart.h \
	OperatingMode/OperatingModeStatechart.h \
	Route/RouteStatechart.h \
	Section/SectionStatechart.h \
	Signal/SignalStatechart.h \
	Switch/SwitchStatechart.h \
	Tracker/TrackerStatechart.h \
	Update/UpdateStatechart.h \
	common/sc_cyclebased.h \
	common/sc_eventdriven.h \
	common/sc_runner_timed.h \
	common/sc_statemachine.h \
	common/sc_timer.h \
	common/sc_timer_service.h \
	common/sc_types.h

SOURCES += \
	Config/ConfigStatechart.cpp \
	Config/ConfigTest.cpp \
	OperatingMode/OperatingModeStatechart.cpp \
	OperatingMode/OperatingModeTest.cpp \
	Route/RouteStatechart.cpp \
	Route/RouteTest.cpp \
	Section/SectionStatechart.cpp \
	Section/SectionTest.cpp \
	Signal/SignalStatechart.cpp \
	Signal/SignalTest.cpp \
	Switch/SwitchStatechart.cpp \
	Switch/SwitchTest.cpp \
	Tracker/TrackerStatechart.cpp \
	Tracker/TrackerTest.cpp \
	Update/UpdateStatechart.cpp \
	Update/UpdateTest.cpp \
	common/sc_runner_timed.cpp \
	common/sc_timer_service.cpp
