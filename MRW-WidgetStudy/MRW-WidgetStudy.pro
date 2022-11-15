#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += gui widgets

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	regularswitchwidget.cpp \
	signalwidget.cpp

HEADERS += \
	mainwindow.h \
	regularswitchwidget.h \
	signalwidget.h

FORMS += \
	mainwindow.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
