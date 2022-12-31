#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT -= gui

include(../common.pri)

CONFIG += console

SOURCES += \
	main.cpp \
	updateservice.cpp

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Util

install.files = $$TARGET

INSTALLS        += install

HEADERS += \
	updateservice.h
