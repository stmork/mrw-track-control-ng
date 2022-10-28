QT += core testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testutil.cpp

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET

HEADERS += \
	testutil.h
