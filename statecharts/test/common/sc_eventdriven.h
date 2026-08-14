/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//
* */

#ifndef SC_EVENTDRIVEN_H_
#define SC_EVENTDRIVEN_H_

#include "sc_statemachine.h"

namespace sc
{

	/*! \file
	Interface for event-driven state machines.
	*/
	class EventDrivenInterface : public sc::StatemachineInterface
	{
	public:

		/*! Start a run-to-completion step without any event.
		*/
		virtual void triggerWithoutEvent() = 0;

	protected:
		~EventDrivenInterface() noexcept = default;
	};

} /* namespace sc */

#endif /* SC_EVENTDRIVEN_H_ */
