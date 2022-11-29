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
	mrwmessagedispatcher.cpp \
	regionform.cpp \
	../ctrl/basecontroller.cpp \
	../ctrl/controllerregistrand.cpp \
	../ctrl/controllerregistry.cpp \
	../ctrl/regularswitchcontroller.cpp \
	../ctrl/regularswitchcontrollerproxy.cpp \
	../ctrl/doublecrossswitchcontroller.cpp \
	../ctrl/doublecrossswitchcontrollerproxy.cpp \
	../ctrl/railcontroller.cpp \
	../ctrl/railcontrollerproxy.cpp \
	../ctrl/signalcontroller.cpp \
	../ctrl/signalcontrollerproxy.cpp \
	../ctrl/sectioncontroller.cpp \
	../ui/basewidget.cpp \
	../ui/controllerwidget.cpp \
	../ui/barwidget.cpp \
	../ui/circlelivewidget.cpp \
	../ui/clockwidget.cpp \
	../ui/stationwidget.cpp \
	../ui/railwidget.cpp \
	../ui/regularswitchwidget.cpp \
	../ui/doublecrossswitchwidget.cpp \
	../ui/signalwidget.cpp

HEADERS += \
	mainwindow.h \
	modelrepository.h \
	mrwmessagedispatcher.h \
	regionform.h \
	../ctrl/basecontroller.h \
	../ctrl/controllerregistrand.h \
	../ctrl/controllerregistry.h \
	../ctrl/regularswitchcontroller.h \
	../ctrl/regularswitchcontrollerproxy.h \
	../ctrl/doublecrossswitchcontroller.h \
	../ctrl/doublecrossswitchcontrollerproxy.h \
	../ctrl/railcontroller.h \
	../ctrl/railcontrollerproxy.h \
	../ctrl/signalcontroller.h \
	../ctrl/signalcontrollerproxy.h \
	../ctrl/sectioncontroller.h \
	../ui/basewidget.h \
	../ui/controllerwidget.h \
	../ui/barwidget.h \
	../ui/circlelivewidget.h \
	../ui/clockwidget.h \
	../ui/stationwidget.h \
	../ui/railwidget.h \
	../ui/regularswitchwidget.h \
	../ui/doublecrossswitchwidget.h \
	../ui/signalwidget.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET
