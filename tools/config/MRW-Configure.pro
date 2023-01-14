#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT -= gui
QT += network

include(../../common.pri)

CONFIG += console

SOURCES += \
	configurationservice.cpp \
	main.cpp

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

QMAKE_CLEAN     += $$TARGET

install.files    = $$TARGET

INSTALLS        += install

HEADERS += \
	configurationservice.h
