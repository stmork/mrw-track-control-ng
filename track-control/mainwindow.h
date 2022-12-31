//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>
#include <random>

#include <QMainWindow>
#include <QListWidgetItem>

#include <statecharts/OperatingMode.h>
#include <model/modelrepository.h>
#include <model/rail.h>
#include <model/route.h>
#include <ctrl/basecontroller.h>

#include "regionform.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MrwMessageDispatcher;
class WidgetRoute;

class MainWindow :
	public QMainWindow,
	public mrw::statechart::OperatingMode::OperationCallback
{
	Q_OBJECT

public:
	explicit MainWindow(
		mrw::model::ModelRepository   &  repository,
		MrwMessageDispatcher      &      dispatcher,
		QWidget             *            parent = nullptr);
	~MainWindow();

private slots:
	void itemClicked(QListWidgetItem * item);
	void enable();

	void on_actionTabLeft_triggered();
	void on_actionTabRight_triggered();

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
	void on_actionLock_triggered();
	void on_actionUnlock_triggered();
	void on_actionBeermodeLeft_triggered();
	void on_actionBeermodeRight_triggered();

	void onInit();
	void onOperate(const bool active);
	void onEdit(const bool active);
	void onFailed();

	void routeFinished();

private:
	typedef std::function<void(
		mrw::ctrl::BaseController *,
		mrw::model::Position *)> Callback;

	void initRegion(MrwMessageDispatcher & dispatcher);
	void connectEditActions();
	void connectOpModes(MrwMessageDispatcher & dispatcher);

	void expandBorder(
		RegionForm         *        form,
		mrw::ctrl::BaseController * controller,
		mrw::model::Position    *   position);
	mrw::model::RailPart * rail(const int idx) const;
	void traverse(Callback callback);
	bool isSameRegion();

	template <class T> size_t count()
	{
		size_t count = 0;

		traverse([&] (mrw::ctrl::BaseController * ctrl, mrw::model::Position * pos)
		{
			Q_UNUSED(pos);
			const T * ptr = dynamic_cast<T *>(ctrl);

			if (ptr != nullptr)
			{
				count++;
			}
		}
		);
		return count;
	}

	virtual void        reset() override;
	virtual bool        hasActiveRoutes() override;

	mrw::model::Route * createRoute(
		const bool                     direction,
		const mrw::model::SectionState state);
	void addRoute(WidgetRoute * route);
	void                changePage(const int offset);

	void findCandidates(
		std::vector<mrw::model::Rail *> & candidates,
		const bool                        dir) const;
	void findPassthrough(
		std::vector<mrw::model::Rail *> & candidates,
		const mrw::model::Region     *    region,
		const bool                        is_same) const;

	static bool isPassThrough(
		const mrw::model::Rail * rail);
	static mrw::model::Rail * isNeighbourOccupied(
		const mrw::model::Rail * rail,
		const bool               dir);
	static bool isNeighbourFree(
		const mrw::model::Rail * rail,
		const bool               dir);

	int random(const size_t size) const;
	mrw::model::Rail * random(const std::vector<mrw::model::Rail *> & rails) const;
	void startBeermode(const bool dir);
	void dump(
		const std::vector<mrw::model::Rail *> & rails);
	QString dump(const mrw::model::Rail * rail);

	Ui::MainWindow          *         ui;
	mrw::model::ModelRepository   &   repo;
	mrw::model::Route        *        beer_route = nullptr;
	mrw::statechart::OperatingMode    statechart;

	mutable std::random_device        rd;
	mutable std::mt19937              rng;
};

#endif // MAINWINDOW_H
