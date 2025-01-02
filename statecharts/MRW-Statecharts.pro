#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

QMAKE_CXXFLAGS  += -Wno-reorder -Wno-unused-parameter

HEADERS += \
	ConfigStatechart.h \
	OperatingModeStatechart.h \
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
	ConfigStatechart.cpp \
	OperatingModeStatechart.cpp \
	RouteStatechart.cpp \
	SectionStatechart.cpp \
	SignalControllerStatechart.cpp \
	SignalStatechart.cpp \
	SwitchStatechart.cpp \
	TrackerStatechart.cpp \
	UpdateStatechart.cpp \
	timerservice.cpp

QMAKE_CLEAN += $$TARGET
