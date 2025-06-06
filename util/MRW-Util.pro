#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	batch.cpp \
	batchparticipant.cpp \
	clockservice.cpp \
	dumphandler.cpp \
	duration.cpp \
	globalbatch.cpp \
	hexline.cpp \
	log.cpp \
	properties.cpp \
	settings.cpp \
	signalhandler.cpp \
	stringutil.cpp \
	termhandler.cpp

HEADERS += \
	batch.h \
	batchparticipant.h \
	cleanvector.h \
	clockservice.h \
	constantenumerator.h \
	dumphandler.h \
	duration.h \
	globalbatch.h \
	hexline.h \
	log.h \
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
