#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT -= gui
QT += network

include(../../common.pri)

CONFIG += console

SOURCES += \
 main.cpp \
 simulatorservice.cpp

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

HEADERS += \
	simulatorservice.h
