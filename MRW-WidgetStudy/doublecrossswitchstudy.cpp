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
	WidgetSaver(parent),
	ui(new Ui::DoubleCrossSwitchStudy)
{
	ui->setupUi(this);

	// set black background
	// Qt::black / "#000000" / "black"
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

	ui->bigSwitchWidget->setController(&mock);
	ui->smallSwitchWidget->setController(&mock);

	/********************************************************/
	/*  Switch orientation                                  */
	/********************************************************/

	connect(
		ui->leftHandedButton, &QRadioButton::clicked,
		&mock, &DoubleCrossSwitchControllerMock::setLeftHanded);
	connect(
		ui->rightHandedButton, &QRadioButton::clicked,
		&mock, &DoubleCrossSwitchControllerMock::setRightHanded);

	/********************************************************/
	/*  Switch direction                                    */
	/********************************************************/

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
	/*   Section state                                      */
	/********************************************************/

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

	/********************************************************/
	/*   Lock state                                         */
	/********************************************************/

	connect(ui->errorButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::FAIL);
	});
	connect(ui->openButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::UNLOCKED);
	});
	connect(ui->pendingButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::PENDING);
	});
	connect(ui->lockedButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setLock(Device::LockState::LOCKED);
	});

	/********************************************************/
	/*   Repaint connection                                 */
	/********************************************************/

	connect(
		&mock, &DoubleCrossSwitchControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &DoubleCrossSwitchControllerMock::computeConnectors,
		ui->bigSwitchWidget, &DoubleCrossSwitchWidget::computeConnectors);
	connect(
		&mock, &DoubleCrossSwitchControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &DoubleCrossSwitchControllerMock::computeConnectors,
		ui->smallSwitchWidget, &DoubleCrossSwitchWidget::computeConnectors);

	ui->symbolWidget->setAutoFillBackground(true);
	ui->symbolWidget->setPalette(pal);

	ui->forwardButton->setChecked(true);
	ui->openButton->setChecked(true);
	ui->leftHandedButton->setChecked(true);
	ui->freeButton->setChecked(true);
	ui->acButton->setChecked(true);
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

QWidget * DoubleCrossSwitchStudy::widget() const
{
	return ui->bigSwitchWidget;
}

QString DoubleCrossSwitchStudy::name() const
{
	return QString("XSwitch_%1_%2%3%4").
		arg(state(mock.switchState())).
		arg(direction(mock.isDirection())).
		arg(lockState(mock.lock())).
		arg(sectionState(mock.state()));
}

QString DoubleCrossSwitchStudy::state(const DoubleCrossSwitch::State & state)
{
	const QString label = DoubleCrossSwitch::get(state);

	return label.split(":").last();
}
