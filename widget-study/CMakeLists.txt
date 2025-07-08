#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-WidgetStudy VERSION 2.2
	DESCRIPTION "MRW widget study application"
	LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Xml)

add_compile_options(-Wsuggest-override)

set(SOURCES
	main.cpp
	mainwindow.cpp
	doublecrossswitchstudy.cpp
	railstudy.cpp
	signalstudy.cpp
	switchstudy.cpp
	widgetsaver.cpp
	widgetstudy.cpp
)

set(HEADERS
	mainwindow.h
	doublecrossswitchstudy.h
	railstudy.h
	signalstudy.h
	switchstudy.h
	widgetsaver.h
	widgetstudy.h
)

set(FORMS
	doublecrossswitchstudy.ui
	mainwindow.ui
	railstudy.ui
	signalstudy.ui
	switchstudy.ui
	widgetstudy.ui
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS})

target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} PRIVATE
   MRW-UI MRW-Ctrl MRW-CtrlMock MRW-Model MRW-Can MRW-Util
   Qt6::Widgets Qt6::SerialBus Qt6::Xml
)
