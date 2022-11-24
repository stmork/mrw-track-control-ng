//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include <ctrl/basecontroller.h>

#include "modelrepository.h"
#include "regionform.h"

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
	explicit MainWindow(ModelRepository & model_repo, QWidget * parent = nullptr);
	~MainWindow();

public slots:
	void itemClicked(QListWidgetItem * item);

private slots:
	void clearSelectedItems();
	void clearAllItems();
	void move(int right, int down);
	void extend(int inc);
	void expand();
	void incline();

private:
	void initRegion();
	void expandBorder(RegionForm * form, mrw::ctrl::BaseController * controller, mrw::model::Position * position);

private:
	Ui::MainWindow  * ui;
	ModelRepository & repo;
};
#endif // MAINWINDOW_H
