#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Model VERSION 2.1
    DESCRIPTION "MRW component model library"
    LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

set(SOURCES
	abstractswitch.cpp
	assemblypart.cpp
	controller.cpp
	device.cpp
	doublecrossswitch.cpp
	formsignal.cpp
	light.cpp
	lightmodule.cpp
	lightsignal.cpp
	modelrailway.cpp
	modelrepository.cpp
	module.cpp
	multiplexconnection.cpp
	position.cpp
	profilelight.cpp
	rail.cpp
	railpart.cpp
	region.cpp
	regularswitch.cpp
	route.cpp
	section.cpp
	sectionmodule.cpp
	signal.cpp
	switchmodule.cpp
	switchmodulereference.cpp
)

set(HEADERS
	abstractswitch.h
	assemblypart.h
	controller.h
	device.h
	doublecrossswitch.h
	formsignal.h
	light.h
	lightmodule.h
	lightsignal.h
	modelrailway.h
	modelrepository.h
	module.h
	multiplexconnection.h
	position.h
	profilelight.h
	rail.h
	railpart.h
	region.h
	regularswitch.h
	route.h
	section.h
	sectionmodule.h
	signal.h
	switchmodule.h
	switchmodulereference.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Xml Qt6::SerialBus)
