#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
#

CONFIG          += c++20
CONFIG          += link_pkgconfig

QMAKE_CXXFLAGS  += -Wextra -pedantic -Werror
QMAKE_CXXFLAGS  += -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow
QMAKE_CXXFLAGS  += -Wsuggest-override
