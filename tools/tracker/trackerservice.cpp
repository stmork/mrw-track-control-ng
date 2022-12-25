//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <can/mrwmessage.h>
#include <model/section.h>
#include <model/rail.h>

#include "trackerservice.h"

using namespace mrw::can;
using namespace mrw::model;

TrackerService::TrackerService(
	ModelRepository & repo,
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

void TrackerService::process(const MrwMessage & message)
{
	if (message.isResponse() && (message.response() == MSG_OK))
	{
		const Command cmd = message.command();

		switch (cmd)
		{
		case SETRON:
			append(message.eid(), message.unitNo());
			break;

		case SETROF:
			if (driving)
			{
				remove(message.eid(), message.unitNo());
			}
			else if (track.empty())
			{
				append(message.eid(), message.unitNo(), false);
			}
			break;

		default:
			// Intentionally do nothing!
			break;
		}
	}
}

void TrackerService::append(const ControllerId id, const UnitNo unitNo, const bool enable)
{
	Device  * device  = model->deviceById(id, unitNo);
	Section * section = dynamic_cast<Section *>(device);

	if (section == nullptr)
	{
		throw std::invalid_argument(QString::asprintf(
				"Device not found: %04x:%04x", id, unitNo).toStdString());
	}
	section->enable(enable);
	track.push_front(section);
	qDebug().noquote() << *section;

	timer.stop();
	position = track.end();
	previous = position;
	timer.start(1000);
}

void TrackerService::remove(const ControllerId id, const UnitNo unitNo)
{
	Device  * device  = model->deviceById(id, unitNo);
	Section * section = dynamic_cast<Section *>(device);

	if (section == nullptr)
	{
		throw std::invalid_argument(QString::asprintf(
				"Device not found: %04x:%04x", id, unitNo).toStdString());
	}
	track.remove(section);
}

void TrackerService::trigger()
{
	Section * section = nullptr;

	if (position == track.end())
	{
		size_t on  = std::count_if(track.begin(), track.end(), [](Section * candidate)
		{
			return candidate->enabled();
		});
		size_t off = std::count_if(track.begin(), track.end(), [](Section * candidate)
		{
			return !candidate->enabled();
		});

		if ((on == 0) || (off > 1))
		{
			qWarning().noquote() << "Sections on:  " << on;
			qWarning().noquote() << "Sections off: " << off;
			track.clear();
			return;
		}

		// First init!
		position = track.begin();
		previous = position;
		section  = *position;
		section->setOccupation();
		driving = true;
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
		driving = false;

		return;
	}
	else
	{
		timer.start(300);
	}
}
