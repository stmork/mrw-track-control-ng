//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/signalcontrollerproxy.h>
#include <ctrl/railcontrollerproxy.h>

#include <ui/railwidget.h>
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
	form_region(region),
	ui(new Ui::RegionForm)
{
	ui->setupUi(this);
	ui->stationWidget->setStationLabel(region->name());
	setAutoFillBackground(true);
	setPalette(background_color);

	for (size_t s = 0; s < region->sectionCount(); s++)
	{
		Section      *      section    = region->section(s);
		SectionController * controller = new SectionController(section, this);

		setupRails(controller);
		setupSignals(controller, true);
		setupSignals(controller, false);
		setupRegularSwitches(controller);
		setupDoubleCrossSwitches(controller);
	}

	connect(this, &RegionForm::brightness, ui->brightnessBar, &QProgressBar::setValue);

	setupSize(region);
}

RegionForm::~RegionForm()
{
	delete ui;
}

void RegionForm::line(std::vector<Position *> & positions, const int y) const
{
	form_region->parts<Position>(positions, [&](const Position * input)
	{
		return input->point().y() == y;
	});
}

void RegionForm::setOpMode(
	QTabWidget   *  tab,
	const QString & text,
	const QColor  & color,
	const bool      blink)
{
	for (int i = 0; i < tab->count(); i++)
	{
		QWidget   *  widget = tab->widget(i);
		RegionForm * form   = dynamic_cast<RegionForm *>(widget);

		if (form != nullptr)
		{
			form->ui->opModeWidget->setLabel(text, color, blink);
		}
	}
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

void RegionForm::setupSize(Region * region)
{
	std::vector<Position *> positions;
	int xMax = Position::NEW_LINE * Position::FRACTION;
	int yMax = 5 *                  Position::FRACTION;

	region->parts<Position>(positions);
	for (Position * pos : positions)
	{
		const QPoint & point = pos->point();
		const int      x     = point.x() + pos->width();
		const int      y     = point.y() + pos->height();

		xMax = std::max(xMax, x);
		yMax = std::max(yMax, y);
	}

	ui->controlWidget->setFixedSize(
		xMax * BaseWidget::gridSize() / Position::FRACTION + BaseWidget::gridSize(),
		yMax * BaseWidget::gridSize() / Position::FRACTION + BaseWidget::gridSize());
}

void RegionForm::setupRails(SectionController * controller)
{
	Section       *      section = controller->section();
	std::vector<Rail *>  rails;

	section->parts<Rail>(rails);
	for (Rail * rail : rails)
	{
		RailControllerProxy * ctrl   = new RailControllerProxy(rail, this);
		RailWidget      *     widget = new RailWidget(ui->controlWidget, ctrl);

		ctrl->reposition();
		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
		connect(
			controller, &BaseController::update, ctrl,
			&BaseController::update);
	}
}

void RegionForm::setupSignals(SectionController * controller, const bool direction)
{
	Section        *       section = controller->section();
	std::vector<Signal *>  section_signals;

	section->parts<Signal>(section_signals, [direction] (const Signal * input)
	{
		return direction == input->direction();
	});

	if (section_signals.size() > 0)
	{
		SignalControllerProxy * ctrl   = new SignalControllerProxy(section, direction, this);
		SignalWidget      *     widget = new SignalWidget(ui->controlWidget, ctrl);

		ctrl->reposition();
		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
		connect(
			controller, &BaseController::update, ctrl,
			&BaseController::update);
	}
}

void RegionForm::setupRegularSwitches(SectionController * controller)
{
	Section           *           section = controller->section();
	std::vector<RegularSwitch *>  switches;

	section->parts<RegularSwitch>(switches);
	for (RegularSwitch * part : switches)
	{
		RegularSwitchControllerProxy * ctrl   = new RegularSwitchControllerProxy(part, this);
		RegularSwitchWidget      *     widget = new RegularSwitchWidget(ui->controlWidget, ctrl);

		ctrl->reposition();
		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
		connect(
			controller, &BaseController::update, ctrl,
			&BaseController::update);
	}
}

void RegionForm::setupDoubleCrossSwitches(SectionController * controller)
{
	Section             *             section = controller->section();
	std::vector<DoubleCrossSwitch *>  switches;

	section->parts<DoubleCrossSwitch>(switches);
	for (DoubleCrossSwitch * part : switches)
	{
		DoubleCrossSwitchControllerProxy * ctrl   = new DoubleCrossSwitchControllerProxy(part, this);
		DoubleCrossSwitchWidget      *     widget = new DoubleCrossSwitchWidget(ui->controlWidget, ctrl);

		ctrl->reposition();
		connect(
			ctrl, &BaseController::update,
			widget, qOverload<>(&BaseWidget::repaint));
		connect(
			controller, &BaseController::update, ctrl,
			&BaseController::update);
	}
}
