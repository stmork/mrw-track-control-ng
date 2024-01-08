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

#include <util/self.h>
#include <can/mrwbusservice.h>
#include <statecharts/UpdateStatechart.h>

/**
 * This class provides the behaviour of the firmware update process. It reads
 * a HEX file containing the firmware and sends it via CAN bus to the CAN
 * controller for flashing. The main control is achieved by the
 * mrw::statemachine::UpdateStatechart.
 *
 * @img html UpdateStatechart_0.png
 */
class UpdateService :
	public mrw::can::MrwBusService,
	public mrw::util::Self<mrw::statechart::UpdateStatechart::OperationCallback>
{
	Q_OBJECT

	// ATmega32 signature
	static const uint8_t   SIGNATURE_BYTE_1 = 0x1E;
	static const uint8_t   SIGNATURE_BYTE_2 = 0x95;
	static const uint8_t   SIGNATURE_BYTE_3 = 0x02;

	// Hardware description
	static const uint8_t   DEFAULT_HARDWARE =    1;
	static const size_t    SPM_PAGESIZE     =  128;

	mrw::statechart::UpdateStatechart                statechart;
	std::unordered_multiset<mrw::can::ControllerId>  controller_ids;
	std::unordered_multiset<mrw::can::ControllerId>  request_ids;
	std::vector<uint8_t>                             buffer;

	size_t    rest     = 0;
	unsigned  address  = 0;
	unsigned  checksum = 0;

public:
	explicit UpdateService(
		const QString & filename,
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

	void init(sc::integer count) override;
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
