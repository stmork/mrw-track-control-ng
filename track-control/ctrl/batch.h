//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BATCH_H
#define MRW_CTRL_BATCH_H

#include <atomic>
#include <unordered_set>

namespace mrw::ctrl
{
	class BatchParticipant;

	class Batch
	{
		static std::atomic_uint32_t            counter;
		std::unordered_set<BatchParticipant *> transaction;

	protected:
		const uint32_t                         id;

	public:
		Batch();
		void reset();
		void increase(BatchParticipant * element);
		void decrease(BatchParticipant * element);
		bool contains(BatchParticipant * element);
		void tryComplete();
		bool isCompleted();
		void dump();

		virtual void completed() const = 0;
	};
}

#endif
