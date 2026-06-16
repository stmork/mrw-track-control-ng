#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT -= gui
QT += network

include(../../common.pri)

CONFIG += console

SOURCES += \
	main.cpp \
	updateservice.cpp

HEADERS += \
	updateservice.h

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Util

QMAKE_CLEAN     += $$TARGET

install.files    = $$TARGET

INSTALLS        += install
