//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <statecharts/timerservice.h>
#include <model/modelrailway.h>
#include <ctrl/basecontroller.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/railcontroller.h>
#include <ctrl/signalcontroller.h>
#include <ctrl/controllerregistry.h>
#include <ui/controllerwidget.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regionform.h"
#include "mrwmessagedispatcher.h"
#include "route.h"

using namespace mrw::statechart;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

MainWindow::MainWindow(
	ModelRepository    &   repository,
	MrwMessageDispatcher & dispatcher,
	QWidget        *       parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	repo(repository),
	statechart(nullptr)
{
	BaseWidget::setVerbose(false);

	ui->setupUi(this);
	initRegion();

	QList<ControllerWidget *> widgets = findChildren<ControllerWidget *>();

	for (ControllerWidget * w : widgets)
	{
		connect(w, &ControllerWidget::clicked, this, &MainWindow::itemClicked);
	}

	connect(
		ui->actionQuit,        &QAction::triggered,
		QCoreApplication::instance(), &QCoreApplication::quit);

	connect(ui->actionBendLeft,    &QAction::triggered, [this]()
	{
		bend(Bending::LEFT);
	});
	connect(ui->actionBendRight,   &QAction::triggered, [this]()
	{
		bend(Bending::RIGHT);
	});

	connect(ui->actionRight,      &QAction::triggered, [this] ()
	{
		move(1, 0);
	});
	connect(ui->actionLeft,       &QAction::triggered, [this] ()
	{
		move(-1, 0);
	});
	connect(ui->actionUp,         &QAction::triggered, [this] ()
	{
		move(0, -Position::FRACTION);
	});
	connect(ui->actionDown,       &QAction::triggered, [this] ()
	{
		move(0, Position::FRACTION);
	});

	connect(ui->actionExtend,     &QAction::triggered, [this] ()
	{
		extend(1);
	});
	connect(ui->actionReduce,     &QAction::triggered, [this] ()
	{
		extend(-1);
	});

	connect(ui->actionLineUp,     &QAction::triggered, [this] ()
	{
		lineup(1);
	});
	connect(ui->actionLineDown,   &QAction::triggered, [this] ()
	{
		lineup(-1);
	});

	connect(
		ui->routeListWidget, &QListWidget::currentItemChanged,
		this, &MainWindow::enable);
	connect(
		ui->sectionListWidget, &QListWidget::currentItemChanged,
		this, &MainWindow::enable);

	connect(
		&dispatcher, &MrwBusService::connected,
		&statechart, &OperatingMode::can_connected,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingMode::inquire,
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		Qt::QueuedConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::completed,
		&statechart, &OperatingMode::inquired,
		Qt::QueuedConnection);

	connect(
		ui->actionOperate, &QAction::triggered,
		&statechart, &OperatingMode::operate,
		Qt::QueuedConnection);
	connect(
		ui->actionEdit, &QAction::triggered,
		&statechart, &OperatingMode::edit,
		Qt::QueuedConnection);
	connect(
		ui->actionClear, &QAction::triggered,
		&statechart, &OperatingMode::clear,
		Qt::QueuedConnection);

	connect(
		&statechart, &OperatingMode::operating,
		this, &MainWindow::onOperate,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingMode::editing,
		this, &MainWindow::onEdit,
		Qt::QueuedConnection);

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.can()->setOperationCallback(&dispatcher);
	statechart.enter();
}

MainWindow::~MainWindow()
{
	statechart.exit();

	on_clearAllSections_clicked();
	on_clearAllRoutes_clicked();

	delete ui;
}

void MainWindow::initRegion()
{
	ModelRailway * model = repo;

	for (size_t r = 0; r < model->regionCount(); r++)
	{
		Region   *   region = model->region(r);
		RegionForm * form = new RegionForm(region);

		ui->regionTabWidget->addTab(form, region->name());
	}
}

void MainWindow::enable()
{
	const bool operating      = statechart.isStateActive(OperatingMode::State::main_region_Operating);
	const bool editing        = statechart.isStateActive(OperatingMode::State::main_region_Editing);
	const bool fail           = statechart.isStateActive(OperatingMode::State::main_region_Fail);
	const size_t switch_count = count<RegularSwitchController>() + count<DoubleCrossSwitchController>();
	const size_t rail_count   = count<RailController>();

	ui->actionEdit->setEnabled(!editing);
	ui->actionUp->setEnabled(editing);
	ui->actionDown->setEnabled(editing);
	ui->actionLeft->setEnabled(editing);
	ui->actionRight->setEnabled(editing);
	ui->actionInclination->setEnabled(editing);
	ui->actionExpand->setEnabled(editing);
	ui->actionExtend->setEnabled(editing);
	ui->actionReduce->setEnabled(editing);
	ui->actionBendLeft->setEnabled(editing);
	ui->actionBendRight->setEnabled(editing);
	ui->actionLineUp->setEnabled(editing);
	ui->actionLineDown->setEnabled(editing);

	ui->actionOperate->setEnabled(!operating);
	ui->actionClear->setEnabled(fail);

	ui->actionTurnSwitchLeft->setEnabled(switch_count > 0);
	ui->actionTurnSwitch->setEnabled(switch_count > 0);
	ui->actionTurnSwitchRight->setEnabled(switch_count > 0);

	ui->clearRoute->setEnabled(ui->routeListWidget->currentItem() != nullptr);
	ui->clearAllRoutes->setEnabled(ui->routeListWidget->count() > 0);
	ui->clearSection->setEnabled(ui->sectionListWidget->currentItem() != nullptr);
	ui->clearAllSections->setEnabled(ui->sectionListWidget->count() > 0);

	ui->tourLeftPushButton->setEnabled(
		operating &&
		(rail_count >= 2) && (switch_count == 0));
	ui->shuntLeftPushButton->setEnabled(
		operating &&
		isSameRegion() &&
		(rail_count >= 2) && (switch_count == 0));
	ui->extendPushButton->setEnabled(
		operating &&
		(ui->routeListWidget->currentItem() != nullptr) &&
		(rail_count > 0) && (switch_count == 0));
	ui->shuntRightPushButton->setEnabled(
		operating &&
		isSameRegion() &&
		(rail_count >= 2) && (switch_count == 0));
	ui->tourRightPushButton->setEnabled(
		operating &&
		(rail_count >= 2) && (switch_count == 0));
}

void MainWindow::expandBorder(RegionForm * form, BaseController * controller, Position * position)
{
	std::vector<Position *> positions;

	form->line(positions, position->point().y());
	std::sort(positions.begin(), positions.end(), &Position::less);

	for (unsigned i = 0; i < positions.size(); i++)
	{
		if (positions[i] == position)
		{
			if (i > 0)
			{
				const Position * prev = positions[i - 1];
				const int        x    = prev->point().x() + prev->width();

				position->setX(x);
			}
			if (i < positions.size())
			{
				const Position * next = positions[i + 1];
				const int        inc  = next->point().x() - position->point().x() - Position::FRACTION;
				const int        diff = inc - position->extension();

				position->extend(diff);
			}
			else
			{
				position->extend(20);
			}
			controller->reposition();
			return;
		}
	}
}

RailPart * MainWindow::rail(const int idx) const
{
	QListWidgetItem * item       = ui->sectionListWidget->item(idx);
	BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
	RailPartInfo   *  info       = dynamic_cast<RailPartInfo *>(controller);

	return info != nullptr ? info->railPart() : nullptr;
}

void MainWindow::traverse(std::function<void(BaseController *, Position *)> editor)
{
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(ControllerWidget::USER_ROLE).value<BaseController *>();
		Position     *    position   = controller->position();

		if (position != nullptr)
		{
			editor(controller, position);
		}
	}
}

bool MainWindow::isSameRegion()
{
	std::unordered_set<Region *> regions;

	traverse([&](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		RailPartInfo * info = dynamic_cast<RailPartInfo *>(controller);

		if (info != nullptr)
		{
			regions.emplace(info->region());
		}
	});

	return regions.size() == 1;
}

void MainWindow::itemClicked(QListWidgetItem * item)
{
	if (item->listWidget() == nullptr)
	{
		ui->sectionListWidget->addItem(item);
	}
	else
	{
		int row = ui->sectionListWidget->row(item);

		ui->sectionListWidget->takeItem(row);
	}
	enable();
}

void MainWindow::on_clearSection_clicked()
{
	for (QListWidgetItem * item : ui->sectionListWidget->selectedItems())
	{
		int row = ui->sectionListWidget->row(item);

		ui->sectionListWidget->takeItem(row);
	}
	enable();
}

void MainWindow::on_clearAllSections_clicked()
{
	while (ui->sectionListWidget->count() > 0)
	{
		ui->sectionListWidget->takeItem(0);
	}
	enable();
}

void MainWindow::on_clearRoute_clicked()
{
	for (QListWidgetItem * item : ui->routeListWidget->selectedItems())
	{
		const int row = ui->routeListWidget->row(item);

		ui->routeListWidget->takeItem(row);

		Route * route = item->data(Route::USER_ROLE).value<Route *>();

		delete route;
	}
	enable();
}

void MainWindow::on_clearAllRoutes_clicked()
{
	while (ui->routeListWidget->count() > 0)
	{
		QListWidgetItem * item  = ui->routeListWidget->takeItem(0);
		Route      *      route = item->data(Route::USER_ROLE).value<Route *>();

		delete route;
	}
	enable();
}

void MainWindow::on_tourLeftPushButton_clicked()
{
	create(false, SectionState::TOUR);
}

void MainWindow::on_shuntLeftPushButton_clicked()
{
	create(false, SectionState::SHUNTING);
}

void MainWindow::on_extendPushButton_clicked()
{

}

void MainWindow::on_shuntRightPushButton_clicked()
{
	create(true, SectionState::SHUNTING);
}

void MainWindow::on_tourRightPushButton_clicked()
{
	create(true, SectionState::TOUR);
}

void MainWindow::move(int right, int down)
{
	traverse([right, down](BaseController * controller, Position * position)
	{
		position->move(right, down);

		emit controller->reposition();
	});
}

void MainWindow::extend(int inc)
{
	traverse([inc](BaseController * controller, Position * position)
	{
		position->extend(inc);

		emit controller->reposition();
	});
}

void MainWindow::lineup(int inc)
{
	traverse([inc](BaseController * controller, Position * position)
	{
		position->lineup(inc);

		emit controller->reposition();
	});
}

void MainWindow::on_actionExpand_triggered()
{
	if (ui->sectionListWidget->count() == 1)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(0);
		BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
		Position     *    position   = controller->position();

		Q_ASSERT(controller != nullptr);
		if ((position != nullptr) && controller->isExpandable())
		{
			RegionForm * form = dynamic_cast<RegionForm *>(ui->regionTabWidget->currentWidget());

			expandBorder(form, controller, position);
		}
	}
}

void MainWindow::on_actionInclination_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		position->toggleInclination();
		controller->update();
	});
}

void MainWindow::bend(const Position::Bending bend)
{
	traverse([bend](BaseController * controller, Position * position)
	{
		if ((position->bending() != bend) ||
			(dynamic_cast<RegularSwitchController *>(controller) != nullptr))
		{
			position->setBending(bend);
		}
		else
		{
			position->setBending(Bending::STRAIGHT);
		}
		emit controller->update();
	});
}

void MainWindow::on_actionTurnSwitchLeft_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		RegularSwitchControllerProxy * rs_ctrl =
			dynamic_cast<RegularSwitchControllerProxy *>(controller);
		DoubleCrossSwitchControllerProxy * dcs_ctrl =
			dynamic_cast<DoubleCrossSwitchControllerProxy *>(controller);

		if (rs_ctrl != nullptr)
		{
			rs_ctrl->turnLeft();
		}
		if (dcs_ctrl != nullptr)
		{
			dcs_ctrl->turnLeft();
		}
	});
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitch_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		RegularSwitchControllerProxy * rs_ctrl =
			dynamic_cast<RegularSwitchControllerProxy *>(controller);
		DoubleCrossSwitchControllerProxy * dcs_ctrl =
			dynamic_cast<DoubleCrossSwitchControllerProxy *>(controller);

		if (rs_ctrl != nullptr)
		{
			rs_ctrl->turn();
		}
		if (dcs_ctrl != nullptr)
		{
			dcs_ctrl->turn();
		}
	});
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitchRight_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		RegularSwitchControllerProxy * rs_ctrl =
			dynamic_cast<RegularSwitchControllerProxy *>(controller);
		DoubleCrossSwitchControllerProxy * dcs_ctrl =
			dynamic_cast<DoubleCrossSwitchControllerProxy *>(controller);

		if (rs_ctrl != nullptr)
		{
			rs_ctrl->turnRight();
		}
		if (dcs_ctrl != nullptr)
		{
			dcs_ctrl->turnRight();
		}
	});
	on_clearAllSections_clicked();
}

void MainWindow::onOperate(const bool active)
{
	Q_UNUSED(active);
	__METHOD__;

	enable();
	on_clearAllSections_clicked();
}

void MainWindow::onEdit(const bool active)
{
	__METHOD__;

	BaseWidget::setVerbose(active);
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::reset()
{
	__METHOD__;

	ControllerRegistry::instance().reset();
}

Route * MainWindow::create(const bool direction, SectionState state)
{
	RailPart * first = rail(0);
	Route   *  route = new Route(direction, state, first);

	for (int i = 1; i < ui->sectionListWidget->count(); i++)
	{
		if (!route->extend(rail(i)))
		{
			return nullptr;
		}
	}
	route->prepare();
	ui->routeListWidget->addItem(*route);
	ui->regionTabWidget->currentWidget()->update();
	on_clearAllSections_clicked();

	return route;
}
