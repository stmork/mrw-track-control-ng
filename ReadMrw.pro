TEMPLATE = subdirs
SUBDIRS  = \
	model \
	util \
	test \
	track-control-ng

QMAKE_CLEAN              += qtest*.xml valgrind*.xml

model.file                = model/MRW-Model.pro
util.file                 = util/MRW-Util.pro
test.file                 = test/MRW-Test.pro
track-control-ng.file     = track-control-ng/MRW-Reader.pro

model.depends             = util
test.depends              = util model
track-control-ng.depends  = util model

#####################################################################
#
# Extra astyle and cppcheck target
#
#####################################################################

astyle.commands = astyle */*.cpp */*.h

cppcheck.commands = cppcheck -I$$[QT_INSTALL_HEADERS]\
	--inline-suppr\
	--enable=style,warning,performance,portability\
	--language=c++ --std=c++14\
	--library=qt\
	--xml-version=2 --force -q\
	*/*.cpp */*.h 2>cppcheck.xml

QMAKE_EXTRA_TARGETS += cppcheck astyle
QMAKE_CLEAN         += cppcheck.xml

#####################################################################
#
# Extra Doxygen target
#
#####################################################################

doxygen.commands = doxygen

QMAKE_EXTRA_TARGETS += doxygen
QMAKE_CLEAN         += $$_PRO_FILE_PWD_/doc/ *.qch

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

LCOV_DIR       = $$_PRO_FILE_PWD_/lcov-out

#  --no-external
lcov.commands += lcov -c -d $$_PRO_FILE_PWD_ -o coverage.info;
lcov.commands += genhtml coverage.info --legend --title=\"MRW-NG Test Coverage\" --output-directory $$LCOV_DIR --rc genhtml_med_limit=50

QMAKE_EXTRA_TARGETS += lcov
QMAKE_CLEAN         += $$LCOV_DIR/ coverage.info
