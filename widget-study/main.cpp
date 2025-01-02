//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);
	MainWindow   main_window;

	main_window.show();
	return app.exec();
}
