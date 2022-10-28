QT += core testlib
CONFIG += testcase

include(../common.pri)

SOURCES += \
	main.cpp \
	testutil.cpp

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET

HEADERS += \
	testutil.h

#####################################################################
#
# Extra target valgrind
#
#####################################################################

valgrind.commands = valgrind\
	--tool=memcheck --leak-check=full\
	--show-reachable=no --undef-value-errors=no --track-origins=no\
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no\
	--xml=yes --xml-file=valgrind.xml ./MRW-Test

QMAKE_EXTRA_TARGETS += valgrind
QMAKE_CLEAN         += valgrind.xml
