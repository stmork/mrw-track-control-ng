//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "widgetstudy.h"
#include "ui_widgetstudy.h"

using namespace mrw::ui;

WidgetStudy::WidgetStudy(QWidget * parent) :
	WidgetSaver(parent),
	ui(new Ui::WidgetStudy)
{
	QPalette pal = QPalette();
	pal.setColor(QPalette::Window, Qt::black);

	ui->setupUi(this);

	ui->rightWidget->setAutoFillBackground(true);
	ui->rightWidget->setPalette(pal);

	ui->opModeWidget->setLabel(ui->opModeEdit->text());
	ui->stationWidget->setStationLabel(ui->stationEdit->text());
	ui->barWidget->setVerbose(ui->blinkChecker->isChecked());
}

WidgetStudy::~WidgetStudy()
{
	delete ui;
}

void WidgetStudy::changeEvent(QEvent * e)
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

void WidgetStudy::on_stationEdit_textChanged(const QString & text)
{
	ui->stationWidget->setStationLabel(text);
}

void WidgetStudy::on_opModeEdit_textChanged(const QString & text)
{
	ui->opModeWidget->setLabel(text, BaseWidget::WHITE, ui->blinkChecker->isChecked());
}

void WidgetStudy::on_blinkChecker_clicked()
{
	ui->opModeWidget->setLabel(ui->opModeEdit->text(), BaseWidget::WHITE, ui->blinkChecker->isChecked());
}

void WidgetStudy::save()
{
	ui->barWidget->grab().save(path("BarWidget"));
	ui->clockWidget->grab().save(path("ClockWidget"));
	ui->stationWidget->grab().save(path("StationWidget"));
	ui->opModeWidget->grab().save(path("OpModeWidget"));
}

QWidget * WidgetStudy::widget() const
{
	return nullptr;
}

QString WidgetStudy::name() const
{
	return "";
}
