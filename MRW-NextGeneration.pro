#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#

TEMPLATE = subdirs
SUBDIRS  = \
	util \
	log \
	can \
	statecharts \
	model \
	ctrl \
	mock \
	ui \
	test \
	test-sct \
	ping \
	reset \
	reader \
	sim \
	proxy \
	tracker \
	config \
	update \
	study \
	track-control

util.file              = util/MRW-Util.pro
log.file               = log/MRW-Log.pro
can.file               = can/MRW-Can.pro
statecharts.file       = statecharts/MRW-Statecharts.pro
model.file             = model/MRW-Model.pro
ctrl.file              = ctrl/MRW-Ctrl.pro
mock.file              = mock/MRW-CtrlMock.pro
ui.file                = ui/MRW-UI.pro
test.file              = test/MRW-Test.pro
test-sct.file          = statecharts/test/MRW-Test-Statecharts.pro
ping.file              = tools/ping/MRW-Ping.pro
reset.file             = tools/reset/MRW-Reset.pro
reader.file            = tools/reader/MRW-Reader.pro
sim.file               = tools/sim/MRW-Simulator.pro
proxy.file             = tools/proxy/MRW-Proxy.pro
tracker.file           = tools/tracker/MRW-Tracker.pro
config.file            = tools/config/MRW-Configure.pro
update.file            = tools/update/MRW-Update.pro
study.file             = widget-study/MRW-WidgetStudy.pro
track-control.file     = track-control/MRW-TrackControl.pro

log.depends            = util
can.depends            = util
model.depends          = util can
test.depends           = util can model
ctrl.depends           = model can util
mock.depends           = ctrl
ui.depends             = ctrl
ping.depends           = test
reset.depends          = test
reader.depends         = test
sim.depends            = test
proxy.depends          = test
tracker.depends        = test statecharts
config.depends         = test statecharts
update.depends         = test statecharts
study.depends          = test ctrl mock ui
track-control.depends  = log study statecharts

QMAKE_CLEAN           += qtest*.xml *.log

#####################################################################
#
# Extra astyle and cppcheck target
#
#####################################################################

astyle.commands = astyle --options=$$PWD/.astylerc\
	*/*.cpp */*.h\
	tools/*/*.cpp tools/*/*.h\
	*/ctrl/*.cpp */ctrl/*.h\
	*/ui/*.cpp */ui/*.h\
	*/test/*/*.cpp */test/*/*.h

cppcheck.commands = cppcheck -I$$[QT_INSTALL_HEADERS]\
	--inline-suppr\
	--suppress=unusedStructMember\
	--suppress=noExplicitConstructor:statecharts/*.h\
	--suppress=duplicateBreak:statecharts/*.cpp\
	--suppress=redundantAssignment:statecharts/*.cpp\
	--enable=style,warning,performance,portability\
	--language=c++ --std=c++14\
	--library=qt\
	--xml-version=2 --force -q -j 4\
	*/*.cpp */*.h\
	tools/*/*.cpp tools/*/*.h\
	*/ctrl/*.cpp */ctrl/*.h\
	*/ui/*.cpp */ui/*.h\
	2>cppcheck.xml

cccc.commands = cccc --lang=c++ --outdir=cccc\
	can/*.cpp can/*.h\
	ctrl/*.cpp ctrl/*.h\
	model/*.cpp model/*.h\
	ui/*.cpp ui/*.h\
	util/*.cpp util/*.h\
	tools/*/*.cpp tools/*/*.h\
	track*/*.cpp track*/*.h\
	track*/*/*.cpp track*/*/*.h

QMAKE_EXTRA_TARGETS += cppcheck astyle cccc
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
# Extra target sct-unit
#
#####################################################################

sct-unit.commands = statecharts/test/sct-unit.sh

QMAKE_EXTRA_TARGETS += sct-unit
QMAKE_CLEAN         += statecharts/test/gtest-*.xml

#####################################################################
#
# Extra target lcov
#
#####################################################################

LCOV_DIR = $$(PWD)/lcov-out

lcov.commands += lcov -c --no-external -d $$(PWD) -o coverage.info\
	--exclude \"$$(PWD)/*/moc_*\"\
	--exclude \"$$(PWD)/statecharts/test/common/*\"\
	--exclude \"$$(PWD)/statecharts/test/*/*Test.cpp\"\
	--exclude \"$$(PWD)/test/*\";
lcov.commands += genhtml coverage.info -s --num-spaces 4 --legend -t \"MRW-NG Test Coverage\" --output-directory $$LCOV_DIR --rc genhtml_med_limit=50

QMAKE_EXTRA_TARGETS += lcov
QMAKE_CLEAN         += coverage.info *.deb
