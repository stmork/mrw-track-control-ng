#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT              -= gui core

CONFIG          += c++17
CONFIG          += link_pkgconfig

PKGCONFIG       += gtest
PKGCONFIG       += gtest_main

QMAKE_CXXFLAGS  += -Wextra -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow
QMAKE_CXXFLAGS  -= -Werror -Wunused-parameter -Wreorder

INCLUDEPATH = $$PWD/common

HEADERS += \
	../common/sc_runner_timed.h \
	../common/sc_statemachine.h \
	../common/sc_timer.h \
	../common/sc_timer_service.h \
	../common/sc_types.h

SOURCES += \
	../common/sc_runner_timed.cpp \
	../common/sc_timer_service.cpp
