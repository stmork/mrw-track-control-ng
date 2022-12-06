#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT += gui widgets network

include(../common.pri)

SOURCES += \
	ctrl/railpartinfo.cpp \
	main.cpp \
	mainwindow.cpp \
	modelrepository.cpp \
	mrwmessagedispatcher.cpp \
	regionform.cpp \
	ctrl/controllerregistrand.cpp \
	ctrl/controllerregistry.cpp \
	ctrl/regularswitchcontrollerproxy.cpp \
	ctrl/doublecrossswitchcontrollerproxy.cpp \
	ctrl/railcontrollerproxy.cpp \
	ctrl/signalcontrollerproxy.cpp \
	ctrl/sectioncontroller.cpp \
	widgetroute.cpp

HEADERS += \
	ctrl/railpartinfo.h \
	mainwindow.h \
	modelrepository.h \
	mrwmessagedispatcher.h \
	regionform.h \
	ctrl/controllerregistrand.h \
	ctrl/controllerregistry.h \
	ctrl/regularswitchcontrollerproxy.h \
	ctrl/doublecrossswitchcontrollerproxy.h \
	ctrl/railcontrollerproxy.h \
	ctrl/signalcontrollerproxy.h \
	ctrl/sectioncontroller.h \
	widgetroute.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-UI -lMRW-Ctrl -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Util

QMAKE_CLEAN     += $$TARGET
