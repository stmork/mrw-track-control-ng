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

#include <statecharts/OperatingMode.h>
#include <ctrl/basecontroller.h>

#include "modelrepository.h"
#include "regionform.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MrwMessageDispatcher;

class MainWindow :
	public QMainWindow,
	public mrw::statechart::OperatingMode::OperationCallback
{
	Q_OBJECT

public:
	explicit MainWindow(
		ModelRepository    &   repository,
		MrwMessageDispatcher & dispatcher,
		QWidget        *       parent = nullptr);
	~MainWindow();

public slots:
	void itemClicked(QListWidgetItem * item);

private slots:
	void on_clearSection_clicked();
	void on_clearAllSections_clicked();
	void on_clearRoute_clicked();
	void on_clearAllRoutes_clicked();

	void on_tourLeftPushButton_clicked();
	void on_shuntLeftPushButton_clicked();
	void on_extendPushButton_clicked();
	void on_shuntRightPushButton_clicked();
	void on_tourRightPushButton_clicked();

	void on_actionExpand_triggered();
	void on_actionInclination_triggered();
	void move(int right, int down);
	void extend(int inc);
	void lineup(int inc);
	void bend(const mrw::model::Position::Bending bend);

	void on_actionTurnSwitchLeft_triggered();
	void on_actionTurnSwitch_triggered();
	void on_actionTurnSwitchRight_triggered();

	void onOperate(const bool active);
	void onEdit(const bool active);

private:
	void initRegion();
	void enable();
	void expandBorder(
		RegionForm         *        form,
		mrw::ctrl::BaseController * controller,
		mrw::model::Position    *   position);
	void edit(
		std::function<void(mrw::ctrl::BaseController *, mrw::model::Position *)> editor);
	template <class T> size_t count()
	{
		size_t count = 0;

		edit([&] (mrw::ctrl::BaseController * ctrl, mrw::model::Position * pos)
		{
			Q_UNUSED(pos);
			T * ptr = dynamic_cast<T *>(ctrl);

			if (ptr != nullptr)
			{
				count++;
			}
		}
		);
		return count;
	}

	virtual void reset() override;

	Ui::MainWindow  * ui;
	ModelRepository & repo;
	mrw::statechart::OperatingMode   statechart;
};
#endif // MAINWINDOW_H
