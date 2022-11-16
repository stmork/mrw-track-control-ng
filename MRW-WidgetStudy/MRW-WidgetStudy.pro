#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += gui widgets

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	regularswitchcontroller.cpp \
	regularswitchwidget.cpp \
	signalcontroller.cpp \
	signalwidget.cpp

HEADERS += \
	mainwindow.h \
	regularswitchcontroller.h \
	regularswitchwidget.h \
	signalcontroller.h \
	signalwidget.h

FORMS += \
	mainwindow.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
