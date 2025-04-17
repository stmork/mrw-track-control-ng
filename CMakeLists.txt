#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-NextGeneration VERSION 2.2
    DESCRIPTION "MRW next generation track control"
    LANGUAGES CXX)

find_package(QT NAMES Qt6)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network SerialBus Xml Test)
find_package(PkgConfig REQUIRED)
find_package(Doxygen REQUIRED COMPONENTS dot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(MRW_BASE ${PROJECT_SOURCE_DIR})
set(tool_dest "${PROJECT_SOURCE_DIR}/rootfs/usr/bin")

add_compile_options(-Wextra -Wpedantic -Werror)
add_compile_options(-fstack-protector-strong)
add_compile_options(-Wshadow)

if(DEFINED ENV{BUILD_NUMBER})
	set(BUILD_NUMBER $ENV{BUILD_NUMBER})
	message ("Build: ${BUILD_NUMBER}")
	add_compile_definitions(BUILD_NUMBER=${BUILD_NUMBER})
else()
	message ("Build number undefined.")
endif()

enable_testing()

add_subdirectory(util)
add_subdirectory(log)
add_subdirectory(can)
add_subdirectory(model)
add_subdirectory(ctrl)
add_subdirectory(mock)
add_subdirectory(ui)
add_subdirectory(statecharts)
add_subdirectory(test)
add_subdirectory(tools)
add_subdirectory(widget-study)
add_subdirectory(track-control)

add_custom_target(astyle
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND astyle --options=${PROJECT_SOURCE_DIR}/.astylerc
	*/*.cpp */*.h
	tools/*/*.cpp tools/*/*.h
	*/ctrl/*.cpp */ctrl/*.h
	*/ui/*.cpp */ui/*.h
	*/test/*/*.cpp */test/*/*.h
)

add_custom_target(cppcheck
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND cppcheck
	--inline-suppr
	--suppress=unusedStructMember
	--suppress=preprocessorErrorDirective
	--suppress=duplicateConditionalAssign:statecharts/*.cpp
	--suppress=noExplicitConstructor:statecharts/*.h
	--suppress=redundantInitialization:statecharts/*.cpp
	--suppress=duplicateBreak:statecharts/*.cpp
	--suppress=redundantAssignment:statecharts/*.cpp
	--enable=style,warning,performance,portability
	--language=c++ --std=c++14
	--library=qt
	--xml-version=2 --force -q -j `nproc`
	*/*.cpp */*.h
	tools/*/*.cpp tools/*/*.h
	*/ctrl/*.cpp */ctrl/*.h
	*/ui/*.cpp */ui/*.h
	2>cppcheck.xml
)

set_property(
	TARGET cppcheck
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_SOURCE_DIR}/cppcheck.xml
)

add_custom_target(cccc
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMAND cccc  --lang=c++ --outdir=cccc
	can/*.cpp can/*.h
	ctrl/*.cpp ctrl/*.h
	mock/*.cpp mock/*.h
	model/*.cpp model/*.h
	ui/*.cpp ui/*.h
	util/*.cpp util/*.h
	tools/*/*.cpp tools/*/*.h
	track*/*.cpp track*/*.h
	track*/*/*.cpp track*/*/*.h
)

set_property(
	TARGET cccc
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_SOURCE_DIR}/cccc/
)

add_custom_target(valgrind
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	DEPENDS MRW-Test
	COMMAND valgrind
	--suppressions=offscreen.supp
	--tool=memcheck --leak-check=full
	--show-reachable=no --undef-value-errors=no --track-origins=no
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no
	--xml=yes --xml-file=valgrind.xml ${CMAKE_BINARY_DIR}/test/MRW-Test -platform offscreen
)

set_property(
	TARGET valgrind
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES
	${CMAKE_SOURCE_DIR}/valgrind.xml
	${CMAKE_SOURCE_DIR}/qtest-*.xml
)

add_custom_target(sct-unit
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/statecharts/test
	DEPENDS MRW-Test-Statecharts
	COMMAND ${CMAKE_BINARY_DIR}/statecharts/test/MRW-Test-Statecharts --gtest_output=xml:gtest-mrw.xml
)

set_property(
	TARGET sct-unit
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES
	${CMAKE_SOURCE_DIR}/statecharts/test/gtest-mrw.xml
)

add_custom_target(doxygen
	COMMAND doxygen
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
