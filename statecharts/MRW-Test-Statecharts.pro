#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

include (../common.pri)

HEADERS += \
	test/OperatingModeImpl.h \
	test/RouteStatechartImpl.h \
	test/SectionStatechartImpl.h \
	test/SignalControllerStatechartImpl.h \
	test/SignalStatechartImpl.h \
	test/SwitchStatechartImpl.h \
	test/TrackerStatechartImpl.h \
	test/UpdateStatechartImpl.h \
	test/sc_rxcpp.h \
	test/sc_statemachine.h \
	test/sc_timer.h \
	test/sc_timer_service.h \
	test/sc_types.h

SOURCES += \
	test/OperatingModeImpl.cpp \
	test/RouteStatechartImpl.cpp \
	test/SectionStatechartImpl.cpp \
	test/SignalControllerStatechartImpl.cpp \
	test/SignalStatechartImpl.cpp \
	test/SwitchStatechartImpl.cpp \
	test/TrackerStatechartImpl.cpp \
	test/UpdateStatechartImpl.cpp \
	test/sc_timer_service.cpp

LIBS += -lgtest -lgtest_main
