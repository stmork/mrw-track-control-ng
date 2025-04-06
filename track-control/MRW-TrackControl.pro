#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

QT += gui widgets network

linux {
	DEFINES     += X11_SCREEN_SAVER
	LIBS        += -lX11 -lXext
	message("Using X11 screen saver.")
}

linux:CONFIG(drm-ss) {
	PKGCONFIG   += libdrm
	DEFINES     += DRM_SCREEN_SAVER
	message("Using DRM screen saver.")
}

linux:CONFIG(qt-ss) {
	QT          += gui-private
	DEFINES     += QT_SCREEN_SAVER
	message("Using Qt based screen saver.")
}

CONFIG += link_pkgconfig

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
	log.cpp \
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
	log.h \
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
