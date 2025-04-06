//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <fstream>
#include <chrono>

#include <unistd.h>
#include <systemd/sd-daemon.h>

#include <QScreen>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QKeyEvent>
#include <QHostInfo>

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
#include "controlledroute.h"
#include "beermodeservice.h"
#include "log.h"

using namespace std::chrono;
using namespace std::chrono_literals;

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
	repo(repository)
{
	const QScreen * screen = blanker;

	if (screen == nullptr)
	{
		throw std::runtime_error("No primary screen available!");
	}

	const QSize     size   = screen->availableSize();
	qCInfo(mrw::tools::log).noquote() << "Screen size:" << size << "depth:" << screen->depth();

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
	connect(
		&BeerModeService::instance(), &BeerModeService::disabledBeerMode,
		this, &MainWindow::disableBeerMode);

	connectEditActions();
	connectOpModes(dispatcher);

	Qt::WindowFlags window_flags = windowFlags();
	window_flags |= Qt::WindowMinimizeButtonHint;
	window_flags |= Qt::WindowMaximizeButtonHint;
	window_flags |= Qt::WindowFullscreenButtonHint;
	window_flags |= Qt::WindowSystemMenuHint;
	setWindowFlags(window_flags);

	setMouseTracking(true);
	QApplication::instance()->installEventFilter(this);

	status_label = new QLabel(tr("Start."));
	ui->statusbar->addPermanentWidget(status_label);

	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);
	statechart.screen().setOperationCallback(blanker);
	statechart.can().setOperationCallback(dispatcher);
	setScreenBlankTimeout();

	Q_ASSERT(statechart.check());

	static SignalHandler terminator( { SIGTERM, SIGINT }, [&]()
	{
		statechart.finalize();
	});

	// And startup!
	qCDebug(mrw::tools::log, "Starting up...");
	statechart.enter();
}

MainWindow::~MainWindow()
{
	__METHOD__;

	qCInfo(mrw::tools::log, "  Quitting main window.");
	Q_ASSERT(!MainWindow::hasActiveRoutes());
	Q_ASSERT(GlobalBatch::instance().isCompleted());

	statechart.exit();

	delete ui;
}

bool MainWindow::eventFilter(QObject * object, QEvent * event)
{
	if ((dynamic_cast<QMouseEvent *>(event) != nullptr) ||
		(dynamic_cast<QKeyEvent *>(event)   != nullptr) ||
		(dynamic_cast<QTouchEvent *>(event) != nullptr))
	{
#if 0
		qCDebug(mrw::tools::log) << "Filter:" << object << this << event;
#endif

		statechart.screen_userInput();
	}
	return QMainWindow::eventFilter(object, event);
}

void MainWindow::disableBeerMode()
{
	ui->actionBeermodeLeft->setChecked(false);
	ui->actionBeermodeRight->setChecked(false);
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
	const size_t switch_count =
		ui->sectionListWidget->typedCount<RegularSwitchController>() +
		ui->sectionListWidget->typedCount<DoubleCrossSwitchController>();
	const size_t rail_count   =
		ui->sectionListWidget->typedCount<RailController>() +
		ui->sectionListWidget->typedCount<SignalControllerProxy>();

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
		ui->sectionListWidget->isSameRegion() &&
		(rail_count >= 2) && (switch_count == 0));
	ui->extendPushButton->setEnabled(
		operating &&
		(ui->routeListWidget->currentItem() != nullptr) &&
		(rail_count > 0) && (switch_count == 0));
	ui->shuntRightPushButton->setEnabled(
		operating &&
		ui->sectionListWidget->isSameRegion() &&
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

/*************************************************************************
**                                                                      **
**       Manual mode methods                                            **
**                                                                      **
*************************************************************************/

void MainWindow::activateManual(const bool activate)
{
	std::vector<SectionController *> section_controllers;
	Section             *            selected = manualSection();

	ControllerRegistry::instance().collect<SectionController>(section_controllers);

	for (SectionController * controller : section_controllers)
	{
		Section * section = controller->section();

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

Section * MainWindow::manualSection()
{
	std::unordered_set<Section *> sections;

	ui->sectionListWidget->traverse([&](RailPartInfo * info)
	{
		Section * section = info->section();

		if (section->occupation())
		{
			sections.insert(section);
		}
	});

	return sections.size() == 1 ? *sections.begin() : nullptr;
}

void MainWindow::warn(const QString & message)
{
	qCWarning(mrw::tools::log()).noquote() << message;
	ui->statusbar->showMessage(message, 10000);
}

/*************************************************************************
**                                                                      **
**       Routing support                                                **
**                                                                      **
*************************************************************************/

Route * MainWindow::createRoute(const bool direction, const SectionState state)
{
	__METHOD__;

	std::vector<RailPartInfo *> infos;

	ui->sectionListWidget->collect(infos);

	ControlledRoute * route = new ControlledRoute(direction, state, infos[0]->railPart());

	for (size_t i = 1; i < infos.size(); i++)
	{
		RailPart * part = infos[i]->railPart();

		if (!route->append(part))
		{
			QListWidgetItem * route_item = *route;
			QString           message    =
				tr("Fahrstraße %1 kann nicht bis %2 angelegt werden!").arg(route_item->text()).arg(part->partName());

			warn(message);

			delete route;

			return nullptr;
		}
	}

	addRoute(route);
	return route;
}

void MainWindow::extendRoute(ControlledRoute * route)
{
	RailPartInfoCallback callback = [route, this](RailPartInfo * part_info)
	{
		RailPart * part = part_info->railPart();

		if (!route->append(part))
		{
			QListWidgetItem * route_item = *route;
			QString           message    =
				tr("Fahrstraße %1 kann nicht bis %2 erweitert werden!").arg(route_item->text()).arg(part->partName());

			warn(message);
			route->dump();
			return;
		}
	};

	ui->sectionListWidget->traverse(callback);
	ui->regionTabWidget->currentWidget()->update();
	on_clearAllSections_clicked();

	route->turn();
	statechart.routesChanged();
}

void MainWindow::addRoute(ControlledRoute * route)
{
	ui->routeListWidget->addItem(*route);
	ui->routeListWidget->setCurrentItem(*route);
	ui->regionTabWidget->currentWidget()->update();
	on_clearAllSections_clicked();

	connect(
		route, &ControlledRoute::finished,
		this,  &MainWindow::routeFinished,
		Qt::QueuedConnection);

	route->turn();
	statechart.routesChanged();
}

void MainWindow::routeFinished()
{
	__METHOD__;

	ControlledRoute * route = dynamic_cast<ControlledRoute *>(QObject::sender());
	QListWidgetItem * item  = *route;
	const int         row   = ui->routeListWidget->row(item);

	ui->routeListWidget->takeItem(row);

	delete route;
	if (route == BeerModeService::instance())
	{
		// Wait at least 1250 ms for track occupation simulator.
		const int wait_time = 1250 + Random::random<int>(500);

		BeerModeService::instance().clearBeerRoute();

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
	statechart.routesChanged();
}

void MainWindow::setScreenBlankTimeout()
{
	static constexpr seconds   min_blank_time = 5min;

	std::fstream               blank_file("/sys/module/kernel/parameters/consoleblank", std::ios_base::in);
	sc::integer                blank_time = min_blank_time.count();
	mrw::util::Settings        settings;
	mrw::util::SettingsGroup   group(&settings, QHostInfo::localHostName());

	blank_file >> blank_time;

	const sc::integer          clamped = std::max(blank_time, (int)min_blank_time.count());
	const sc::integer          timeout = std::max(settings.value("blank", clamped).toInt(), (int)min_blank_time.count());

	qCInfo(mrw::tools::log).noquote() << "Setting screen blank timeout to" << timeout << "seconds.";
	statechart.screen().setTimeout(timeout);
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
	ControlledRoute * route = ui->routeListWidget->selected();

	if (route != nullptr)
	{
		route->disable();
		if (route == BeerModeService::instance())
		{
			disableBeerMode();
		}
	}
	enable();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::on_clearAllRoutes_clicked()
{
	std::vector<ControlledRoute *> routes;

	// Disable collected routes.
	ui->routeListWidget->collect(routes);
	for (ControlledRoute * route : routes)
	{
		route->disable();
	}

	disableBeerMode();
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

	ControlledRoute * route = ui->routeListWidget->selected();

	if (route != nullptr)
	{
		extendRoute(route);
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
	PositionCallback callback = [right, down](BaseController * controller, Position * position)
	{
		position->move(right, down);

		emit controller->reposition();
	};

	ui->sectionListWidget->traverse(callback);
}

void MainWindow::extend(int inc)
{
	PositionCallback callback = [inc](BaseController * controller, Position * position)
	{
		position->extend(inc);

		emit controller->reposition();
	};

	ui->sectionListWidget->traverse(callback);
}

void MainWindow::lineup(int inc)
{
	PositionCallback callback = [inc](BaseController * controller, Position * position)
	{
		position->lineup(inc);

		emit controller->reposition();
	};

	ui->sectionListWidget->traverse(callback);
}

void MainWindow::on_actionExpand_triggered()
{
	if (ui->sectionListWidget->count() == 1)
	{
		BaseController  * controller = ui->sectionListWidget->controller(0);
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
	PositionCallback callback = [](BaseController * controller, Position * position)
	{
		position->toggleInclination();
		controller->update();
	};

	ui->sectionListWidget->traverse(callback);
}

void MainWindow::bend(const Position::Bending bend)
{
	PositionCallback callback = [bend](BaseController * controller, Position * position)
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
	};

	ui->sectionListWidget->traverse(callback);
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

void MainWindow::keepAlive()
{
	sd_notify (0, "WATCHDOG=1");
}

/*************************************************************************
**                                                                      **
**       Normal operation commands                                      **
**                                                                      **
*************************************************************************/

void MainWindow::on_actionTurnSwitchLeft_triggered()
{
	ControllerCallback callback = [](BaseController * controller)
	{
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turnLeft);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turnLeft);
	};

	ui->sectionListWidget->traverse(callback);
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitch_triggered()
{
	ControllerCallback callback = [](BaseController * controller)
	{
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::change);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::change);
	};

	ui->sectionListWidget->traverse(callback);
	on_clearAllSections_clicked();
}

void MainWindow::on_actionTurnSwitchRight_triggered()
{
	ControllerCallback callback = [](BaseController * controller)
	{
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turnRight);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turnRight);
	};

	ui->sectionListWidget->traverse(callback);
	on_clearAllSections_clicked();
}

void MainWindow::on_actionLock_triggered()
{
	ControllerCallback callback = [](BaseController * controller)
	{
		BaseController::callback<SectionController>(
			controller,                &SectionController::failed);
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::failed);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::failed);
		BaseController::callback<SignalControllerProxy>(
			controller,            &SignalControllerProxy::failed);
	};

	ui->sectionListWidget->traverse(callback);
	on_clearAllSections_clicked();
	ui->regionTabWidget->currentWidget()->update();
}

void MainWindow::on_actionUnlock_triggered()
{
	ControllerCallback callback = [](BaseController * controller)
	{
		BaseController::callback<ControllerRegistrand>(
			controller, &ControllerRegistrand::restart);
	};

	ui->sectionListWidget->traverse(callback);
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
	ControlledRoute * beer_route = BeerModeService::instance();

	if (beer_route == nullptr)
	{
		beer_route = BeerModeService::instance().startBeerMode(dir);

		if (beer_route != nullptr)
		{
			addRoute(beer_route);
		}
		else
		{
			warn(tr("Diese Fahrstraße ist nicht möglich. Der Biermodus wurde abgeschaltet."));
			disableBeerMode();
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
	status_label->setText(tr("Initialisierung"));
}

void MainWindow::onOperate(const bool active)
{
	Q_UNUSED(active);
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "O" : "");
	enable();
	on_clearAllSections_clicked();
	status_label->setText(tr("Betriebsbereit."));
}

void MainWindow::onEdit(const bool active)
{
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "E" : "");
	BaseWidget::setVerbose(active);
	enable();
	ui->regionTabWidget->currentWidget()->update();
	status_label->setText(tr("Bearbeiten des Spurplans"));
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
	status_label->setText(tr("Manueller Betriebsmodus"));
}

void MainWindow::onQuit(const bool active)
{
	Q_UNUSED(active);
	__METHOD__;

	RegionForm::setOpMode(ui->regionTabWidget, active ? "Q" : "", BaseWidget::WHITE, true);
	BaseWidget::setVerbose(false);
	enable();
	ui->regionTabWidget->currentWidget()->update();
	status_label->setText(tr("Herunterfahren..."));
}

void MainWindow::onFailed()
{
	__METHOD__;

	GlobalBatch::instance().dump();
	RegionForm::setOpMode(ui->regionTabWidget, "F", BaseWidget::RED, true);
	on_clearAllRoutes_clicked();
	enable();
	status_label->setText(tr("Fehlermodus"));
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
