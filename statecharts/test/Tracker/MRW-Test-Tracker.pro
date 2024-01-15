#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += TrackerStatechart.h

SOURCES += \
	TrackerStatechart.cpp \
	TrackerTest.cpp

QMAKE_CLEAN     += $$TARGET
