#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT       += widgets
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES +=  \
	regularswitchcontrollermock.cpp \
	doublecrossswitchcontrollermock.cpp \
	railcontrollermock.cpp \
	signalcontrollermock.cpp

HEADERS +=  \
	regularswitchcontrollermock.h \
	doublecrossswitchcontrollermock.h \
	railcontrollermock.h \
	signalcontrollermock.h

QMAKE_CLEAN         += $$TARGET
