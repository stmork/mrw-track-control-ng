#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT += network
QT -= gui

include(../../common.pri)

CONFIG += console

SOURCES += \
	main.cpp

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

install.files = $$TARGET

INSTALLS        += install
