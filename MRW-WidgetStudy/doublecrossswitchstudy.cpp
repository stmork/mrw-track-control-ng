//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "doublecrossswitchstudy.h"
#include "ui_doublecrossswitchstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;
using namespace mrw::model;

DoubleCrossSwitchStudy::DoubleCrossSwitchStudy(QWidget * parent) :
	QWidget(parent),
	ui(new Ui::DoubleCrossSwitchStudy)
{
	ui->setupUi(this);

	// set black background
	// Qt::black / "#000000" / "black"
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

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

	connect(ui->openButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(BaseController::LockState::UNLOCKED);
	});
	connect(ui->transitButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(BaseController::LockState::TRANSIT);
	});
	connect(ui->closedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(BaseController::LockState::LOCKED);
	});

	connect(
		ui->freeButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::FREE);
	});
	connect(
		ui->shuntButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::SHUNTING);
	});
	connect(
		ui->tourButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::TOUR);
	});
	connect(
		ui->occupiedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::OCCUPIED);
	});
	connect(
		ui->passedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setSectionState(SectionState::PASSED);
	});

	connect(
		ui->acButton, &QRadioButton::clicked,
		&mock, [&] ()
	{
		mock.setSwitchState(DoubleCrossSwitch::State::AC);
	});
	connect(
		ui->adButton, &QRadioButton::clicked,
		&mock, [&] ()
	{
		mock.setSwitchState(DoubleCrossSwitch::State::AD);
	});
	connect(
		ui->bcButton, &QRadioButton::clicked,
		&mock, [&] ()
	{
		mock.setSwitchState(DoubleCrossSwitch::State::BC);
	});
	connect(
		ui->bdButton, &QRadioButton::clicked,
		&mock, [&] ()
	{
		mock.setSwitchState(DoubleCrossSwitch::State::BD);
	});

	connect(
		&mock, &DoubleCrossSwitchControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &DoubleCrossSwitchControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));

	ui->symbolWidget->setAutoFillBackground(true);
	ui->symbolWidget->setPalette(pal);

	ui->acButton->setChecked(true);
	ui->leftHandedButton->setChecked(true);
	ui->backwardButton->setChecked(true);
	ui->freeButton->setChecked(true);
}

DoubleCrossSwitchStudy::~DoubleCrossSwitchStudy()
{
	delete ui;
}

void DoubleCrossSwitchStudy::changeEvent(QEvent * e)
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
