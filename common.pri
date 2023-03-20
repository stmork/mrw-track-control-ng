#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

VERSION          = 2.0
QT              += core xml serialbus
CONFIG          += c++17
QMAKE_CXXFLAGS  += -Wextra -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow
QMAKE_CXXFLAGS  += -Wsuggest-override

INCLUDEPATH      = $$PWD

LIBS            += -L$$PWD/model -L$$PWD/util -L$$PWD/log -L$$PWD/can
LIBS            += -L$$PWD/ctrl  -L$$PWD/ui   -L$$PWD/statecharts

linux:CONFIG(gcov) {
	CONFIG         += debug
	CONFIG         -= release

	QMAKE_CXXFLAGS += --coverage
	QMAKE_LFLAGS   += --coverage
	LIBS           += -lgcov
}

install.path  = $$(PREFIX)/usr/bin

QMAKE_CLEAN += *.gcno *gcda

#####################################################################
#
# Define version/build number if available
#
#####################################################################

DEFINES += VERSION=\"\\\"$$VERSION\\\"\"

#####################################################################
#
# Define build number if available
#
#####################################################################

BUILD_NUMBER=$$getenv(BUILD_NUMBER)

!isEmpty(BUILD_NUMBER) {
	DEFINES += BUILD_NUMBER=$$(BUILD_NUMBER)
}
