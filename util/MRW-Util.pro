TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	method.cpp

HEADERS += \
	method.h \
	singleton.h \
	string.h

QMAKE_CLEAN         += $$TARGET
