#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE = subdirs
SUBDIRS  = \
	can \
	util \
	model \
	test \
	ping \
	track-control-ng

QMAKE_CLEAN              += qtest*.xml valgrind*.xml

can.file                  = can/MRW-Can.pro
util.file                 = util/MRW-Util.pro
model.file                = model/MRW-Model.pro
test.file                 = test/MRW-Test.pro
ping.file                 = tools/ping/MRW-Ping.pro
track-control-ng.file     = track-control-ng/MRW-Reader.pro

model.depends             = util can
test.depends              = util can model
ping.depends              = test
track-control-ng.depends  = util can model

#####################################################################
#
# Extra astyle and cppcheck target
#
#####################################################################

astyle.commands = astyle */*.cpp */*.h tools/*/*.cpp

cppcheck.commands = cppcheck -I$$[QT_INSTALL_HEADERS]\
	--inline-suppr\
	--enable=style,warning,performance,portability\
	--language=c++ --std=c++14\
	--library=qt\
	--xml-version=2 --force -q\
	*/*.cpp */*.h tools/*/*.cpp 2>cppcheck.xml

QMAKE_EXTRA_TARGETS += cppcheck astyle
QMAKE_CLEAN         += cppcheck.xml

#####################################################################
#
# Extra Doxygen target
#
#####################################################################

doxygen.commands = doxygen

QMAKE_EXTRA_TARGETS += doxygen
QMAKE_CLEAN         += *.qch

#####################################################################
#
# Extra target valgrind
#
#####################################################################

valgrind.commands = valgrind\
	--tool=memcheck --leak-check=full\
	--show-reachable=no --undef-value-errors=no --track-origins=no\
	--child-silent-after-fork=no --trace-children=no --gen-suppressions=no\
	--xml=yes --xml-file=valgrind.xml test/MRW-Test

QMAKE_EXTRA_TARGETS += valgrind
QMAKE_CLEAN         += valgrind.xml

#####################################################################
#
# Extra target lcov
#
#####################################################################

LCOV_DIR = $$(PWD)/lcov-out

lcov.commands += lcov -c --no-external -d $$(PWD) -o coverage.info\
	--exclude \"$$(PWD)/*/moc_*\"\
	--exclude \"$$(PWD)/test/*\";
lcov.commands += genhtml coverage.info -s --num-spaces 4 --legend -t \"MRW-NG Test Coverage\" --output-directory $$LCOV_DIR --rc genhtml_med_limit=50

QMAKE_EXTRA_TARGETS += lcov
QMAKE_CLEAN         += coverage.info