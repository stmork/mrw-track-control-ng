#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
#

TEMPLATE = subdirs
SUBDIRS  = \
	util \
	can \
	statecharts \
	model \
	ctrl \
	ui \
	test \
	ping \
	proxy \
	track-control-ng \
	study

util.file                 = util/MRW-Util.pro
can.file                  = can/MRW-Can.pro
statecharts.file          = statecharts/MRW-Statecharts.pro
model.file                = model/MRW-Model.pro
ctrl.file                 = ctrl/MRW-Ctrl.pro
ui.file                   = ui/MRW-UI.pro
test.file                 = test/MRW-Test.pro
ping.file                 = tools/ping/MRW-Ping.pro
proxy.file                = tools/proxy/MRW-Proxy.pro
track-control-ng.file     = track-control-ng/MRW-Reader.pro
study.file                = MRW-WidgetStudy/MRW-WidgetStudy.pro

can.depends               = util
model.depends             = util can
test.depends              = util can model
ctrl.depends              = model can util
ui.depends                = ctrl
ping.depends              = test
proxy.depends             = test
study.depends             = test ctrl ui
track-control-ng.depends  = study statecharts

QMAKE_CLEAN              += qtest*.xml

#####################################################################
#
# Extra astyle and cppcheck target
#
#####################################################################

astyle.commands = astyle */*.cpp */*.h tools/*/*.cpp */ctrl/*.cpp */ctrl/*.h

cppcheck.commands = cppcheck -I$$[QT_INSTALL_HEADERS]\
	--inline-suppr\
	--suppress=noExplicitConstructor:statecharts/*.h\
	--suppress=duplicateBreak:statecharts/*.cpp\
	--suppress=redundantAssignment:statecharts/*.cpp\
	--enable=style,warning,performance,portability\
	--language=c++ --std=c++14\
	--library=qt\
	--xml-version=2 --force -q -j 4\
	*/*.cpp */*.h tools/*/*.cpp */ctrl/*.cpp */ctrl/*.h 2>cppcheck.xml

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
