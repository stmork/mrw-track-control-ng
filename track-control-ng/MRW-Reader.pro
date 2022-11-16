#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT += gui widgets

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	modelrepository.cpp \
	regionform.cpp

HEADERS += \
	mainwindow.h \
	modelrepository.h \
	regionform.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET
