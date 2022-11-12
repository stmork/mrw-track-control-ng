#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT -= gui

include(../../common.pri)

CONFIG += console

SOURCES += \
	main.cpp

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can
