//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/modelrailway.h>
#include <ui/controllerwidget.h>
#include <ctrl/basecontroller.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regionform.h"

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

MainWindow::MainWindow(ModelRepository & repository, QWidget * parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow), repo(repository)
{
	ui->setupUi(this);
	initRegion();

	connect(ui->clearSection, &QPushButton::clicked, this, &MainWindow::clearSelectedItems);
	connect(ui->clearAllSections, &QPushButton::clicked, this, &MainWindow::clearAllItems);

	QList<ControllerWidget *> widgets = findChildren<ControllerWidget *>();

	for (ControllerWidget * w : widgets)
	{
		connect(w, &ControllerWidget::clicked, this, &MainWindow::itemClicked);
	}

	connect(ui->actionQuit,        &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
	connect(ui->actionInclination, &QAction::triggered, this, &MainWindow::incline);
	connect(ui->actionExpand,      &QAction::triggered, this, &MainWindow::expand);
	connect(ui->actionBendLeft,    &QAction::triggered, this, &MainWindow::bendLeft);
	connect(ui->actionBendRight,   &QAction::triggered, this, &MainWindow::bendRight);

	connect(ui->actionRight, &QAction::triggered, [this] ()
	{
		move(1, 0);
	});
	connect(ui->actionLeft, &QAction::triggered, [this] ()
	{
		move(-1, 0);
	});
	connect(ui->actionUp, &QAction::triggered, [this] ()
	{
		move(0, -Position::FRACTION);
	});
	connect(ui->actionDown, &QAction::triggered, [this] ()
	{
		move(0, Position::FRACTION);
	});

	connect(ui->actionExtend, &QAction::triggered, [this] ()
	{
		extend(1);
	});
	connect(ui->actionReduce, &QAction::triggered, [this] ()
	{
		extend(-1);
	});
}

MainWindow::~MainWindow()
{
	clearAllItems();
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
	std::sort(positions.begin(), positions.end(), &Position::compare);

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

void MainWindow::clearSelectedItems()
{
	for (QListWidgetItem * item : ui->sectionListWidget->selectedItems())
	{
		int row = ui->sectionListWidget->row(item);

		ui->sectionListWidget->takeItem(row);
	}
}

void MainWindow::clearAllItems()
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

void MainWindow::expand()
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

void MainWindow::incline()
{
	edit([](BaseController * controller, Position * position)
	{
		position->toggleInclination();
		controller->update();
	});
}

void MainWindow::bendLeft()
{
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
		Position     *    position = controller->position();

		if (position != nullptr)
		{
			if (position->bending() != Bending::LEFT)
			{
				position->setBending(Bending::LEFT);
			}
			else
			{
				position->setBending(Bending::STRAIGHT);
			}
			emit controller->update();
		}
	}
}

void MainWindow::bendRight()
{
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
		Position     *    position = controller->position();

		if (position != nullptr)
		{
			if (position->bending() != Bending::RIGHT)
			{
				position->setBending(Bending::RIGHT);
			}
			else
			{
				position->setBending(Bending::STRAIGHT);
			}
			emit controller->update();
		}
	}
}
