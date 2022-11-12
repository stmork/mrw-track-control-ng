#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	method.cpp \
	settings.cpp \
	stringutil.cpp

HEADERS += \
	constantenumerator.h \
	constantenumerator.h \
	method.h \
	settings.h \
	singleton.h \
	stringutil.h

QMAKE_CLEAN         += $$TARGET
