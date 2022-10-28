QT             += core gui widgets xml
CONFIG         += c++17
QMAKE_CXXFLAGS += -Wextra -Wshadow -Wsuggest-override

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	model/area.cpp \
	model/controller.cpp \
	model/formsignal.cpp \
	model/light.cpp \
	model/lightmodule.cpp \
	model/lightsignal.cpp \
	model/module.cpp \
	model/mrwmodel.cpp \
	model/multiplexconnection.cpp \
	model/rail.cpp \
	model/raildoubleswitch.cpp \
	model/railpart.cpp \
	model/railswitch.cpp \
	model/section.cpp \
	model/sectionmodule.cpp \
	model/signal.cpp \
	model/switchmodule.cpp \
	util/method.cpp

HEADERS += \
	mainwindow.h \
	model/area.h \
	model/controller.h \
	model/formsignal.h \
	model/light.h \
	model/lightmodule.h \
	model/lightsignal.h \
	model/module.h \
	model/mrwmodel.h \
	model/multiplexconnection.h \
	model/rail.h \
	model/raildoubleswitch.h \
	model/railpart.h \
	model/railswitch.h \
	model/section.h \
	model/sectionmodule.h \
	model/signal.h \
	model/switchmodule.h \
	util/method.h \
	util/singleton.h \
	util/string.h

FORMS += \
	mainwindow.ui

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
