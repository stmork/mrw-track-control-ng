#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Reader VERSION 2.2
	DESCRIPTION "MRW model reader test tool"
	LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Xml)

set(SOURCES
	main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ../..)

target_link_libraries(${PROJECT_NAME} PRIVATE
	MRW-Model MRW-Can MRW-Statecharts MRW-Util
	Qt6::Xml Qt6::SerialBus)

install(TARGETS ${PROJECT_NAME} DESTINATION "${tool_dest}")
