//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "doublecrossswitchstudy.h"
#include "ui_doublecrossswitchstudy.h"

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
