#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += gui widgets

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	doublecrossswitchstudy.cpp \
	railstudy.cpp \
	signalstudy.cpp \
	switchstudy.cpp \
	../ctrl/basecontroller.cpp \
	../ctrl/regularswitchcontroller.cpp \
	../ctrl/regularswitchcontrollermock.cpp \
	../ctrl/doublecrossswitchcontroller.cpp \
	../ctrl/doublecrossswitchcontrollermock.cpp \
	../ctrl/railcontroller.cpp \
	../ctrl/railcontrollermock.cpp \
	../ctrl/signalcontroller.cpp \
	../ctrl/signalcontrollermock.cpp \
	../ui/basewidget.cpp \
	../ui/controllerwidget.cpp \
	../ui/regularswitchwidget.cpp \
	../ui/doublecrossswitchwidget.cpp \
	../ui/railwidget.cpp \
	../ui/signalwidget.cpp

HEADERS += \
	mainwindow.h \
	doublecrossswitchstudy.h \
	railstudy.h \
	signalstudy.h \
	switchstudy.h \
	../ctrl/basecontroller.h \
	../ctrl/regularswitchcontroller.h \
	../ctrl/regularswitchcontrollermock.h \
	../ctrl/doublecrossswitchcontroller.h \
	../ctrl/doublecrossswitchcontrollermock.h \
	../ctrl/railcontroller.h \
	../ctrl/railcontrollermock.h \
	../ctrl/signalcontroller.h \
	../ctrl/signalcontrollermock.h \
	../ui/basewidget.h \
	../ui/controllerwidget.h \
	../ui/regularswitchwidget.h \
	../ui/doublecrossswitchwidget.h \
	../ui/railwidget.h \
	../ui/signalwidget.h

FORMS += \
	doublecrossswitchstudy.ui \
	mainwindow.ui \
	railstudy.ui \
	signalstudy.ui \
	switchstudy.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
