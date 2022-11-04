QT       += xml
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	area.cpp \
	assemblypart.cpp \
	controller.cpp \
	device.cpp \
	formsignal.cpp \
	light.cpp \
	lightmodule.cpp \
	lightsignal.cpp \
	modelrailway.cpp \
	module.cpp \
	multiplexconnection.cpp \
	profilelight.cpp \
	rail.cpp \
	raildoubleswitch.cpp \
	railpart.cpp \
	railswitch.cpp \
	section.cpp \
	sectionmodule.cpp \
	signal.cpp \
	switchmodule.cpp \
	switchmodulereference.cpp

HEADERS += \
	area.h \
	assemblypart.h \
	commands.h \
	controller.h \
	device.h \
	formsignal.h \
	light.h \
	lightmodule.h \
	lightsignal.h \
	modelrailway.h \
	module.h \
	multiplexconnection.h \
	profilelight.h \
	rail.h \
	raildoubleswitch.h \
	railpart.h \
	railswitch.h \
	section.h \
	sectionmodule.h \
	signal.h \
	switchmodule.h \
	switchmodulereference.h

QMAKE_CLEAN         += $$TARGET