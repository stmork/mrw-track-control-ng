//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SWITCHCONTROLLER_H
#define MRW_CTRL_SWITCHCONTROLLER_H

#include <util/batchparticipant.h>
#include <util/self.h>
#include <ctrl/railpartinfo.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/timerservice.h>
#include <statecharts/SwitchStatechart.h>

namespace mrw::ctrl
{
	class SwitchController :
		public RailPartInfo,
		public ControllerRegistrand,
		public mrw::util::BatchParticipant,
		public mrw::util::Self<mrw::statechart::SwitchStatechart::OperationCallback>
	{
	protected:
		mrw::statechart::QtStatechart<mrw::statechart::SwitchStatechart>  statechart;

	public:
		SwitchController();
		virtual ~SwitchController();

		void setManual(const bool manual);

		// Implementations from OperationCallback
		virtual void inc() override;
		virtual void dec() override;
		virtual bool isReserved() override;

	protected:
		bool isFlankRelevantReserved() const;
	};
}

#endif
