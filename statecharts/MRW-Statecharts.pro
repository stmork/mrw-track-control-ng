#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

HEADERS += \
	OperatingMode.h \
	RouteStatechart.h \
	SectionStatechart.h \
	SignalControllerStatechart.h \
	SignalStatechart.h \
	SwitchStatechart.h \
	TrackerStatechart.h \
	UpdateStatechart.h \
	common/sc_statemachine.h \
	common/sc_timer.h \
	common/sc_types.h \
	timerservice.h

SOURCES += \
	OperatingMode.cpp \
	RouteStatechart.cpp \
	SectionStatechart.cpp \
	SignalControllerStatechart.cpp \
	SignalStatechart.cpp \
	SwitchStatechart.cpp \
	TrackerStatechart.cpp \
	UpdateStatechart.cpp \
	timerservice.cpp

QMAKE_CLEAN         += $$TARGET
