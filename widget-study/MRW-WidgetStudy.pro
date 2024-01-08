#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

QT       += gui widgets

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	doublecrossswitchstudy.cpp \
	railstudy.cpp \
	signalstudy.cpp \
	switchstudy.cpp \
	widgetsaver.cpp \
	widgetstudy.cpp

HEADERS += \
	mainwindow.h \
	doublecrossswitchstudy.h \
	railstudy.h \
	signalstudy.h \
	switchstudy.h \
	widgetsaver.h \
	widgetstudy.h

FORMS += \
	doublecrossswitchstudy.ui \
	mainwindow.ui \
	railstudy.ui \
	signalstudy.ui \
	switchstudy.ui \
	widgetstudy.ui

LIBS            += -lMRW-UI -lMRW-Ctrl -lMRW-CtrlMock -lMRW-Model -lMRW-Can -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
