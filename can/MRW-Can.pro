#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

QT       += network
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

HEADERS += \
	cansettings.h \
	commands.h \
	mrwbusservice.h \
	mrwmessage.h \
	types.h

SOURCES += \
	cansettings.cpp \
	mrwbusservice.cpp \
	mrwmessage.cpp

QMAKE_CLEAN  += $$TARGET
