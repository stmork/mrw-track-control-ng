QT += core gui widgets xml

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	mainwindow.h

FORMS += \
	mainwindow.ui

LIBS            += -lMRW-Model -lMRW-Util

QMAKE_CLEAN     += $$TARGET