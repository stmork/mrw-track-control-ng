//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef TRACKERSERVICE_H
#define TRACKERSERVICE_H

#include <QTimer>

#include <can/mrwbusservice.h>
#include <model/modelrepository.h>
#include <model/route.h>

class TrackerService : public mrw::can::MrwBusService
{
	Q_OBJECT

	mrw::model::ModelRailway *                  model = nullptr;
	mrw::model::Route::SectionTrack             track;

	mrw::model::Route::SectionTrack::iterator   position;
	mrw::model::Route::SectionTrack::iterator   previous;

	QTimer                                      timer;
	bool                                        driving = false;

public:
	TrackerService() = delete;
	explicit TrackerService(
			mrw::model::ModelRepository & repo,
			QObject   *                   parent    = nullptr);
	virtual ~TrackerService() = default;

	void info();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void append(
			const mrw::can::ControllerId id,
			const mrw::can::UnitNo       unitNo,
			const bool                   enable = true);
	void remove(
			const mrw::can::ControllerId id,
			const mrw::can::UnitNo       unitNo);

private slots:
	void trigger();
};

#endif
