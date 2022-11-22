//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/signalcontrollerproxy.h>
#include <ctrl/sectioncontrollerproxy.h>

#include <ui/sectionwidget.h>
#include <ui/signalwidget.h>
#include <ui/regularswitchwidget.h>
#include <ui/doublecrossswitchwidget.h>

#include <model/rail.h>

#include "regionform.h"
#include "ui_regionform.h"

using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::ui;

// set black background
// Qt::black / "#000000" / "black"
const QPalette RegionForm::background_color(QPalette::Window, Qt::black);

RegionForm::RegionForm(Region * region, QWidget * parent) :
	QWidget(parent),
	ui(new Ui::RegionForm)
{
	ui->setupUi(this);
	ui->stationWidget->setStationLabel(region->name());
	setAutoFillBackground(true);
	setPalette(background_color);

	for (size_t s = 0; s < region->sectionCount(); s++)
	{
		Section * section = region->section(s);

		setupSection(section);
		setupSignals(section, true);
		setupSignals(section, false);
	}

	region->parts<RegularSwitch>(region_reg_switches);
	for (RegularSwitch * part : region_reg_switches)
	{
		RegularSwitchControllerProxy * ctrl   = new RegularSwitchControllerProxy(part, this);
		RegularSwitchWidget      *     widget = new RegularSwitchWidget(ui->controlWidget, ctrl);

		widget->setFixedSize(BaseWidget::SIZE, BaseWidget::SIZE);
		widget->move(part->point() * BaseWidget::SIZE / 2);

		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
	}

	region->parts<DoubleCrossSwitch>(region_dc_switches);
	for (DoubleCrossSwitch * part : region_dc_switches)
	{
		DoubleCrossSwitchControllerProxy * ctrl   = new DoubleCrossSwitchControllerProxy(part, this);
		DoubleCrossSwitchWidget      *     widget = new DoubleCrossSwitchWidget(ui->controlWidget, ctrl);

		widget->setFixedSize(BaseWidget::SIZE, BaseWidget::SIZE);
		widget->move(part->point() * BaseWidget::SIZE / 2);

		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
	}

	setupSize(region);
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

void RegionForm::setupSize(mrw::model::Region * region)
{
	std::vector<Position *> positions;
	int xMax = 20;
	int yMax = 10;

	region->parts<Position>(positions);
	for (Position * pos : positions)
	{
		QPoint point = pos->point();

		xMax = std::max(xMax, point.x());
		yMax = std::max(yMax, point.y());
	}

	fields.setWidth(xMax);
	fields.setHeight(yMax);

	ui->controlWidget->setFixedSize(
		xMax * BaseWidget::SIZE / 2 + 100,
		yMax * BaseWidget::SIZE / 2 + 100);
}

void RegionForm::setupSection(Section * section)
{
	std::vector<Rail *> rails;

	section->parts<Rail>(rails);

	for (Rail * rail : rails)
	{
		SectionControllerProxy * ctrl   = new SectionControllerProxy(section, rail, this);
		SectionWidget      *     widget = new SectionWidget(ui->controlWidget, ctrl);

		widget->setFixedHeight(BaseWidget::SIZE);
		widget->extend();
		widget->move(rail->point() * BaseWidget::SIZE / 2);

		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
	}
}

void RegionForm::setupSignals(Section * section, const bool direction)
{
	std::vector<Signal *> section_signals;

	section->parts<Signal>(section_signals, [direction] (const Signal * input)
	{
		return direction == input->direction();
	});

	if (section_signals.size() > 0)
	{
		SignalControllerProxy * ctrl   = new SignalControllerProxy(section, direction, this);
		SignalWidget      *     widget = new SignalWidget(ui->controlWidget, ctrl);

		widget->setFixedHeight(BaseWidget::SIZE);
		widget->extend();
		widget->move(section_signals[0]->point() * BaseWidget::SIZE / 2);

		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
	}
}
