#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	ctrl/regularswitchcontrollermock.cpp \
	ctrl/doublecrossswitchcontrollermock.cpp \
	ctrl/railcontrollermock.cpp \
	ctrl/signalcontrollermock.cpp \
	widgetsaver.cpp

HEADERS += \
	mainwindow.h \
	doublecrossswitchstudy.h \
	railstudy.h \
	signalstudy.h \
	switchstudy.h \
	ctrl/regularswitchcontrollermock.h \
	ctrl/doublecrossswitchcontrollermock.h \
	ctrl/railcontrollermock.h \
	ctrl/signalcontrollermock.h \
	widgetsaver.h

FORMS += \
	doublecrossswitchstudy.ui \
	mainwindow.ui \
	railstudy.ui \
	signalstudy.ui \
	switchstudy.ui

LIBS            += -lMRW-UI -lMRW-Ctrl -lMRW-Model -lMRW-Can -lMRW-Util

QMAKE_CLEAN     += $$TARGET qtest*.xml
