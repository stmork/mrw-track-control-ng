//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <unistd.h>

#include <QScreen>

#include <util/globalbatch.h>
#include <util/method.h>
#include <util/random.h>
#include <util/termhandler.h>
#include <statecharts/timerservice.h>

#include <model/modelrailway.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/basecontroller.h>
#include <ctrl/railcontroller.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/signalcontrollerproxy.h>
#include <ui/controllerwidget.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regionform.h"
#include "mrwmessagedispatcher.h"
#include "widgetroute.h"
#include "beermodeservice.h"

using namespace mrw::util;
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
	const QScreen * screen = QGuiApplication::primaryScreen();
	const QSize     size   = screen->availableSize();

	BaseWidget::setVerbose(false);

	ui->setupUi(this);
	setWindowTitle("Modelbased railway control (next generation) - " + repository.modelName());
	initRegion(dispatcher);
	BeerModeService::instance().init(repo);

	QList<ControllerWidget *> widgets = findChildren<ControllerWidget *>();

	for (ControllerWidget * w : widgets)
	{
		connect(w, &ControllerWidget::clicked, this, &MainWindow::itemClicked);
	}

	connect(
		ui->routeListWidget, &QListWidget::currentItemChanged,
		this, &MainWindow::enable);
	connect(
		ui->sectionListWidget, &QListWidget::currentItemChanged,
		this, &MainWindow::enable);

	connectEditActions();
	connectOpModes(dispatcher);

	qInfo().noquote() << "Screen size: " << size;

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.can()->setOperationCallback(&dispatcher);

	Q_ASSERT(statechart.check());
	statechart.enter();

	static SignalHandler  terminator( { SIGTERM, SIGINT }, [&]()
	{
		statechart.finalize();
	});
}

MainWindow::~MainWindow()
{
	__METHOD__;

	qInfo("  Quitting main window.");
	Q_ASSERT(!MainWindow::hasActiveRoutes());
	Q_ASSERT(GlobalBatch::instance().isCompleted());

	statechart.exit();

	delete ui;
}

void MainWindow::initRegion(MrwMessageDispatcher & dispatcher)
{
	ModelRailway * model = repo;

	for (size_t r = 0; r < model->regionCount(); r++)
	{
		Region   *   region = model->region(r);
		RegionForm * form = new RegionForm(region);

		ui->regionTabWidget->addTab(form, region->name());

		connect(
			&dispatcher, &MrwMessageDispatcher::brightness,
			form,        &RegionForm::brightness);
	}
}

void MainWindow::connectEditActions()
{
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
}

void MainWindow::connectOpModes(MrwMessageDispatcher & dispatcher)
{
	connect(
		&dispatcher, &MrwBusService::connected,
		&statechart, &OperatingModeStatechart::can_connected,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::cleared,
		&ControllerRegistry::instance(), &ControllerRegistry::clear,
		Qt::DirectConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::failed,
		&statechart, &OperatingModeStatechart::failed,
		Qt::QueuedConnection);

	connect(
		&statechart, &OperatingModeStatechart::start,
		&ControllerRegistry::instance(), &ControllerRegistry::start,
		Qt::QueuedConnection);
	connect(
		&GlobalBatch::instance(), &GlobalBatch::completed,
		&statechart, &OperatingModeStatechart::started,
		Qt::QueuedConnection);
	connect(
		&GlobalBatch::instance(), &GlobalBatch::completed,
		&statechart, &OperatingModeStatechart::completed,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::quit,
		QCoreApplication::instance(), &QCoreApplication::quit);

	connect(
		ui->actionOperate, &QAction::triggered,
		&statechart, &OperatingModeStatechart::operate,
		Qt::QueuedConnection);
	connect(
		ui->actionEdit, &QAction::triggered,
		&statechart, &OperatingModeStatechart::edit,
		Qt::QueuedConnection);
	connect(
		ui->actionManual, &QAction::triggered,
		&statechart, &OperatingModeStatechart::manual,
		Qt::QueuedConnection);
	connect(
		ui->actionInit, &QAction::triggered,
		&statechart, &OperatingModeStatechart::init,
		Qt::QueuedConnection);
	connect(
		ui->actionClear, &QAction::triggered,
		&statechart, &OperatingModeStatechart::clear,
		Qt::DirectConnection);
	connect(
		ui->actionQuit,  &QAction::triggered,
		&statechart, &OperatingModeStatechart::finalize);

	connect(
		&statechart, &OperatingModeStatechart::start,
		this, &MainWindow::onInit,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::failing,
		&dispatcher, &MrwMessageDispatcher::emergencyStop,
		Qt::DirectConnection);
	connect(
		&statechart, &OperatingModeStatechart::failing,
		this, &MainWindow::onFailed,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::operating,
		this, &MainWindow::onOperate,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::playing,
		this, &MainWindow::onManual,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::editing,
		this, &MainWindow::onEdit,
		Qt::QueuedConnection);
	connect(
		&statechart, &OperatingModeStatechart::quitting,
		this, &MainWindow::onQuit,
		Qt::QueuedConnection);
}

void MainWindow::enable()
{
	const bool   operating    = statechart.isStateActive(OperatingModeStatechart::State::main_region_Running_operating_Operating);
	const bool   editing      = statechart.isStateActive(OperatingModeStatechart::State::main_region_Running_operating_Editing);
	const bool   failed       = statechart.isStateActive(OperatingModeStatechart::State::main_region_Running_operating_Failed);
	const bool   manual       = statechart.isStateActive(OperatingModeStatechart::State::main_region_Manual);
	const size_t switch_count = count<RegularSwitchController>() + count<DoubleCrossSwitchController>();
	const size_t rail_count   = count<RailController>() +          count<SignalControllerProxy>();

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

	ui->actionOperate->setEnabled(editing);
	ui->actionEdit->setEnabled((operating && !hasActiveRoutes()) || failed);
	ui->actionManual->setEnabled((operating && isManualValid()) || manual);
	ui->actionClear->setEnabled(failed);
	ui->actionInit->setEnabled(operating);

	ui->actionTurnSwitchLeft->setEnabled(switch_count > 0);
	ui->actionTurnSwitch->setEnabled(switch_count > 0);
	ui->actionTurnSwitchRight->setEnabled(switch_count > 0);
	ui->actionBeermodeLeft->setEnabled(operating && !ui->actionBeermodeRight->isChecked());
	ui->actionBeermodeRight->setEnabled(operating &&  !ui->actionBeermodeLeft->isChecked());

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

/*************************************************************************
**                                                                      **
**       Statechart callbacks                                           **
**                                                                      **
*************************************************************************/

void MainWindow::resetTransaction()
{
	__METHOD__;

	GlobalBatch::instance().reset();
}

bool MainWindow::hasActiveRoutes()
{
	return ui->routeListWidget->count() > 0;
}

void MainWindow::disableRoutes()
{
	__METHOD__;

	on_clearAllRoutes_clicked();
}

void MainWindow::activateManual(const bool activate)
{
	std::vector<SectionController *> section_controllers;
	Section             *            selected = manualSection();

	ControllerRegistry::instance().collect<SectionController>(section_controllers);

	for (SectionController * controller : section_controllers)
	{
		Section * section = *controller;

		section->setState(activate ? SectionState::SHUNTING : SectionState::FREE);

		if (activate)
		{
			// enable() may turn relais on and locks Section.
			controller->setAutoOff(false);
			controller->setAutoUnlock(false);
			controller->enable(!section->occupation() || (section == selected));
		}
		else
		{
			// disable() turns relais off but keeps Section locked.
			controller->disable();
			controller->unlock();
		}
	}
}

bool MainWindow::isManualValid()
{
	return manualSection() != nullptr;
}

Section * MainWindow::manualSection() const
{
	std::unordered_set<Section *> sections;

	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(ControllerWidget::USER_ROLE).value<BaseController *>();
		RailPartInfo   *  info       = dynamic_cast<RailPartInfo *>(controller);

		if (info != nullptr)
		{
			Section * section = info->section();

			if (section->occupation())
			{
				sections.insert(section);
			}
		}
	}

	return sections.size() == 1 ? *sections.begin() : nullptr;
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

void MainWindow::traverse(Callback callback)
{
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(ControllerWidget::USER_ROLE).value<BaseController *>();
		Position     *    position   = controller->position();

		if (position != nullptr)
		{
			callback(controller, position);
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

/*************************************************************************
**                                                                      **
**       Routing support                                                **
**                                                                      **
*************************************************************************/

Route * MainWindow::createRoute(const bool direction, const SectionState state)
{
	__METHOD__;

	RailPart   *  first = rail(0);
	WidgetRoute * route = new WidgetRoute(direction, state, first);

	for (int i = 1; i < ui->sectionListWidget->count(); i++)
	{
		if (!route->append(rail(i)))
		{
			delete route;

			return nullptr;
		}
	}

	addRoute(route);
	return route;
}

void MainWindow::addRoute(WidgetRoute * route)
{
	ui->routeListWidget->addItem(*route);
	ui->routeListWidget->setCurrentItem(*route);
	ui->regionTabWidget->currentWidget()->update();
	on_clearAllSections_clicked();

	connect(
		route, &WidgetRoute::finished,
		this,  &MainWindow::routeFinished,
		Qt::QueuedConnection);

	route->turn();
}

void MainWindow::routeFinished()
{
	__METHOD__;

	WidgetRoute   *   route = dynamic_cast<WidgetRoute *>(QObject::sender());
	QListWidgetItem * item  = *route;
	const int         row   = ui->routeListWidget->row(item);

	ui->routeListWidget->takeItem(row);

	delete route;
	if (route == beer_route)
	{
		// Wait at least 1250 ms for track occupation simulator.
		const int wait_time = 1250 + Random::random<int>(500);

		beer_route = nullptr;

		if (ui->actionBeermodeLeft->isChecked())
		{
			QTimer::singleShot(wait_time,
				this, &MainWindow::on_actionBeermodeLeft_triggered);
		}
		if (ui->actionBeermodeRight->isChecked())
		{
			QTimer::singleShot(wait_time,
				this, &MainWindow::on_actionBeermodeRight_triggered);
		}
	}

	statechart.completed();
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

/*************************************************************************
**                                                                      **
**       List widget commands                                           **
**                                                                      **
*************************************************************************/

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

		ui->routeListWidget->item(row);

		WidgetRoute * route = item->data(WidgetRoute::USER_ROLE).value<WidgetRoute *>();

		route->disable();
		if (route == beer_route)
		{
			beer_route = nullptr;
			ui->actionBeermodeLeft->setChecked(false);
			ui->actionBeermodeRight->setChecked(false);
		}
	}
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::on_clearAllRoutes_clicked()
{
	std::vector<WidgetRoute *> routes;

	for (int index = 0; index < ui->routeListWidget->count(); index++)
	{
		QListWidgetItem * item  = ui->routeListWidget->item(index);
		WidgetRoute   *   route = item->data(WidgetRoute::USER_ROLE).value<WidgetRoute *>();

		routes.push_back(route);
	}

	// Disable collected routes.
	for (WidgetRoute * route : routes)
	{
		route->disable();
	}

	beer_route = nullptr;
	ui->actionBeermodeLeft->setChecked(false);
	ui->actionBeermodeRight->setChecked(false);
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

/*************************************************************************
**                                                                      **
**       Routing commands                                               **
**                                                                      **
*************************************************************************/

void MainWindow::on_tourLeftPushButton_clicked()
{
	createRoute(false, SectionState::TOUR);
}

void MainWindow::on_shuntLeftPushButton_clicked()
{
	createRoute(false, SectionState::SHUNTING);
}

void MainWindow::on_extendPushButton_clicked()
{
	__METHOD__;

	for (QListWidgetItem * item : ui->routeListWidget->selectedItems())
	{
		WidgetRoute * route = item->data(WidgetRoute::USER_ROLE).value<WidgetRoute *>();

		for (int i = 0; i < ui->sectionListWidget->count(); i++)
		{
			if (!route->append(rail(i)))
			{
				route->dump();
				return;
			}
		}

		ui->regionTabWidget->currentWidget()->update();
		on_clearAllSections_clicked();

		route->turn();
	}
	enable();
}

void MainWindow::on_shuntRightPushButton_clicked()
{
	createRoute(true, SectionState::SHUNTING);
}

void MainWindow::on_tourRightPushButton_clicked()
{
	createRoute(true, SectionState::TOUR);
}

/*************************************************************************
**                                                                      **
**       Editing commands                                               **
**                                                                      **
*************************************************************************/

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

/*************************************************************************
**                                                                      **
**       Normal operation commands                                      **
**                                                                      **
*************************************************************************/

void MainWindow::on_actionTurnSwitchLeft_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turnLeft);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turnLeft);
	});
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitch_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::change);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::change);
	});
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitchRight_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turnRight);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turnRight);
	});
	on_clearAllSections_clicked();
}

void MainWindow::on_actionLock_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		BaseController::callback<SectionController>(
			controller,                &SectionController::failed);
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::failed);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::failed);
		BaseController::callback<SignalControllerProxy>(
			controller,            &SignalControllerProxy::failed);
	});

	on_clearAllSections_clicked();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::on_actionUnlock_triggered()
{
	traverse([](BaseController * controller, Position * position)
	{
		Q_UNUSED(position);

		BaseController::callback<ControllerRegistrand>(
			controller, &ControllerRegistrand::restart);
	});

	on_clearAllSections_clicked();
	ui->regionTabWidget->currentWidget()->update();
}

/*************************************************************************
**                                                                      **
**       Beer mode actions                                              **
**                                                                      **
*************************************************************************/

void MainWindow::on_actionBeermodeLeft_triggered()
{
	__METHOD__;

	if (ui->actionBeermodeLeft->isChecked())
	{
		startBeermode(false);
	}
}

void MainWindow::on_actionBeermodeRight_triggered()
{
	__METHOD__;

	if (ui->actionBeermodeRight->isChecked())
	{
		startBeermode(true);
	}
}

void MainWindow::startBeermode(const bool dir)
{
	if (beer_route == nullptr)
	{
		beer_route = BeerModeService::instance().startBeerMode(dir);

		if (beer_route != nullptr)
		{
			addRoute(beer_route);
		}
		else
		{
			qInfo("No route possible! Disabling beer mode...");
			ui->actionBeermodeLeft->setChecked(false);
			ui->actionBeermodeRight->setChecked(false);
		}
		enable();
	}
}

/*************************************************************************
**                                                                      **
**       Operation modes                                                **
**                                                                      **
*************************************************************************/

void MainWindow::onInit()
{
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, "I");
	enable();
	on_clearAllSections_clicked();
}

void MainWindow::onOperate(const bool active)
{
	Q_UNUSED(active);
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "O" : "");
	enable();
	on_clearAllSections_clicked();
}

void MainWindow::onEdit(const bool active)
{
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "E" : "");
	BaseWidget::setVerbose(active);
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::onManual(const bool active)
{
	std::vector<SwitchController *>  switch_controllers;

	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "M" : "");
	ControllerRegistry::instance().collect<SwitchController>(switch_controllers);
	for (SwitchController * controller : switch_controllers)
	{
		controller->setManual(active);
	}
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::onQuit(const bool active)
{
	Q_UNUSED(active);
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "Q" : "", BaseWidget::WHITE, true);
	BaseWidget::setVerbose(false);
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::onFailed()
{
	__METHOD__;

	GlobalBatch::instance().dump();
	RegionForm::setOpMode(ui->regionTabWidget, "F", BaseWidget::RED, true);
	on_clearAllRoutes_clicked();
	enable();
}

/*************************************************************************
**                                                                      **
**       Tab control for regions                                        **
**                                                                      **
*************************************************************************/

void MainWindow::on_actionTabLeft_triggered()
{
	changePage(-1);
}

void MainWindow::on_actionTabRight_triggered()
{
	changePage(1);
}

void MainWindow::changePage(const int offset)
{
	const int index = ui->regionTabWidget->currentIndex() + offset;
	const int count = ui->regionTabWidget->count();

	ui->regionTabWidget->setCurrentIndex((index + count) % count);
}
