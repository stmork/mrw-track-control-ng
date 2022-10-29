CONFIG          += c++17
QMAKE_CXXFLAGS  += -Wextra -fstack-protector-strong
QMAKE_CXXFLAGS  += -Wshadow
QMAKE_CXXFLAGS  += -Wsuggest-override

INCLUDEPATH      = ..

LIBS            += -L../model -L../util

linux:CONFIG(gcov) {
	CONFIG         += debug
	CONFIG         -= release

	QMAKE_CXXFLAGS += --coverage
	QMAKE_LFLAGS   += --coverage
	LIBS           += -lgcov
}

QMAKE_CLEAN += *.gcno *gcda
