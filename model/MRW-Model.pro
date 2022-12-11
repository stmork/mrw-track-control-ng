#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT       += network
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	abstractswitch.cpp \
	assemblypart.cpp \
	controller.cpp \
	device.cpp \
	doublecrossswitch.cpp \
	formsignal.cpp \
	light.cpp \
	lightmodule.cpp \
	lightsignal.cpp \
	modelrailway.cpp \
	modelrepository.cpp \
	module.cpp \
	multiplexconnection.cpp \
	position.cpp \
	profilelight.cpp \
	rail.cpp \
	railpart.cpp \
	region.cpp \
	regularswitch.cpp \
	route.cpp \
	section.cpp \
	sectionmodule.cpp \
	signal.cpp \
	switchmodule.cpp \
	switchmodulereference.cpp

HEADERS += \
	abstractswitch.h \
	assemblypart.h \
	controller.h \
	device.h \
	doublecrossswitch.h \
	formsignal.h \
	light.h \
	lightmodule.h \
	lightsignal.h \
	modelrailway.h \
	modelrepository.h \
	module.h \
	multiplexconnection.h \
	position.h \
	profilelight.h \
	rail.h \
	railpart.h \
	region.h \
	regularswitch.h \
	route.h \
	section.h \
	sectionmodule.h \
	signal.h \
	switchmodule.h \
	switchmodulereference.h

QMAKE_CLEAN         += $$TARGET
