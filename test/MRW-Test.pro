#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

QT     += testlib widgets network
CONFIG += testcase no_testcase_installs

include(../common.pri)

SOURCES += \
	collections.cpp \
	main.cpp \
	testcan.cpp \
	testcanservice.cpp \
	testdoublecrossswitchwidget.cpp \
	testflankswitch.cpp \
	testmodel.cpp \
	testmodelbase.cpp \
	testnumbering.cpp \
	testrailwidget.cpp \
	testregularswitchwidget.cpp \
	testrouting.cpp \
	testsignalwidget.cpp \
	testunknown.cpp \
	testswitch.cpp \
	testlight.cpp \
	testutil.cpp

HEADERS += \
	collections.h \
	testcan.h \
	testcanservice.h \
	testdef.h \
	testdoublecrossswitchwidget.h \
	testflankswitch.h \
	testmodel.h \
	testmodelbase.h \
	testnumbering.h \
	testrailwidget.h \
	testregularswitchwidget.h \
	testroute.h \
	testrouting.h \
	testsignalwidget.h \
	testunknown.h \
	testswitch.h \
	testlight.h \
	testutil.h

LIBS            += -lMRW-UI -lMRW-Ctrl -lMRW-CtrlMock -lMRW-Model -lMRW-Can -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
