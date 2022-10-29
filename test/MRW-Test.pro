QT     += core xml testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testmodel.cpp \
	testutil.cpp

HEADERS += \
	testmodel.h \
	testutil.h

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
