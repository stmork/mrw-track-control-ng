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

	connect(ui->actionBeenden,     &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
	connect(ui->actionNeigung,     &QAction::triggered, this, &MainWindow::incline);
	connect(ui->actionErweitern,   &QAction::triggered, this, &MainWindow::expand);
	connect(ui->actionTeilschritt, &QAction::triggered, this, &MainWindow::extend);

	connect(ui->actionRechts, &QAction::triggered, [this] ()
	{
		move(1, 0);
	});
	connect(ui->actionLinks, &QAction::triggered, [this] ()
	{
		move(-1, 0);
	});
	connect(ui->actionHoch, &QAction::triggered, [this] ()
	{
		move(0, -1);
	});
	connect(ui->actionRunter, &QAction::triggered, [this] ()
	{
		move(0, 1);
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
	for (int i = 0; i < ui->sectionListWidget->count(); i++)
	{
		QListWidgetItem * item       = ui->sectionListWidget->item(i);
		BaseController  * controller = item->data(Qt::UserRole).value<BaseController *>();
		Position * position = controller->position();

		if (position != nullptr)
		{
			position->move(right, down);
			controller->refresh();
		}
	}
}

void MainWindow::extend()
{

}

void MainWindow::expand()
{

}

void MainWindow::incline()
{

}
