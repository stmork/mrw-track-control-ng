//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef STUDY_MAINWINDOW_H
#define STUDY_MAINWINDOW_H

#include <QMainWindow>

#include <can/mrwbusservice.h>
#include <ctrl/signalcontrollermock.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow * ui;

	mrw::ctrl::SignalControllerMock mock_ss1;
	mrw::ctrl::SignalControllerMock mock_ss2;
	mrw::ctrl::SignalControllerMock mock_ss3;

	mrw::ctrl::SignalControllerMock mock_sb1;
	mrw::ctrl::SignalControllerMock mock_sb2;
	mrw::ctrl::SignalControllerMock mock_sb3;
	mrw::ctrl::SignalControllerMock mock_sb4;
	mrw::ctrl::SignalControllerMock mock_sb5;
	mrw::ctrl::SignalControllerMock mock_sb6;
};

#endif
