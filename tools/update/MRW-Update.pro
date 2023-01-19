#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT -= gui
QT += network

include(../../common.pri)

CONFIG += console

SOURCES += \
	hexline.cpp \
	main.cpp \
	updateservice.cpp

HEADERS += \
	hexline.h \
	updateservice.h

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Util

QMAKE_CLEAN     += $$TARGET

install.files    = $$TARGET

INSTALLS        += install
