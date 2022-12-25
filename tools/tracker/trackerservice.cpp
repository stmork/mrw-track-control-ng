//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <can/mrwmessage.h>
#include <model/section.h>
#include <model/rail.h>
#include <statecharts/timerservice.h>

#include "trackerservice.h"

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::statechart;

TrackerService::TrackerService(
	ModelRepository & repo,
	QObject           *           parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent),
	statechart(nullptr)
{
	model = repo;

	statechart.setOperationCallback(this);
	statechart.setTimerService(&TimerService::instance());
	statechart.enter();
}

TrackerService::~TrackerService()
{
	statechart.exit();
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
			append(message.eid(), message.unitNo(), true);
			break;

		case SETROF:
			append(message.eid(), message.unitNo(), false);
			break;

		default:
			// Intentionally do nothing!
			break;
		}
	}
}

void TrackerService::append(
	const ControllerId id,
	const UnitNo unitNo,
	const bool enable)
{
	if (!statechart.isStateActive(TrackerStatechart::State::main_region_Driving))
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

		position = track.end();
		previous = position;
	}
	statechart.received();
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

void TrackerService::send(Section * section)
{
	MrwMessage message(section->controller()->id(), section->unitNo(), GETRBS, MSG_OK);
	message.append(section->occupation());
	write(message);
}

void TrackerService::first()
{
	__METHOD__;

	Section * section = nullptr;

	position = track.begin();
	previous = position;
	section  = *position;
	section->setOccupation();
	send(section);
}

void TrackerService::occupy()
{
	__METHOD__;

	Section * section = nullptr;

	// Proceed and occupy
	++position;
	section = *position;
	section->setOccupation();
	send(section);
}

void TrackerService::free()
{
	__METHOD__;

	// Lag behind and free.
	Section * section = *previous++;
	section->setOccupation(false);
	track.remove(section);

	Q_ASSERT(previous == position);
	send(section);
}

bool TrackerService::valid()
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
		return false;
	}
	return true;
}

bool TrackerService::last()
{
	return *previous == track.back();
}

void TrackerService::clear()
{
	__METHOD__;

	qDebug("End of track reached!");
	track.clear();
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
		send(section);
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
}
