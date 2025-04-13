#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

cmake_minimum_required(VERSION 3.16)

project(MRW-Tools VERSION 2.1
    DESCRIPTION "MRW tools"
    LANGUAGES CXX)

add_compile_options(-Wsuggest-override)

add_subdirectory(config)
add_subdirectory(ping)
add_subdirectory(proxy)
add_subdirectory(reader)
add_subdirectory(reset)
add_subdirectory(sim)
add_subdirectory(tracker)
add_subdirectory(update)
