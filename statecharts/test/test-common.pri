#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

CONFIG          += c++17
QMAKE_CXXFLAGS  += -Wextra -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow

INCLUDEPATH = $$PWD/common

HEADERS += \
	../common/sc_rxcpp.h \
	../common/sc_statemachine.h \
	../common/sc_timer.h \
	../common/sc_types.h

SOURCES += ../common/sc_runner_timed.cpp

LIBS += -lgtest -lgtest_main
