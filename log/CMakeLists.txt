#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Log VERSION 2.2
    DESCRIPTION "MRW logging library"
    LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	filelogger.cpp
	loggerbase.cpp
	loggerservice.cpp
	stdlogger.cpp
	syslogger.cpp
)

set(HEADERS
	filelogger.h
	loggerbase.h
	loggerservice.h
	stdlogger.h
	syslogger.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
