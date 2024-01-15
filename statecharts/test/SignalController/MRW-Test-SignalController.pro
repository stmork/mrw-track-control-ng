#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += SignalControllerStatechart.h

SOURCES += \
	SignalControllerStatechart.cpp \
	SignalControllerTest.cpp

QMAKE_CLEAN     += $$TARGET
