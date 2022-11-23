//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "sectionstudy.h"
#include "ui_sectionstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;
using namespace mrw::model;

SectionStudy::SectionStudy(QWidget * parent) :
	QWidget(parent),
	ui(new Ui::SectionStudy)
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
	connect(
		ui->extensionBox, qOverload<int>(&QSpinBox::valueChanged),
		&mock, &SectionControllerMock::setExtension);

	/********************************************************/
	/*   End rail state                                     */
	/********************************************************/

	connect(ui->leftButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setEnds(true, false);
	});
	connect(ui->noButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setEnds(false, false);
	});
	connect(ui->rightButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setEnds(false, true);
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
		&mock, &SectionControllerMock::update,
		ui->bigSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &SectionControllerMock::extend,
		ui->bigSwitchWidget, qOverload<>(&SectionWidget::extend));
	connect(
		&mock, &SectionControllerMock::update,
		ui->smallSwitchWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &SectionControllerMock::extend,
		ui->smallSwitchWidget, qOverload<>(&SectionWidget::extend));

	ui->forwardButton->setChecked(true);
	ui->noButton->setChecked(true);
	ui->freeButton->setChecked(true);
	ui->openButton->setChecked(true);
}

SectionStudy::~SectionStudy()
{
	delete ui;
}

void SectionStudy::changeEvent(QEvent * e)
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
