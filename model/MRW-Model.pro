QT       += xml
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	area.cpp \
	controller.cpp \
	formsignal.cpp \
	light.cpp \
	lightmodule.cpp \
	lightsignal.cpp \
	module.cpp \
	mrwmodel.cpp \
	multiplexconnection.cpp \
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
	controller.h \
	formsignal.h \
	light.h \
	lightmodule.h \
	lightsignal.h \
	module.h \
	mrwmodel.h \
	multiplexconnection.h \
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
