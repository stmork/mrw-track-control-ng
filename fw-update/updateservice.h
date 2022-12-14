//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef UPDATESERVICE_H
#define UPDATESERVICE_H

#include <cstdint>
#include <vector>
#include <unordered_set>

#include <can/mrwbusservice.h>
#include <statecharts/UpdateStatechart.h>

class UpdateService :
	public mrw::can::MrwBusService,
	public mrw::statechart::UpdateStatechart::OperationCallback
{
	Q_OBJECT

	// ATmega32 signature
	static const uint8_t   SIGNATURE_BYTE_1 = 0x1E;
	static const uint8_t   SIGNATURE_BYTE_2 = 0x95;
	static const uint8_t   SIGNATURE_BYTE_3 = 0x02;

	// Hardware description
	static const uint8_t   DEFAULT_HARDWARE =    1;
	static const size_t    SPM_PAGESIZE     =  128;

	mrw::statechart::UpdateStatechart          statechart;
	std::unordered_set<mrw::can::ControllerId> controller_ids;
	std::unordered_set<mrw::can::ControllerId> request_ids;
	std::vector<uint8_t>                       buffer;

	size_t    rest     = 0;
	unsigned  address  = 0;
	unsigned  checksum = 0;

public:
	explicit UpdateService(
		const QString & interface = "can0",
		const QString & plugin    = "socketcan",
		QObject    *    parent    = nullptr);
	virtual ~UpdateService();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void read(const QString & filename);
	bool check(
		const mrw::can::MrwMessage & message,
		const mrw::can::Response     response = mrw::can::MSG_OK);

	void init() override;
	void ping() override;
	void boot() override;
	void flashRequest(const uint8_t hid);
	void flashRequest() override;
	void flashData(const size_t bytes);
	void flashCheck() override;

	void flashCompletePage() override;
	void flashRestPage() override;
	void quit() override;
	void fail(sc::integer error_code) override;

	bool hasController() override;
	bool hasPages() override;
};

#endif
