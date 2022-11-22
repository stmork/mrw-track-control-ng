//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/modelrailway.h>
#include <ui/controllerwidget.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regionform.h"

using namespace mrw::model;
using namespace mrw::ui;

MainWindow::MainWindow(ModelRepository & repository, QWidget * parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow), repo(repository)
{
	ui->setupUi(this);
	initRegion();

#if 0
	// NOTE: Disabled for the first.
	QList<ControllerWidget *> widgets = findChildren<ControllerWidget *>();

	for (ControllerWidget * w : widgets)
	{
	}
#endif
}

MainWindow::~MainWindow()
{
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

