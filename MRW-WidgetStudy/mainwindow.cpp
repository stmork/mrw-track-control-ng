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
	ui->left_forward->setLeft();
	ui->left_forward->setDirection();
	ui->right_forward->setRight();
	ui->right_forward->setDirection();
	ui->left_backward->setLeft();
	ui->left_backward->setDirection(false);
	ui->right_backward->setRight();
	ui->right_backward->setDirection(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

