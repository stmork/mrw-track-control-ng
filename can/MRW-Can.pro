#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	cansettings.cpp \
	mrwbusservice.cpp \
	mrwmessage.cpp

HEADERS += \
	cansettings.h \
	commands.h \
	mrwbusservice.h \
	mrwmessage.h \
	types.h

QMAKE_CLEAN  += $$TARGET
