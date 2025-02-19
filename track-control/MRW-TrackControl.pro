#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

QT += gui widgets network

include(../common.pri)

SOURCES += \
	controlledroute.cpp \
	ctrl/controllerregistrand.cpp \
	ctrl/controllerregistry.cpp \
	ctrl/doublecrossswitchcontrollerproxy.cpp \
	ctrl/railpartinfo.cpp \
	ctrl/railcontrollerproxy.cpp \
	ctrl/regularswitchcontrollerproxy.cpp \
	ctrl/sectioncontroller.cpp \
	ctrl/signalcontrollerproxy.cpp \
	ctrl/signalproxy.cpp \
	ctrl/switchcontroller.cpp \
	main.cpp \
	mainwindow.cpp \
	mrwmessagedispatcher.cpp \
	beermodeservice.cpp \
	regionform.cpp \
	routebatch.cpp \
	screenblankhandler.cpp \
	ui/routelistwidget.cpp \
	ui/sectionlistwidget.cpp

HEADERS += \
	beermodeservice.h \
	controlledroute.h \
	ctrl/controllerregistrand.h \
	ctrl/controllerregistry.h \
	ctrl/doublecrossswitchcontrollerproxy.h \
	ctrl/railcontrollerproxy.h \
	ctrl/railpartinfo.h \
	ctrl/regularswitchcontrollerproxy.h \
	ctrl/sectioncontroller.h \
	ctrl/signalcontrollerproxy.h \
	ctrl/signalproxy.h \
	ctrl/switchcontroller.h \
	mainwindow.h \
	mrwmessagedispatcher.h \
	regionform.h \
	routebatch.h \
	screenblankhandler.h \
	ui/routelistwidget.h \
	ui/sectionlistwidget.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lsystemd -lMRW-UI -lMRW-Ctrl -lMRW-Model -lMRW-Can -lMRW-Statecharts -lMRW-Log -lMRW-Util

linux {
	DEFINES     += X11_SCREEN_SAVER
	LIBS        += -lX11 -lXss
}

QMAKE_CLEAN     += $$TARGET

install.path  = $$(PREFIX)/usr/bin
install.files = $$TARGET

INSTALLS        += install

RESOURCES += \
	resources.qrc

message("Version: *** $$VERSION ***")
!isEmpty(BUILD_NUMBER) {
	message("Build number: $$BUILD_NUMBER")
}
