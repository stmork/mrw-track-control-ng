//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "railstudy.h"
#include "ui_railstudy.h"

using namespace mrw::ctrl;
using namespace mrw::ui;
using namespace mrw::model;

RailStudy::RailStudy(QWidget * parent) :
	WidgetSaver(parent),
	ui(new Ui::RailStudy)
{
	ui->setupUi(this);

	// set black background
	// Qt::black / "#000000" / "black"
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

	ui->symbolWidget->setAutoFillBackground(true);
	ui->symbolWidget->setPalette(pal);

	ui->bigRailWidget->setController(&mock);
	ui->smallRailWidget->setController(&mock);

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
		&mock, &RailControllerMock::setExtension);
	connect(
		ui->lineBox, qOverload<int>(&QSpinBox::valueChanged),
		this, &RailStudy::updateLines);

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
		&mock, &RailControllerMock::update,
		ui->bigRailWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &RailControllerMock::extend,
		ui->bigRailWidget, &RailWidget::extend);
	connect(
		&mock, &RailControllerMock::computeConnectors,
		ui->bigRailWidget, &RailWidget::computeConnectors);
	connect(
		&mock, &RailControllerMock::update,
		ui->smallRailWidget, qOverload<>(&QWidget::repaint));
	connect(
		&mock, &RailControllerMock::extend,
		ui->smallRailWidget, &RailWidget::extend);
	connect(
		&mock, &RailControllerMock::computeConnectors,
		ui->smallRailWidget, &RailWidget::computeConnectors);

	ui->forwardButton->setChecked(true);
	ui->noButton->setChecked(true);
	ui->straightButton->setChecked(true);
	ui->freeButton->setChecked(true);
	ui->openButton->setChecked(true);
}

RailStudy::~RailStudy()
{
	delete ui;
}

void RailStudy::changeEvent(QEvent * e)
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

QWidget * RailStudy::widget() const
{
	return ui->bigRailWidget;
}

QString RailStudy::name() const
{
	return QString("Rail____%1_%2%3%4").
		arg(code(mock.aEnds(), mock.bEnds())).
		arg(direction(mock.isDirection())).
		arg(lockState(mock.lock())).
		arg(sectionState(mock.state()));
}

void RailStudy::updateLines(const int new_ext_lines)
{
	const float rel = (1.0f + new_ext_lines) / (1.0f + mock.lines());

	resize(ui->smallRailWidget, rel);
	resize(ui->bigRailWidget, rel);

	mock.setLines(new_ext_lines);
}

void RailStudy::resize(QWidget * widget, const float rel)
{
	widget->setFixedHeight(widget->height() * rel);
}

QString RailStudy::code(const bool a_end, const bool b_end)
{
	if (a_end)
	{
		return "A";
	}
	else if (b_end)
	{
		return "B";
	}
	else
	{
		return "I";
	}
}
