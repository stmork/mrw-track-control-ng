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

	setupSignals();
	setupRegularSwitches();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setupSignals()
{
	mock_ss1.setShunting();
	mock_ss2.setDistant();
	mock_ss3.setShunting();
	mock_ss3.setMain(false);
	ui->signalWidget1->setController(&mock_ss1);
	ui->signalWidget2->setController(&mock_ss2);
	ui->signalWidget3->setController(&mock_ss3);

	mock_sb1.setDirection(false);
	mock_sb1.setDistant(true);
	mock_sb1.setShunting(true);
	mock_sb2.setDirection(false);
	mock_sb2.setDistant(true);
	mock_sb3.setDirection(false);
	mock_sb4.setDirection(true);
	mock_sb5.setDirection(true);
	mock_sb5.setDistant(true);
	mock_sb6.setDirection(true);
	mock_sb6.setDistant(true);
	mock_sb6.setShunting(true);

	ui->signal_backward_paired_shunt->setController(&mock_sb1);
	ui->signal_backward_paired->setController(&mock_sb2);
	ui->signal_backward_single->setController(&mock_sb3);
	ui->signal_forward_single->setController(&mock_sb4);
	ui->signal_forward_paired->setController(&mock_sb5);
	ui->signal_forward_paired_shunt->setController(&mock_sb6);
}

void MainWindow::setupRegularSwitches()
{
	mock_top_lf.setLeftHanded();
	mock_top_lf.setLeft();
	mock_top_lf.setDirection(false);
	mock_top_rf.setRightHanded();
	mock_top_rf.setRight();
	mock_top_rf.setDirection(false);
	mock_top_lb.setLeftHanded();
	mock_top_lb.setRight();
	mock_top_lb.setDirection(false);
	mock_top_rb.setRightHanded();
	mock_top_rb.setLeft();
	mock_top_rb.setDirection(false);

	mock_bottom_lf.setRightHanded();
	mock_bottom_lf.setRight();
	mock_bottom_lf.setDirection(true);
	mock_bottom_rf.setLeftHanded();
	mock_bottom_rf.setLeft();
	mock_bottom_rf.setDirection(true);
	mock_bottom_lb.setRightHanded();
	mock_bottom_lb.setLeft();
	mock_bottom_lb.setDirection(true);
	mock_bottom_rb.setLeftHanded();
	mock_bottom_rb.setRight();
	mock_bottom_rb.setDirection(true);

	ui->left_forward_top->setController(&mock_top_lf);
	ui->right_forward_top->setController(&mock_top_rf);
	ui->left_backward_top->setController(&mock_top_lb);
	ui->right_backward_top->setController(&mock_top_rb);
	ui->left_forward_bottom->setController(&mock_bottom_lf);
	ui->right_forward_bottom->setController(&mock_bottom_rf);
	ui->left_backward_bottom->setController(&mock_bottom_lb);
	ui->right_backward_bottom->setController(&mock_bottom_rb);

	ui->switchWidget1->setController(&mock_rss1);
	ui->switchWidget2->setController(&mock_rss2);
	ui->switchWidget3->setController(&mock_rss3);
}

