#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += gui widgets

include(../common.pri)

SOURCES += \
	doublecrossswitchstudy.cpp \
	main.cpp \
	mainwindow.cpp \
	../ctrl/basecontroller.cpp \
	../ctrl/regularswitchcontroller.cpp \
	../ctrl/regularswitchcontrollermock.cpp \
	../ctrl/signalcontroller.cpp \
	../ctrl/signalcontrollermock.cpp \
	../ui/basewidget.cpp \
	../ui/doublecrossswitchwidget.cpp \
	../ui/regularswitchwidget.cpp \
	../ui/sectionwidget.cpp \
	../ui/signalwidget.cpp \
	sectionstudy.cpp \
	signalstudy.cpp \
	switchstudy.cpp

HEADERS += \
	doublecrossswitchstudy.h \
	mainwindow.h \
	../ctrl/basecontroller.h \
	../ctrl/regularswitchcontroller.h \
	../ctrl/regularswitchcontrollermock.h \
	../ctrl/signalcontroller.h \
	../ctrl/signalcontrollermock.h \
	../ui/basewidget.h \
	../ui/doublecrossswitchwidget.h \
	../ui/regularswitchwidget.h \
	../ui/sectionwidget.h \
	../ui/signalwidget.h \
	sectionstudy.h \
	signalstudy.h \
	switchstudy.h

FORMS += \
	doublecrossswitchstudy.ui \
	mainwindow.ui \
	sectionstudy.ui \
	signalstudy.ui \
	switchstudy.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
