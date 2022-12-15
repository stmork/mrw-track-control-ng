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

private slots:
	void on_actionQuit_triggered();
	void on_actionSaveWidget_triggered();
	void on_actionLeft_triggered();
	void on_actionRight_triggered();

private:
	Ui::MainWindow * ui;

	void changePage(const int offset);
};

#endif
