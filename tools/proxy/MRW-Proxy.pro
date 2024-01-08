#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT -= gui

include(../../common.pri)

CONFIG += console

SOURCES += \
	main.cpp

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

install.files = $$TARGET

INSTALLS        += install
