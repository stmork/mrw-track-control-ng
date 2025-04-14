#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Test VERSION 2.2
    DESCRIPTION "MRW unit tests"
    LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

set(SOURCES
	collections.cpp
	main.cpp
	testcan.cpp
	testdoublecrossswitchwidget.cpp
	testflankswitch.cpp
	testmodel.cpp
	testmodelbase.cpp
	testnumbering.cpp
	testrailwidget.cpp
	testregularswitchwidget.cpp
	testrouting.cpp
	testsignalwidget.cpp
	testunknown.cpp
	testswitch.cpp
	testlight.cpp
	testutil.cpp
)

set(HEADERS
	collections.h
	testcan.h
	testdef.h
	testdoublecrossswitchwidget.h
	testflankswitch.h
	testmodel.h
	testmodelbase.h
	testnumbering.h
	testrailwidget.h
	testregularswitchwidget.h
	testroute.h
	testrouting.h
	testsignalwidget.h
	testunknown.h
	testswitch.h
	testlight.h
	testutil.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} PRIVATE
   MRW-UI MRW-Ctrl MRW-CtrlMock MRW-Model MRW-Can MRW-Util
   Qt6::Core Qt6::Widgets Qt6::Network Qt6::SerialBus Qt6::Xml Qt6::Test
)
