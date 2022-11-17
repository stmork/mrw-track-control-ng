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
	QPalette pal = QPalette();

	// set black background
	// Qt::black / "#000000" / "black"
	pal.setColor(QPalette::Window, Qt::black);

	centralWidget()->setAutoFillBackground(true);
	centralWidget()->setPalette(pal);
	centralWidget()->show();

	ui->left_forward_top->setLeftHanded();
	ui->left_forward_top->setLeft();
	ui->left_forward_top->setDirection(false);
	ui->right_forward_top->setRightHanded();
	ui->right_forward_top->setRight();
	ui->right_forward_top->setDirection(false);
	ui->left_backward_top->setLeftHanded();
	ui->left_backward_top->setRight();
	ui->left_backward_top->setDirection(false);
	ui->right_backward_top->setRightHanded();
	ui->right_backward_top->setLeft();
	ui->right_backward_top->setDirection(false);

	ui->left_forward_bottom->setRightHanded();
	ui->left_forward_bottom->setRight();
	ui->left_forward_bottom->setDirection(true);
	ui->right_forward_bottom->setLeftHanded();
	ui->right_forward_bottom->setLeft();
	ui->right_forward_bottom->setDirection(true);
	ui->left_backward_bottom->setRightHanded();
	ui->left_backward_bottom->setLeft();
	ui->left_backward_bottom->setDirection(true);
	ui->right_backward_bottom->setLeftHanded();
	ui->right_backward_bottom->setRight();
	ui->right_backward_bottom->setDirection(true);

	mock_ss1.setShunting();
	mock_ss2.setDistant();
	mock_ss3.setShunting();
	mock_ss3.setMain(false);
	ui->signalWidget1->setController(&mock_ss1);
	ui->signalWidget2->setController(&mock_ss2);
	ui->signalWidget3->setController(&mock_ss3);

	ui->signal_backward_paired_shunt->setDirection(false);
	ui->signal_backward_paired_shunt->setDistant(true);
	ui->signal_backward_paired_shunt->setShunting(true);
	ui->signal_backward_paired->setDirection(false);
	ui->signal_backward_paired->setDistant(true);
	ui->signal_backward_single->setDirection(false);
	ui->signal_forward_single->setDirection(true);
	ui->signal_forward_paired->setDirection(true);
	ui->signal_forward_paired->setDistant(true);
	ui->signal_forward_paired_shunt->setDirection(true);
	ui->signal_forward_paired_shunt->setDistant(true);
	ui->signal_forward_paired_shunt->setShunting(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

