QT       += core xml serialbus
TEMPLATE  = lib
CONFIG   += staticlib

include(../common.pri)

QMAKE_CLEAN         += $$TARGET

HEADERS += \
	commands.h \
	mrwmessage.h

SOURCES += \
	mrwmessage.cpp
