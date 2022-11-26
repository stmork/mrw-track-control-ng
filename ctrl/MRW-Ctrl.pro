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
	doublecrossswitchcontroller.cpp \
	doublecrossswitchcontrollermock.cpp \
	doublecrossswitchcontrollerproxy.cpp \
	railcontroller.cpp \
	railcontrollermock.cpp \
	railcontrollerproxy.cpp \
	signalcontroller.cpp \
	signalcontrollermock.cpp \
	signalcontrollerproxy.cpp

HEADERS +=  \
	basecontroller.h \
	regularswitchcontroller.h \
	regularswitchcontrollermock.h \
	regularswitchcontrollerproxy.h \
	doublecrossswitchcontroller.h \
	doublecrossswitchcontrollermock.h \
	doublecrossswitchcontrollerproxy.h \
	railcontroller.h \
	railcontrollermock.h \
	railcontrollerproxy.h \
	signalcontroller.h \
	signalcontrollermock.h \
	signalcontrollerproxy.h

QMAKE_CLEAN         += $$TARGET
