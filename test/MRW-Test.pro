#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

QT     += testlib widgets
CONFIG += testcase no_testcase_installs

include(../common.pri)

SOURCES += \
	main.cpp \
	testcan.cpp \
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
	testcan.h \
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
