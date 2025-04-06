//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef RESETSERVICE_H
#define RESETSERVICE_H

#include <unordered_set>

#include <QLoggingCategory>

#include <can/mrwbusservice.h>

/**
 * This service class lets reset a modelrailway.
 */
class ResetService : public mrw::can::MrwBusService
{
	Q_OBJECT

private:
	QLoggingCategory                                log;

	std::unordered_multiset<mrw::can::ControllerId> controllers;

public:
	explicit ResetService(
		const QString & interface,
		const QString & plugin,
		QObject    *    parent = nullptr);
	ResetService() = delete;

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void insert(const mrw::can::ControllerId id);
	void remove(const mrw::can::ControllerId id);
};

#endif
