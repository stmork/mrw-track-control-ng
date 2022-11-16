//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "regionform.h"
#include "ui_regionform.h"

// set black background
// Qt::black / "#000000" / "black"
const QPalette RegionForm::background_color(QPalette::Window, Qt::black);

RegionForm::RegionForm(mrw::model::Region * region, QWidget * parent) :
	QWidget(parent),
	ui(new Ui::RegionForm)
{
	ui->setupUi(this);
	ui->stationWidget->setStationLabel(region->name());
	setAutoFillBackground(true);
	setPalette(background_color);
}

RegionForm::~RegionForm()
{
	delete ui;
}

void RegionForm::changeEvent(QEvent * e)
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
