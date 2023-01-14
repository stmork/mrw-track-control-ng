//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BATCH_H
#define MRW_CTRL_BATCH_H

#include <unordered_set>

namespace mrw::ctrl
{
	class ControllerRegistrand;

	class Batch
	{
		std::unordered_set<ControllerRegistrand *> transaction;

	public:
		void increase(ControllerRegistrand * ctrl);
		void decrease(ControllerRegistrand * ctrl);
		bool contains(ControllerRegistrand * ctrl);
		void reset();
		void tryComplete();
		bool isCompleted();
		void dump();

		virtual void completed() const = 0;
	};
}

#endif
