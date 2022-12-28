#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

HEADERS += \
	commands.h \
	mrwbusservice.h \
	mrwmessage.h \
	types.h

SOURCES += \
	mrwbusservice.cpp \
	mrwmessage.cpp

QMAKE_CLEAN  += $$TARGET
