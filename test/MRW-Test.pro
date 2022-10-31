QT     += core xml testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testmodel.cpp \
	testswitch.cpp \
	testlight.cpp \
	testutil.cpp

HEADERS += \
	testmodel.h \
	testswitch.h \
	testlight.h \
	testutil.h

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
