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
	regularswitchcontrollermock.cpp \
	regularswitchcontrollerproxy.cpp \
	signalcontroller.cpp \
	signalcontrollermock.cpp \
	signalcontrollerproxy.cpp
HEADERS +=  \
	regularswitchcontroller.h \
	regularswitchcontrollermock.h \
	regularswitchcontrollerproxy.h \
	signalcontroller.h \
	signalcontrollermock.h \
	signalcontrollerproxy.h

QMAKE_CLEAN         += $$TARGET
