#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	clockservice.cpp \
	method.cpp \
	properties.cpp \
	settings.cpp \
	stringutil.cpp \
	termhandler.cpp

HEADERS += \
	clockservice.h \
	constantenumerator.h \
	constantenumerator.h \
	method.h \
	properties.h \
	settings.h \
	singleton.h \
	stringutil.h \
	termhandler.h

QMAKE_CLEAN         += $$TARGET
