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
#include <ctrl/signalcontrollerproxy.h>

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

	// Implementation of mrw::model::Route
	virtual void dump() const override;
	virtual void prepare(
		mrw::model::Section  * last_section,
		mrw::model::RailPart * last_part) override;

signals:
	void turn();
	void disable();
	void finished();

public slots:
	void left();
	void tryUnblock();
	void unregister();

private:
	mrw::ctrl::SignalControllerProxy * getSignalController(
		mrw::model::Section * section) const;

	void collectSignalController(
		std::vector<mrw::ctrl::SignalControllerProxy *> & controllers) const;
	void collectMainSignals();
	void collectMainSignals(
		mrw::model::Section               *               section);
	void collectSectionControllers(
		std::vector<mrw::ctrl::SectionController *>   &   controllers) const;

	void unregister(mrw::model::Section * section);
	void unregister(mrw::ctrl::SectionController * controller);
	void finalize();
	size_t countAllocatedSections();

	void prepareSections(
		mrw::model::Section  * last_section,
		mrw::model::RailPart * last_part);
	void prepareTrack(
		mrw::model::Section  * last_section,
		mrw::model::RailPart * last_part);
	void prepareSignals(
		mrw::model::Section  * last_section,
		mrw::model::RailPart * last_part);

	// Implementation of RouteStatemachine::OperationCallback
	virtual void reset() override;
	virtual void tryComplete() override;
	virtual bool isCompleted() override;

	virtual void turnSwitches() override;
	virtual void activateSections() override;
	virtual void turnSignals() override;
	virtual void deactivateSections() override;
	virtual void unlockSignals() override;
	virtual void unlockSwitches() override;
};

#endif // WIDGETROUTE_H
