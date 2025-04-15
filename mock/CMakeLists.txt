#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-CtrlMock VERSION 2.2
	DESCRIPTION "MRW MVC controller mocking library"
	LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	regularswitchcontrollermock.cpp
	doublecrossswitchcontrollermock.cpp
	railcontrollermock.cpp
	signalcontrollermock.cpp
)

set(HEADERS
	regularswitchcontrollermock.h
	doublecrossswitchcontrollermock.h
	railcontrollermock.h
	signalcontrollermock.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Xml Qt6::SerialBus)
