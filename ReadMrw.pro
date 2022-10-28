TEMPLATE = subdirs
SUBDIRS  = \
	model \
	util \
	test \
	track-control-ng

model.file                = model/MRW-Model.pro
util.file                 = util/MRW-Util.pro
test.file                 = test/MRW-Test.pro
track-control-ng.file     = track-control-ng/MRW-Reader.pro

model.depends             = util
test.depends              = util model
track-control-ng.depends  = util model

#####################################################################
#
# Extra cppcheck target
#
#####################################################################

astyle.commands = astyle *.cpp *.h */*.cpp */*.h

cppcheck.commands = cppcheck -I$$[QT_INSTALL_HEADERS]\
	--inline-suppr\
	--enable=style,warning,performance,portability\
	--language=c++ --std=c++14\
	--library=qt\
	--xml-version=2 --force -q\
	*.h *.cpp */*.cpp */*.h 2>cppcheck.xml

QMAKE_EXTRA_TARGETS += cppcheck astyle
QMAKE_CLEAN         += $$TARGET cppcheck.xml *.qch
