#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-TrackControl VERSION 2.2
	DESCRIPTION "MRW track control application"
	LANGUAGES CXX)

find_package(X11 OPTIONAL_COMPONENTS X11 Xext)
pkg_check_modules(libdrm IMPORTED_TARGET libdrm)

foreach(INC_DIR IN LISTS Qt6Gui_PRIVATE_INCLUDE_DIRS)
	find_file(RES "qplatformscreen.h" PATHS "${INC_DIR}" PATH_SUFFIXES "qpa" NO_CACHE)
	if (RES)
		set(QT_PRIVATE_API_FOUND "YES")
	endif()
endforeach()

add_compile_options(-Wsuggest-override)

option(X11_SCREEN_SAVER "Use X11 screen blanking." ON)
option(DRM_SCREEN_SAVER "Use DRM screen blanking." OFF)
option(QT_SCREEN_SAVER "Use Qt screen blanking." OFF)

if (LINUX)
	add_compile_definitions(USE_SYSTEMD)
endif()

if (X11_SCREEN_SAVER AND X11_FOUND AND XEXT_FOUND)
	message (STATUS "Using X11 screen saver.")
	add_compile_definitions(X11_SCREEN_SAVER)
endif()

if (DRM_SCREEN_SAVER AND libdrm_FOUND)
	message (STATUS "Using DRM screen saver.")
	add_compile_definitions(DRM_SCREEN_SAVER)
endif()

if (QT_SCREEN_SAVER AND QT_PRIVATE_API_FOUND)
	message (STATUS "Using Qt private API screen saver.")
	add_compile_definitions(QT_SCREEN_SAVER)
endif()

set(SOURCES
	controlledroute.cpp
	ctrl/controllerregistrand.cpp
	ctrl/controllerregistry.cpp
	ctrl/doublecrossswitchcontrollerproxy.cpp
	ctrl/railpartinfo.cpp
	ctrl/railcontrollerproxy.cpp
	ctrl/regularswitchcontrollerproxy.cpp
	ctrl/sectioncontroller.cpp
	ctrl/signalcontrollerproxy.cpp
	ctrl/signalproxy.cpp
	ctrl/switchcontroller.cpp
	log.cpp
	main.cpp
	mainwindow.cpp
	mrwmessagedispatcher.cpp
	beermodeservice.cpp
	regionform.cpp
	routebatch.cpp
	screenblankhandler.cpp
	ui/routelistwidget.cpp
	ui/sectionlistwidget.cpp
)

set(HEADERS
	beermodeservice.h
	controlledroute.h
	ctrl/controllerregistrand.h
	ctrl/controllerregistry.h
	ctrl/doublecrossswitchcontrollerproxy.h
	ctrl/railcontrollerproxy.h
	ctrl/railpartinfo.h
	ctrl/regularswitchcontrollerproxy.h
	ctrl/sectioncontroller.h
	ctrl/signalcontrollerproxy.h
	ctrl/signalproxy.h
	ctrl/switchcontroller.h
	log.h
	mainwindow.h
	mrwmessagedispatcher.h
	regionform.h
	routebatch.h
	screenblankhandler.h
	ui/routelistwidget.h
	ui/sectionlistwidget.h
)

set(FORMS
	mainwindow.ui
	regionform.ui
)

set(RESOURCES
	resources.qrc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} PRIVATE
	MRW-UI MRW-Ctrl MRW-Model MRW-Can MRW-Statecharts MRW-Log MRW-Util
	Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::SerialBus Qt6::Xml
)

if (LINUX)
	target_link_libraries(${PROJECT_NAME} PRIVATE systemd)
endif()

if (X11_SCREEN_SAVER AND X11_FOUND AND XEXT_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE X11 Xext)
endif()

if (DRM_SCREEN_SAVER AND libdrm_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::libdrm)
endif()

if (QT_SCREEN_SAVER AND QT_PRIVATE_API_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::GuiPrivate)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION "${tool_dest}")
