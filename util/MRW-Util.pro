QT       += core
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

SOURCES += \
	method.cpp \
	stringutil.cpp

HEADERS += \
	method.h \
	singleton.h \
	stringutil.h

QMAKE_CLEAN         += $$TARGET
