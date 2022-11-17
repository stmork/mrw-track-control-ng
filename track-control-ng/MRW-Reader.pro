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
	regionform.cpp \
	../ctrl/signalcontroller.cpp \
	../ctrl/signalcontrollerproxy.cpp \
	../ui/basewidget.cpp \
	../ui/barwidget.cpp \
	../ui/circlelivewidget.cpp \
	../ui/clockwidget.cpp \
	../ui/regularswitchwidget.cpp \
	../ui/stationwidget.cpp \
	../ui/signalwidget.cpp

HEADERS += \
	mainwindow.h \
	modelrepository.h \
	regionform.h \
	../ctrl/signalcontroller.h \
	../ctrl/signalcontrollerproxy.h \
	../ui/basewidget.h \
	../ui/barwidget.h \
	../ui/circlelivewidget.h \
	../ui/clockwidget.h \
	../ui/regularswitchwidget.h \
	../ui/stationwidget.h \
	../ui/signalwidget.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET
