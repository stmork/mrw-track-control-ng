CONFIG          += c++17
QMAKE_CXXFLAGS  += -Wextra -fstack-protector-strong

INCLUDEPATH      = ..

LIBS            += -L../model -L../util
LIBS            += -lMRW-Model -lMRW-Util
