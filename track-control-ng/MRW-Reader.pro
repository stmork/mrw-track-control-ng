QT  += core gui widgets xml

include(../common.pri)

SOURCES += \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	mainwindow.h

FORMS += \
	mainwindow.ui

QMAKE_CLEAN         += $$TARGET
