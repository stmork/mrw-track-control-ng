#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += OperatingModeStatechart.h

SOURCES += \
	OperatingModeStatechart.cpp \
	OperatingModeTest.cpp

QMAKE_CLEAN     += $$TARGET
