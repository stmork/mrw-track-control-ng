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
	signalcontroller.cpp \
	../ui/regularswitchwidget.cpp \
	../ui/signalwidget.cpp

HEADERS += \
	mainwindow.h \
	regularswitchcontroller.h \
	signalcontroller.h \
	../ui/regularswitchwidget.h \
	../ui/signalwidget.h

FORMS += \
	mainwindow.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
