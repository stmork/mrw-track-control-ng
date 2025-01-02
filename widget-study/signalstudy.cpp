//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "signalstudy.h"
#include "ui_signalstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;
using namespace mrw::model;

SignalStudy::SignalStudy(QWidget * parent) :
	WidgetSaver(parent),
	ui(new Ui::SignalStudy)
{
	ui->setupUi(this);

	// set black background
	// Qt::black / "#000000" / "black"
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

	ui->symbolWidget->setAutoFillBackground(true);
	ui->symbolWidget->setPalette(pal);

	ui->bigSignalWidget->setController(&mock);
	ui->smallSignalWidget->setController(&mock);

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
		&mock, &SignalControllerMock::setExtension);

	/********************************************************/
	/*   Bended state                                       */
	/********************************************************/

	connect(ui->leftBendButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setBending(Position::Bending::LEFT);
	});
	connect(ui->straightButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setBending(Position::Bending::STRAIGHT);
	});
	connect(ui->rightBendButton, &QRadioButton::clicked,
		&mock, [&]()
	{
		mock.setBending(Position::Bending::RIGHT);
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
	/*  Signal existance and state                          */
	/********************************************************/

	connect(
		ui->availableButton_1, &QCheckBox::clicked,
		&mock, &SignalControllerMock::setShunting);
	connect(
		ui->stopButton_1, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setShuntStop);
	connect(
		ui->goButton_1, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setShuntGo);
	connect(
		ui->availableButton_2, &QCheckBox::clicked,
		&mock, &SignalControllerMock::setDistant);
	connect(
		ui->stopButton_2, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setDistantStop);
	connect(
		ui->goButton_2, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setDistantGo);
	connect(
		ui->availableButton_3, &QCheckBox::clicked,
		&mock, &SignalControllerMock::setMain);
	connect(
		ui->stopButton_3, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setMainStop);
	connect(
		ui->goButton_3, &QRadioButton::clicked,
		&mock, &SignalControllerMock::setMainGo);

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
		&mock, &SignalControllerMock::update,
		ui->bigSignalWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &SignalControllerMock::extend,
		ui->bigSignalWidget, &SignalWidget::extend);
	connect(
		&mock, &SignalControllerMock::computeConnectors,
		ui->bigSignalWidget, &SignalWidget::computeConnectors);
	connect(
		&mock, &SignalControllerMock::update,
		ui->smallSignalWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &SignalControllerMock::extend,
		ui->smallSignalWidget, &SignalWidget::extend);
	connect(
		&mock, &SignalControllerMock::computeConnectors,
		ui->smallSignalWidget, &SignalWidget::computeConnectors);

	ui->forwardButton->setChecked(true);
	ui->straightButton->setChecked(true);
	ui->freeButton->setChecked(true);

	ui->availableButton_1->setChecked(false);
	ui->availableButton_2->setChecked(false);
	ui->availableButton_3->setChecked(true);
	ui->stopButton_1->setChecked(true);
	ui->stopButton_2->setChecked(true);
	ui->stopButton_3->setChecked(true);
	ui->openButton->setChecked(true);
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

QWidget * SignalStudy::widget() const
{
	return ui->bigSignalWidget;
}

QString SignalStudy::name() const
{
	SignalController::Status status;

	mock.status(status);
	return QString("Signal__%1%2%3_%4%5%6").
		arg(code(status.has_main,     status.main_state)).
		arg(code(status.has_distant,  status.distant_state)).
		arg(code(status.has_shunting, status.shunt_state)).
		arg(direction(status.direction)).
		arg(lockState(status.lock_state)).
		arg(sectionState(status.section_state));
}

QString SignalStudy::code(const bool valid, const Signal::Symbol & symbol)
{
	if (valid)
	{
		return Signal::get(symbol).at(0);
	}
	else
	{
		return "_";
	}
}
