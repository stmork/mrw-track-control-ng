#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += ConfigStatechart.h

SOURCES += \
	ConfigStatechart.cpp \
	ConfigTest.cpp

QMAKE_CLEAN     += $$TARGET
