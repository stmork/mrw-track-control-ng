#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

HEADERS += \
	filelogger.h \
	loggerbase.h \
	loggerservice.h \
	stdlogger.h \
	syslogger.h

SOURCES += \
	filelogger.cpp \
	loggerbase.cpp \
	loggerservice.cpp \
	stdlogger.cpp \
	syslogger.cpp

QMAKE_CLEAN  += $$TARGET
