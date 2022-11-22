//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "switchstudy.h"
#include "ui_switchstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;
using namespace mrw::model;

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

	/********************************************************/
	/*  Switch orientation                                  */
	/********************************************************/

	connect(
		ui->leftHandedButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setLeftHanded);
	connect(
		ui->rightHandedButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setRightHanded);
	connect(
		ui->inclinedBox, &QCheckBox::clicked,
		&mock, &RegularSwitchControllerMock::setInclined);

	/********************************************************/
	/*  Switch direction                                    */
	/********************************************************/

	connect(
		ui->leftButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setLeft);
	connect(
		ui->rightButton, &QRadioButton::clicked,
		&mock, &RegularSwitchControllerMock::setRight);

	/********************************************************/
	/*   Counting direction                                 */
	/********************************************************/

	connect(ui->backwardButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setDirection(false);
	});
	connect(ui->forwardButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setDirection(true);
	});

	/********************************************************/
	/*   Lock state                                         */
	/********************************************************/

	connect(ui->openButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::UNLOCKED);
	});
	connect(ui->transitButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::PENDING);
	});
	connect(ui->closedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::LOCKED);
	});

	/********************************************************/
	/*   Section state                                      */
	/********************************************************/

	connect(ui->freeButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::FREE);
	});
	connect(ui->shuntButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::SHUNTING);
	});
	connect(ui->tourButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::TOUR);
	});
	connect(ui->occupiedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::OCCUPIED);
	});
	connect(ui->passedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::PASSED);
	});

	/********************************************************/
	/*   Repaint connection                                 */
	/********************************************************/

	connect(
		&mock, &RegularSwitchControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &RegularSwitchControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));

	ui->leftButton->setChecked(true);
	ui->leftHandedButton->setChecked(true);
	ui->backwardButton->setChecked(true);
	ui->freeButton->setChecked(true);
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
