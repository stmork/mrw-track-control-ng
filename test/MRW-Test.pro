#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT     += testlib
CONFIG += testcase no_testcase_installs

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

LIBS            += -lMRW-Model -lMRW-Can -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
