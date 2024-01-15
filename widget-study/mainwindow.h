//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef STUDY_MAINWINDOW_H
#define STUDY_MAINWINDOW_H

#include <QMainWindow>

#include <can/mrwbusservice.h>
#include <mock/signalcontrollermock.h>
#include <mock/regularswitchcontrollermock.h>

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

private slots:
	void on_actionQuit_triggered();
	void on_actionSaveWidget_triggered();
	void on_actionTabLeft_triggered();
	void on_actionTabRight_triggered();

private:
	Ui::MainWindow * ui;

	void changePage(const int offset);
};

#endif
