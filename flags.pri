#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
#

CONFIG          += c++17
CONFIG          += link_pkgconfig

QMAKE_CXXFLAGS  += -Wextra -pedantic -Wno-deprecated-copy -Werror
QMAKE_CXXFLAGS  += -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow
QMAKE_CXXFLAGS  += -Wsuggest-override
