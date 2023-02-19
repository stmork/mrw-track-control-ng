//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_BATCH_H
#define MRW_UTIL_BATCH_H

#include <atomic>
#include <unordered_set>

namespace mrw::util
{
	class BatchParticipant;

	class Batch
	{
		static std::atomic_uint32_t            counter;
	protected:
		/** The open participants. */
		std::unordered_set<BatchParticipant *> transaction;

		/** The transaction ID. */
		const uint32_t                         id;

	public:
		Batch();
		virtual ~Batch() = default;

		void reset();
		void increase(BatchParticipant * element);
		void decrease(BatchParticipant * element);
		bool contains(BatchParticipant * element);
		void tryComplete();
		bool isCompleted();
		void dump();

		virtual void completed() = 0;
	};
}

#endif
