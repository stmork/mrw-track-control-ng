//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef WIDGETROUTE_H
#define WIDGETROUTE_H

#include <unordered_set>
#include <vector>

#include <QObject>
#include <QListWidgetItem>

#include <statecharts/RouteStatechart.h>
#include <model/section.h>
#include <model/signal.h>
#include <model/route.h>
#include <ctrl/batch.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/signalcontrollerproxy.h>

#include "routebatch.h"

class WidgetRoute :
	public mrw::model::Route,
	public mrw::ctrl::Batch,
	public mrw::statechart::RouteStatechart::OperationCallback
{
	Q_OBJECT

private:
	QListWidgetItem list_item;

	mrw::statechart::RouteStatechart   statechart;

	std::vector<mrw::ctrl::SignalControllerProxy *> controllers_unlocked;
	std::vector<mrw::ctrl::SignalControllerProxy *> controllers_locked;

	std::unordered_set<RouteBatch *, RouteBatch> disable_batches;

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
	virtual void prepare() override;

signals:
	void turn();
	void disable();
	void finished();

	// Implementation of mrw::ctrl::Batch
	void completed() const override;

public slots:
	void entered();
	void left();
	void tryUnblock();
	void unregister();

private:
	mrw::ctrl::SectionController   *   getSectionController(
		mrw::model::Section * section) const;
	mrw::ctrl::SignalControllerProxy * getSignalController(
		mrw::model::Section * section) const;

	void collectSignalControllers(
		std::vector<mrw::ctrl::SignalControllerProxy *> & controllers,
		const bool                                        unlocked) const;
	void collectSignalControllers(
		std::vector<mrw::ctrl::SignalControllerProxy *> & controllers,
		std::function<bool(mrw::ctrl::SignalControllerProxy * ctrl)> guard =
			&mrw::util::Method::always<mrw::ctrl::SignalControllerProxy>) const;
	void collectSectionControllers(
		std::vector<mrw::ctrl::SectionController *>   &   controllers) const;

	void   tryUnblockCtrl(mrw::ctrl::SectionController * controller);

	void   unregister(mrw::model::Section * section);
	void   unregister(mrw::ctrl::SectionController * controller);
	void   finalize();
	size_t countAllocatedSections();
	void   rename();

	void prepareTrack();
	void prepareSignals();

	// Implementation of RouteStatemachine::OperationCallback
	virtual void prepareRoute() override;
	virtual void prepareFlank() override;

	virtual void resetTransaction() override;
	virtual void fail() override;
	virtual void tryComplete() override;
	virtual bool isCompleted() override;
	virtual bool isTour() override;

	virtual void turnSwitches() override;
	virtual void turnFlanks() override;
	virtual void turnSignals() override;
	virtual void extendSignals() override;
	virtual void activateSections() override;

	virtual void unlockSignals() override;
	virtual void unlockSwitches() override;
	virtual void deactivateSections() override;
	virtual void unlockSections() override;
};

#endif
