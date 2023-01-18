#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT += gui widgets network

include(../common.pri)

SOURCES += \
	ctrl/batch.cpp \
	ctrl/railpartinfo.cpp \
	ctrl/signalproxy.cpp \
	ctrl/switchcontroller.cpp \
	main.cpp \
	mainwindow.cpp \
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
	ctrl/batch.h \
	ctrl/railpartinfo.h \
	ctrl/signalproxy.h \
	ctrl/switchcontroller.h \
	mainwindow.h \
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

install.path  = $$(PREFIX)/usr/bin
install.files = $$TARGET

INSTALLS        += install

RESOURCES += \
	resources.qrc
