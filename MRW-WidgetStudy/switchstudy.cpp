//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "switchstudy.h"
#include "ui_switchstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;

SwitchStudy::SwitchStudy(QWidget * parent) :
	QWidget(parent),
	ui(new Ui::SwitchStudy)
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
		ui->leftHandedButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setLeftHanded);
	connect(
		ui->rightHandedButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setRightHanded);
	connect(
		ui->leftButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setLeft);
	connect(
		ui->rightButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setRight);
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
		&mock, &RegularSwitchControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &RegularSwitchControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));
}

SwitchStudy::~SwitchStudy()
{
	delete ui;
}

void SwitchStudy::changeEvent(QEvent * e)
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
