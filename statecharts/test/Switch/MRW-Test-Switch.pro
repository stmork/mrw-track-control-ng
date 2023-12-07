#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += SwitchStatechart.h

SOURCES += \
	SwitchStatechart.cpp \
	SwitchTest.cpp

QMAKE_CLEAN     += $$TARGET
