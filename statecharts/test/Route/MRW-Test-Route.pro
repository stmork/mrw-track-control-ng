#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../test-common.pri)

HEADERS += RouteStatechart.h

SOURCES += \
	RouteStatechart.cpp \
	RouteTest.cpp

QMAKE_CLEAN     += $$TARGET
