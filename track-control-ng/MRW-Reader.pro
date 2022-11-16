#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT += gui widgets

include(../common.pri)

SOURCES += \
	barwidget.cpp \
	circlelivewidget.cpp \
	clockwidget.cpp \
	main.cpp \
	mainwindow.cpp \
	modelrepository.cpp \
	regionform.cpp \
	stationwidget.cpp

HEADERS += \
	barwidget.h \
	circlelivewidget.h \
	clockwidget.h \
	mainwindow.h \
	modelrepository.h \
	regionform.h \
	stationwidget.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET
