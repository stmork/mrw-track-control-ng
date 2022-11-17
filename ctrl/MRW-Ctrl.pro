#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += widgets
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES +=  \
	regularswitchcontroller.cpp \
	signalcontroller.cpp \
	signalcontrollermock.cpp \
	signalcontrollerproxy.cpp
HEADERS +=  \
	regularswitchcontroller.h \
	signalcontroller.h \
	signalcontrollermock.h \
	signalcontrollerproxy.h

QMAKE_CLEAN         += $$TARGET
