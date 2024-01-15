#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += UpdateStatechart.h

SOURCES += \
	UpdateStatechart.cpp \
	UpdateTest.cpp

QMAKE_CLEAN     += $$TARGET
