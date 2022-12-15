//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_actionSaveWidget_triggered()
{
	WidgetSaver * widget = dynamic_cast<WidgetSaver *>(ui->studyWidgets->currentWidget());

	Q_ASSERT(widget != nullptr);
	widget->save();
}

void MainWindow::on_actionLeft_triggered()
{
	changePage(-1);
}

void MainWindow::on_actionRight_triggered()
{
	changePage(1);
}

void MainWindow::changePage(const int offset)
{
	const int index = ui->studyWidgets->currentIndex() + offset;
	const int count = ui->studyWidgets->count();

	ui->studyWidgets->setCurrentIndex((index + count) % count);
}
