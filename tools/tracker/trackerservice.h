//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef TRACKERSERVICE_H
#define TRACKERSERVICE_H

#include <QTimer>

#include <util/self.h>
#include <can/mrwbusservice.h>
#include <model/modelrepository.h>
#include <model/route.h>
#include <statecharts/timerservice.h>
#include <statecharts/TrackerStatechart.h>

class TrackerService :
	public mrw::can::MrwBusService,
	public mrw::util::Self<mrw::statechart::TrackerStatechart::OperationCallback>
{
	Q_OBJECT

	mrw::statechart::QtStatechart<mrw::statechart::TrackerStatechart> statechart;

	mrw::model::ModelRailway          *         model = nullptr;
	mrw::model::Route::SectionTrack             track;

	mrw::model::Route::SectionTrack::iterator   position;
	mrw::model::Route::SectionTrack::iterator   previous;

public:
	TrackerService() = delete;
	explicit TrackerService(
		mrw::model::ModelRepository & repo,
		QObject           *           parent    = nullptr);
	virtual ~TrackerService();

	void info();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void append(
		const mrw::can::ControllerId id,
		const mrw::can::UnitNo       unitNo,
		const bool                   enable);
	void send(mrw::model::Section * section);

	void first() override;
	void occupy() override;
	void free() override;
	bool valid() override;
	bool last() override;
	void clear() override;
};

#endif
