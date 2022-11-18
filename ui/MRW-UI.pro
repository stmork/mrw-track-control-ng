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
	doublecrossswitchwidget.cpp \
	regularswitchwidget.cpp \
	sectionwidget.cpp \
	signalwidget.cpp \
	stationwidget.cpp
HEADERS += \
	barwidget.h \
	basewidget.h \
	circlelivewidget.h \
	clockwidget.h \
	doublecrossswitchwidget.h \
	regularswitchwidget.h \
	sectionwidget.h \
	signalwidget.h \
	stationwidget.h

QMAKE_CLEAN         += $$TARGET
