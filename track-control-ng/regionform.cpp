//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/signalwidget.h>
#include <ui/regularswitchwidget.h>

#include "regionform.h"
#include "ui_regionform.h"

using namespace mrw::model;
using namespace mrw::ui;

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

	region->parts<Signal>(region_signals);
	for (Signal * signal : region_signals)
	{
		SignalWidget * widget = new SignalWidget(ui->controlWidget);

		widget->setFixedSize(BaseWidget::SIZE, BaseWidget::SIZE);
		widget->move(signal->position() * BaseWidget::SIZE);
	}

	region->parts<RegularSwitch>(region_switches);
	for (RegularSwitch * part : region_switches)
	{
		RegularSwitchWidget * widget = new RegularSwitchWidget(ui->controlWidget);

		widget->setFixedSize(BaseWidget::SIZE, BaseWidget::SIZE);
		widget->move(part->position() * BaseWidget::SIZE);
	}
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
