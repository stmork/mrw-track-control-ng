#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	batch.cpp \
	batchparticipant.cpp \
	globalbatch.cpp \
	clockservice.cpp \
	dumphandler.cpp \
	method.cpp \
	properties.cpp \
	random.cpp \
	settings.cpp \
	signalhandler.cpp \
	stringutil.cpp \
	termhandler.cpp

HEADERS += \
	batch.h \
	batchparticipant.h \
	globalbatch.h \
	clockservice.h \
	constantenumerator.h \
	constantenumerator.h \
	dumphandler.h \
	method.h \
	properties.h \
	random.h \
	self.h \
	settings.h \
	signalhandler.h \
	singleton.h \
	stringutil.h \
	termhandler.h

QMAKE_CLEAN         += $$TARGET
