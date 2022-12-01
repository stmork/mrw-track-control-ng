#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT += gui widgets network

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
	../ctrl/sectioncontroller.cpp

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
	../ctrl/sectioncontroller.h

FORMS += \
	mainwindow.ui \
	regionform.ui

LIBS            += -lMRW-UI -lMRW-Model -lMRW-Can -l MRW-Statecharts -lMRW-Util

QMAKE_CLEAN     += $$TARGET
