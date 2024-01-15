#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += SectionStatechart.h

SOURCES += \
	SectionStatechart.cpp \
	SectionTest.cpp

QMAKE_CLEAN     += $$TARGET
