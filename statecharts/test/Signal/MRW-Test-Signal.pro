#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += SignalStatechart.h

SOURCES += \
	SignalStatechart.cpp \
	SignalTest.cpp

QMAKE_CLEAN     += $$TARGET
