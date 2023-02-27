//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef WIDGETROUTE_H
#define WIDGETROUTE_H

#include <vector>

#include <QObject>
#include <QListWidgetItem>

#include <util/batch.h>
#include <model/section.h>
#include <model/signal.h>
#include <model/route.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/signalcontrollerproxy.h>
#include <statecharts/RouteStatechart.h>

#include "routebatch.h"

class WidgetRoute :
	public mrw::model::Route,
	public mrw::util::Batch,
	public mrw::statechart::RouteStatechart::OperationCallback
{
	Q_OBJECT

private:
	QListWidgetItem list_item;

	mrw::statechart::RouteStatechart   statechart;

	std::vector<mrw::ctrl::SignalControllerProxy *> controllers_unlocked;
	std::vector<mrw::ctrl::SignalControllerProxy *> controllers_locked;

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

signals:
	void turn();
	void disable();
	void finished();

	// Implementation of mrw::ctrl::Batch
	void completed() override;

public slots:
	void entered();
	void leaving();
	void left();
	void tryUnblock();
	void unregister();

private:
	void   connectSectionController(mrw::ctrl::SectionController * controller);
	void   disconnectSectionController(mrw::ctrl::SectionController * controller);

	void   tryUnblockCtrl(mrw::ctrl::SectionController * controller);

	void   unregister(mrw::model::Section * section);
	void   unregister(mrw::ctrl::SectionController * controller);
	void   finalize();
	void   rename();

	void   prepare() override;
	void   prepareSections();
	void   prepareTrack();
	void   prepareSignals();

	size_t countAllocatedSections();

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

	// Implementation of RouteStatemachine::OperationCallback
	virtual void prepareRoute() override;
	virtual void prepareFlank() override;

	virtual bool isTour() override;
	virtual bool isCompleted() override;
	virtual void resetTransaction() override;
	virtual void fail() override;
	virtual void tryComplete() override;

	virtual void turnSwitches() override;
	virtual void turnFlanks() override;
	virtual void enableSignals() override;
	virtual void extendSignals() override;
	virtual void enableSections() override;

	virtual void disableSections() override;
	virtual void disableSignals() override;
	virtual void unlockFlanks() override;
	virtual void unlockRailParts() override;
	virtual void unlockSections() override;
};

#endif
