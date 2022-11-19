#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += widgets
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES +=  \
	basecontroller.cpp \
	regularswitchcontroller.cpp \
	regularswitchcontrollermock.cpp \
	regularswitchcontrollerproxy.cpp \
	signalcontroller.cpp \
	signalcontrollermock.cpp \
	signalcontrollerproxy.cpp
HEADERS +=  \
	basecontroller.h \
	regularswitchcontroller.h \
	regularswitchcontrollermock.h \
	regularswitchcontrollerproxy.h \
	signalcontroller.h \
	signalcontrollermock.h \
	signalcontrollerproxy.h

QMAKE_CLEAN         += $$TARGET
