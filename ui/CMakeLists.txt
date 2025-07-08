#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-UI VERSION 2.2
	DESCRIPTION "MRW MVC user interface library"
	LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Xml)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	barwidget.cpp
	basewidget.cpp
	circlelivewidget.cpp
	clockwidget.cpp
	controllerwidget.cpp
	doublecrossswitchwidget.cpp
	opmodewidget.cpp
	regularswitchwidget.cpp
	railwidget.cpp
	signalwidget.cpp
	stationwidget.cpp
)

set(HEADERS
	barwidget.h
	basewidget.h
	circlelivewidget.h
	clockwidget.h
	controllerwidget.h
	doublecrossswitchwidget.h
	opmodewidget.h
	regularswitchwidget.h
	railwidget.h
	signalwidget.h
	stationwidget.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::SerialBus Qt6::Xml)
