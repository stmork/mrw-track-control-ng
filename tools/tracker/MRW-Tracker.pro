#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT -= gui
QT += network

include(../../common.pri)

CONFIG  += console

SOURCES += \
	main.cpp \
	trackerservice.cpp

HEADERS += \
	trackerservice.h

LIBS    += -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Util

install.files = $$TARGET

INSTALLS        += install

QMAKE_CLEAN     += $$TARGET
