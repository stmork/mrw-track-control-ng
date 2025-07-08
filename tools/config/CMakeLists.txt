#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Configure VERSION 2.2
	DESCRIPTION "MRW configuration tool"
	LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Xml)

set(SOURCES
	configurationservice.cpp
	main.cpp
)

set(HEADERS
	configurationservice.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ../..)

target_link_libraries(${PROJECT_NAME} PRIVATE
	MRW-Model MRW-Can MRW-Statecharts MRW-Util
	Qt6::SerialBus Qt6::Xml)

install(TARGETS ${PROJECT_NAME} DESTINATION "${tool_dest}")
