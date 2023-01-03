#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include(../common.pri)

QMAKE_CXXFLAGS  -= -Wsuggest-override

HEADERS += \
	test/OperatingMode.h \
	test/RouteStatechart.h \
	test/SectionStatechart.h \
	test/SignalControllerStatechart.h \
	test/SignalStatechart.h \
	test/SwitchStatechart.h \
	test/TrackerStatechart.h \
	test/UpdateStatechart.h

SOURCES += \
	test/OperatingMode.cpp \
	test/OperatingModeTest.cpp \
	test/RouteStatechart.cpp \
	test/RouteTest.cpp \
	test/SectionStatechart.cpp \
	test/SectionTest.cpp \
	test/SignalControllerStatechart.cpp \
	test/SignalControllerTest.cpp \
	test/SignalStatechart.cpp \
	test/SignalTest.cpp \
	test/SwitchStatechart.cpp \
	test/SwitchTest.cpp \
	test/TrackerStatechart.cpp \
	test/TrackerTest.cpp \
	test/UpdateStatechart.cpp \
	test/UpdateTest.cpp

