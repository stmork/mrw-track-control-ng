//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>

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
	void on_clearSection_clicked();
	void on_clearAllSections_clicked();
	void move(int right, int down);
	void extend(int inc);
	void lineup(int inc);
	void on_actionExpand_triggered();
	void on_actionInclination_triggered();
	void bend(const mrw::model::Position::Bending bend);

	void on_actionTurnSwitchLeft_triggered();
	void on_actionTurnSwitch_triggered();
	void on_actionTurnSwitchRight_triggered();

private:
	void initRegion();
	void expandBorder(RegionForm * form, mrw::ctrl::BaseController * controller, mrw::model::Position * position);
	void edit(std::function<void(
			mrw::ctrl::BaseController *,
			mrw::model::Position *)> editor);

	Ui::MainWindow  * ui;
	ModelRepository & repo;
};
#endif // MAINWINDOW_H
