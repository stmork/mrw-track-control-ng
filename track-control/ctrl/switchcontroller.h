//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SWITCHCONTROLLER_H
#define MRW_CTRL_SWITCHCONTROLLER_H

#include <ctrl/railpartinfo.h>
#include <ctrl/controllerregistrand.h>
#include <ctrl/batchparticipant.h>
#include <statecharts/SwitchStatechart.h>

namespace mrw::ctrl
{
	class SwitchController :
		public RailPartInfo,
		public ControllerRegistrand,
		public BatchParticipant,
		public mrw::statechart::SwitchStatechart::OperationCallback
	{
	protected:
		mrw::statechart::SwitchStatechart   statechart;

	public:
		SwitchController();
		virtual ~SwitchController();

		// Implementations from OperationCallback
		virtual void inc() override;
		virtual void dec() override;
	};
}

#endif
