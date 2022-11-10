QT     += core xml testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testmodel.cpp \
	testmodelbase.cpp \
	testnumbering.cpp \
	testunknown.cpp \
	testswitch.cpp \
	testlight.cpp \
	testutil.cpp

HEADERS += \
	testdef.h \
	testmodel.h \
	testmodelbase.h \
	testnumbering.h \
	testunknown.h \
	testswitch.h \
	testlight.h \
	testutil.h

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
