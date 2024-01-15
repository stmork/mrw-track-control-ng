#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += SignalStatechart.h

SOURCES += \
	SignalStatechart.cpp \
	SignalTest.cpp

QMAKE_CLEAN     += $$TARGET
