#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

HEADERS += \
	OperatingMode.h \
	SectionStatechart.h \
	SwitchStatechart.h \
	common/sc_statemachine.h \
	common/sc_timer.h \
	common/sc_types.h \
	timerservice.h

SOURCES += \
	OperatingMode.cpp \
	SectionStatechart.cpp \
	SwitchStatechart.cpp \
	timerservice.cpp

QMAKE_CLEAN         += $$TARGET
