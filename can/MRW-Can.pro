#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

QMAKE_CLEAN         += $$TARGET

HEADERS += \
	commands.h \
	mrwbusservice.h \
	mrwmessage.h

SOURCES += \
	mrwbusservice.cpp \
	mrwmessage.cpp
