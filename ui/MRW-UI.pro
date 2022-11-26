#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += widgets
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	barwidget.cpp \
	basewidget.cpp \
	circlelivewidget.cpp \
	clockwidget.cpp \
	controllerwidget.cpp \
	doublecrossswitchwidget.cpp \
	regularswitchwidget.cpp \
	railwidget.cpp \
	signalwidget.cpp \
	stationwidget.cpp

HEADERS += \
	barwidget.h \
	basewidget.h \
	circlelivewidget.h \
	clockwidget.h \
	controllerwidget.h \
	doublecrossswitchwidget.h \
	regularswitchwidget.h \
	railwidget.h \
	signalwidget.h \
	stationwidget.h

QMAKE_CLEAN         += $$TARGET
