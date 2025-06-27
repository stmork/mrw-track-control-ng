#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Test-Statecharts VERSION 2.2
	DESCRIPTION "MRW statechart tests"
	LANGUAGES CXX)

add_compile_options(-Wno-unused-parameter)

find_package(GTest REQUIRED)

set(SOURCES
	src-gen/ConfigStatechart.cpp
	src-gen/ConfigTest.cpp
	src-gen/OperatingModeStatechart.cpp
	src-gen/OperatingModeTest.cpp
	src-gen/RouteStatechart.cpp
	src-gen/RouteTest.cpp
	src-gen/SectionStatechart.cpp
	src-gen/SectionTest.cpp
	src-gen/SignalControllerStatechart.cpp
	src-gen/SignalControllerTest.cpp
	src-gen/SignalStatechart.cpp
	src-gen/SignalTest.cpp
	src-gen/SwitchStatechart.cpp
	src-gen/SwitchTest.cpp
	src-gen/TrackerStatechart.cpp
	src-gen/TrackerTest.cpp
	src-gen/UpdateStatechart.cpp
	src-gen/UpdateTest.cpp
	common/sc_runner_timed.cpp
	common/sc_timer_service.cpp
)

set(HEADERS
	src-gen/ConfigStatechart.h
	src-gen/OperatingModeStatechart.h
	src-gen/RouteStatechart.h
	src-gen/SectionStatechart.h
	src-gen/SignalControllerStatechart.h
	src-gen/SignalStatechart.h
	src-gen/SwitchStatechart.h
	src-gen/TrackerStatechart.h
	src-gen/UpdateStatechart.h
	common/sc_cyclebased.h
	common/sc_eventdriven.h
	common/sc_runner_timed.h
	common/sc_statemachine.h
	common/sc_timer.h
	common/sc_timer_service.h
	common/sc_types.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE common)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

add_test(
	NAME sct-unit
	COMMAND ${PROJECT_BINARY_DIR}/${PROJECT_NAME} --gtest_output=xml:gtest-mrw.xml
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
