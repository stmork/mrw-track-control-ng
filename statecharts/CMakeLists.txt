#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Statecharts VERSION 2.2
	DESCRIPTION "MRW statechart library"
	LANGUAGES CXX)

add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	ConfigStatechart.cpp
	OperatingModeStatechart.cpp
	RouteStatechart.cpp
	SectionStatechart.cpp
	SignalControllerStatechart.cpp
	SignalStatechart.cpp
	SwitchStatechart.cpp
	TrackerStatechart.cpp
	UpdateStatechart.cpp
	timerservice.cpp
)

set(HEADERS
	ConfigStatechart.h
	OperatingModeStatechart.h
	RouteStatechart.h
	SectionStatechart.h
	SignalControllerStatechart.h
	SignalStatechart.h
	SwitchStatechart.h
	TrackerStatechart.h
	UpdateStatechart.h
	common/sc_eventdriven.h
	common/sc_statemachine.h
	common/sc_timer.h
	common/sc_types.h
	timerservice.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
