#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT       += widgets
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES +=  \
	basecontroller.cpp \
	baseswitchcontroller.cpp \
	regularswitchcontroller.cpp \
	doublecrossswitchcontroller.cpp \
	railcontroller.cpp \
	signalcontroller.cpp

HEADERS +=  \
	basecontroller.h \
	baseswitchcontroller.h \
	regularswitchcontroller.h \
	doublecrossswitchcontroller.h \
	railcontroller.h \
	signalcontroller.h

QMAKE_CLEAN         += $$TARGET
