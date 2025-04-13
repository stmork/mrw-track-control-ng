#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Util VERSION 2.1
    DESCRIPTION "MRW utility library"
    LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	batch.cpp
	batchparticipant.cpp
	clockservice.cpp
	dumphandler.cpp
	duration.cpp
	globalbatch.cpp
	hexline.cpp
	log.cpp
	properties.cpp
	random.cpp
	settings.cpp
	signalhandler.cpp
	stringutil.cpp
	termhandler.cpp
)

set(HEADERS
	batch.h
	batchparticipant.h
	clockservice.h
	constantenumerator.h
	dumphandler.h
	duration.h
	globalbatch.h
	hexline.h
	log.h
	method.h
	properties.h
	random.h
	self.h
	settings.h
	signalhandler.h
	singleton.h
	stringutil.h
	termhandler.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
