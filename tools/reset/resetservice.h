//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef RESETSERVICE_H
#define RESETSERVICE_H

#include <unordered_set>

#include <can/mrwbusservice.h>

class ResetService : public mrw::can::MrwBusService
{
	Q_OBJECT

	std::unordered_multiset<mrw::can::ControllerId> controllers;

public:
	explicit ResetService(
		const QString & interface,
		const QString & plugin,
		QObject    *    parent = nullptr);

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void insert(const mrw::can::ControllerId id);
	void remove(const mrw::can::ControllerId id);
};

#endif
