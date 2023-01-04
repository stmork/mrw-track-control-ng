#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	clockservice.cpp \
	dumphandler.cpp \
	method.cpp \
	properties.cpp \
	random.cpp \
	settings.cpp \
	stringutil.cpp \
	termhandler.cpp

HEADERS += \
	clockservice.h \
	constantenumerator.h \
	constantenumerator.h \
	dumphandler.h \
	method.h \
	properties.h \
	random.h \
	settings.h \
	singleton.h \
	stringutil.h \
	termhandler.h

QMAKE_CLEAN         += $$TARGET
