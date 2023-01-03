#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += OperatingMode.h

SOURCES += \
	OperatingMode.cpp \
	OperatingModeTest.cpp

QMAKE_CLEAN     += $$TARGET
