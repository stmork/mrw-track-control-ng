//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <can/mrwmessage.h>
#include <model/section.h>

#include "trackerservice.h"

using namespace mrw::can;
using namespace mrw::model;

TrackerService::TrackerService(
	mrw::model::ModelRepository & repo,
	QObject           *           parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent)
{
	model = repo;
	connect(
		&timer, &QTimer::timeout,
		this, &TrackerService::trigger,
		Qt::QueuedConnection);
	timer.setSingleShot(true);
}

void TrackerService::info()
{
	if (model != nullptr)
	{
		model->info();
	}
}

void TrackerService::process(const mrw::can::MrwMessage & message)
{
	if (message.isResponse() && (message.response() == MSG_OK))
	{
		const Command cmd = message.command();

		switch (cmd)
		{
		case SETRON:
			append(message.unitNo());
			break;

		case SETROF:
			remove(message.unitNo());
			break;

		default:
			// Intentionally do nothing!
			break;
		}
	}
}

void TrackerService::append(const UnitNo unitNo)
{
	Device  * device  = model->deviceByUnitNo(unitNo);
	Section * section = dynamic_cast<Section *>(device);

	track.push_front(section);
	qDebug().noquote() << *section;
	timer.stop();
	position = track.end();
	previous = position;
	timer.start(1000);
}

void TrackerService::remove(const UnitNo unitNo)
{
	Device  * device  = model->deviceByUnitNo(unitNo);
	Section * section = dynamic_cast<Section *>(device);

	track.remove(section);
}

void TrackerService::trigger()
{
	Section * section = nullptr;

	if (position == track.end())
	{
		// First init!
		position = track.begin();
		previous = position;
		section  = *position;
		section->setOccupation();
	}
	else if (position == previous)
	{
		// Proceed and occupy
		++position;
		section = *position;
		section->setOccupation();
	}
	else
	{
		// Lag behind and free.
		section = *previous++;
		section->setOccupation(false);

		Q_ASSERT(previous == position);
	}

	MrwMessage message(section->controller()->id(), section->unitNo(), GETRBS, MSG_OK);
	message.append(section->occupation());

#if 0
	qDebug() << message;
#else
	write(message);
#endif

	if (*previous == track.back())
	{
		qDebug("End of track reached!");
		track.clear();
		return;
	}
	else
	{
		timer.start(300);
	}
}
