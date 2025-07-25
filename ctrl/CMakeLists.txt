#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Ctrl VERSION 2.2
	DESCRIPTION "MRW MVC controller library"
	LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Xml)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	basecontroller.cpp
	baseswitchcontroller.cpp
	regularswitchcontroller.cpp
	doublecrossswitchcontroller.cpp
	railcontroller.cpp
	signalcontroller.cpp
)

set(HEADERS
	basecontroller.h
	baseswitchcontroller.h
	regularswitchcontroller.h
	doublecrossswitchcontroller.h
	railcontroller.h
	signalcontroller.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::SerialBus Qt6::Xml)
