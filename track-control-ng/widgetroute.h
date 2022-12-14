//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef WIDGETROUTE_H
#define WIDGETROUTE_H

#include <QObject>
#include <QListWidgetItem>

#include <statecharts/RouteStatechart.h>
#include <model/section.h>
#include <model/signal.h>
#include <model/route.h>
#include <ctrl/sectioncontroller.h>

class WidgetRoute :
	public mrw::model::Route,
	public mrw::statechart::RouteStatechart::OperationCallback
{
	Q_OBJECT

private:
	QListWidgetItem list_item;

	mrw::statechart::RouteStatechart   statechart;
	std::vector<mrw::model::Signal *>  main_signals;
	mrw::model::Section        *       last = nullptr;

public:
	static const int        USER_ROLE = Qt::UserRole + 1;

	explicit WidgetRoute(
		const bool                       dir,
		const mrw::model::SectionState   wanted_state,
		mrw::model::RailPart      *      first,
		QObject             *            parent = nullptr);
	virtual ~WidgetRoute();

	operator QListWidgetItem * ();

	virtual void prepare() override;

signals:
	void turn();
	void finished();

public slots:
	void left();
	void unregister();

private:
	void collectSignals();
	void collectSignals(mrw::model::Section * section);
	void collectSectionControllers(std::vector<mrw::ctrl::SectionController *> & controllers);

	virtual void reset() override;
	virtual void turnSwitches() override;
	virtual void activateSections() override;
	virtual void turnSignals() override;
	virtual void deactivateSections() override;
	virtual void unlockSignals() override;
	virtual void unlockSwitches() override;
};

#endif // WIDGETROUTE_H
