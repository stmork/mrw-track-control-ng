//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "modelrepository.h"

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
	MainWindow(ModelRepository & model_repo, QWidget * parent = nullptr);
	~MainWindow();

private:
	void initRegion();

private:
	Ui::MainWindow *  ui;
	ModelRepository & repo;
};
#endif // MAINWINDOW_H
