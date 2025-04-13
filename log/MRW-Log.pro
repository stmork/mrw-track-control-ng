#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	filelogger.cpp \
	loggerbase.cpp \
	loggerservice.cpp \
	stdlogger.cpp \
	syslogger.cpp

HEADERS += \
	filelogger.h \
	loggerbase.h \
	loggerservice.h \
	stdlogger.h \
	syslogger.h

QMAKE_CLEAN  += $$TARGET
