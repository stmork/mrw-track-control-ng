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
#include <ctrl/regularswitchcontrollermock.h>

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
	void setupSignals();
	void setupRegularSwitches();


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

	mrw::ctrl::RegularSwitchControllerMock mock_rss1;
	mrw::ctrl::RegularSwitchControllerMock mock_rss2;
	mrw::ctrl::RegularSwitchControllerMock mock_rss3;

	mrw::ctrl::RegularSwitchControllerMock mock_top_lf;
	mrw::ctrl::RegularSwitchControllerMock mock_top_rf;
	mrw::ctrl::RegularSwitchControllerMock mock_top_lb;
	mrw::ctrl::RegularSwitchControllerMock mock_top_rb;
	mrw::ctrl::RegularSwitchControllerMock mock_bottom_lf;
	mrw::ctrl::RegularSwitchControllerMock mock_bottom_rf;
	mrw::ctrl::RegularSwitchControllerMock mock_bottom_lb;
	mrw::ctrl::RegularSwitchControllerMock mock_bottom_rb;
};

#endif
