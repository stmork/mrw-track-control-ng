#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

QT     += testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testcan.cpp \
	testflankswitch.cpp \
	testmodel.cpp \
	testmodelbase.cpp \
	testnumbering.cpp \
	testrouting.cpp \
	testunknown.cpp \
	testswitch.cpp \
	testlight.cpp \
	testutil.cpp

HEADERS += \
	testcan.h \
	testdef.h \
	testflankswitch.h \
	testmodel.h \
	testmodelbase.h \
	testnumbering.h \
	testrouting.h \
	testunknown.h \
	testswitch.h \
	testlight.h \
	testutil.h

LIBS            += -lMRW-Model -lMRW-Util -lMRW-Can

QMAKE_CLEAN     += $$TARGET qtest*.xml
