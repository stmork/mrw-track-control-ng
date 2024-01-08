#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT += network
QT -= gui

include(../../common.pri)

CONFIG += console

SOURCES += \
	main.cpp \
	resetservice.cpp

HEADERS += \
	resetservice.h

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

install.files = $$TARGET

INSTALLS        += install
