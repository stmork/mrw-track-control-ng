//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef UPDATESERVICE_H
#define UPDATESERVICE_H

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

	mrw::statechart::UpdateStatechart statechart;

	uint8_t * buffer   = nullptr;
	size_t    size     = 0;
	unsigned  address  = 0;
	unsigned  checksum = 0;

public:
	explicit UpdateService(
			const QString & interface = "can0",
			const QString & plugin    = "socketcan",
			QObject *       parent    = nullptr);
	virtual ~UpdateService();

private:
	void read(const char * filename);
	void ping() override;
	void reset() override;
	void flashRequest(const uint8_t hid);
	void flashRequest() override;
	void flashData(const size_t bytes);
	void flashCheck() override;

	void flashCompletePage() override;
	void flashRestPage() override;
	void quit() override;

	bool hasPages() override;
};

#endif
