//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/modelrailway.h>
#include <ui/controllerwidget.h>
#include <ctrl/basecontroller.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regionform.h"

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

MainWindow::MainWindow(ModelRepository & repository, QWidget * parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow), repo(repository)
{
	ui->setupUi(this);
	initRegion();

	QList<ControllerWidget *> widgets = findChildren<ControllerWidget *>();

	for (ControllerWidget * w : widgets)
	{
		connect(w, &ControllerWidget::clicked, this, &MainWindow::itemClicked);
	}

	connect(ui->actionQuit,        &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);

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

MainWindow::~MainWindow()
{
	on_clearAllSections_clicked();
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

void MainWindow::edit(std::function<void(BaseController *, Position *)> editor)
{
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
		Position     *    position   = controller->position();

		if (position != nullptr)
		{
			editor(controller, position);
		}
	}
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
}

void MainWindow::on_clearSection_clicked()
{
	for (QListWidgetItem * item : ui->sectionListWidget->selectedItems())
	{
		int row = ui->sectionListWidget->row(item);

		ui->sectionListWidget->takeItem(row);
	}
}

void MainWindow::on_clearAllSections_clicked()
{
	while (ui->sectionListWidget->count() > 0)
	{
		ui->sectionListWidget->takeItem(0);
	}
}

void MainWindow::move(int right, int down)
{
	edit([right, down](BaseController * controller, Position * position)
	{
		position->move(right, down);

		emit controller->reposition();
	});
}

void MainWindow::extend(int inc)
{
	edit([inc](BaseController * controller, Position * position)
	{
		position->extend(inc);

		emit controller->reposition();
	});
}

void MainWindow::lineup(int inc)
{
	edit([inc](BaseController * controller, Position * position)
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
	edit([](BaseController * controller, Position * position)
	{
		position->toggleInclination();
		controller->update();
	});
}

void MainWindow::bend(const Position::Bending bend)
{
	edit([bend](BaseController * controller, Position * position)
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
	edit([](BaseController * controller, Position * position)
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
	edit([](BaseController * controller, Position * position)
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
	edit([](BaseController * controller, Position * position)
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
