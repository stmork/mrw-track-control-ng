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
	class BatchParticipant;

	class Batch
	{
		std::unordered_set<BatchParticipant *> transaction;

	public:
		void increase(BatchParticipant * ctrl);
		void decrease(BatchParticipant * ctrl);
		bool contains(BatchParticipant * ctrl);
		void reset();
		void tryComplete();
		bool isCompleted();
		void dump();

		virtual void completed() const = 0;
	};
}

#endif
