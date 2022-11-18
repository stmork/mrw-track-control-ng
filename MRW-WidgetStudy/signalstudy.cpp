//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "signalstudy.h"
#include "ui_signalstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;

SignalStudy::SignalStudy(QWidget * parent) :
	QWidget(parent),
	ui(new Ui::SignalStudy)
{
	ui->setupUi(this);

	// set black background
	// Qt::black / "#000000" / "black"
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

	ui->symbolWidget->setAutoFillBackground(true);
	ui->symbolWidget->setPalette(pal);

	ui->bigSwitchWidget->setController(&mock);
	ui->smallSwitchWidget->setController(&mock);

	connect(
		ui->forwardButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setDirection(true);
	});
	connect(
		ui->backwardButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setDirection(false);
	});

	connect(
		ui->availableButton_1, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setShunting);
	connect(
		ui->availableButton_2, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setDistant);
	connect(
		ui->availableButton_3, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setMain);

	connect(
		&mock, &SignalControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &SignalControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));

	ui->backwardButton->setChecked(true);
	ui->freeButton->setChecked(true);

	ui->availableButton_1->setChecked(true);
	ui->availableButton_2->setChecked(false);
	ui->availableButton_3->setChecked(false);
	ui->stopButton_1->setChecked(true);
	ui->stopButton_2->setChecked(true);
	ui->stopButton_3->setChecked(true);
}

SignalStudy::~SignalStudy()
{
	delete ui;
}

void SignalStudy::changeEvent(QEvent * e)
{
	QWidget::changeEvent(e);
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
