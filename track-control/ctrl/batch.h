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

	/**
	 * This class collects participants doing a job with later completion.
	 * Any BatchParticipant calls increase() when starting its the job and
	 * decrease() on completion. If all jobs are completed and the transaction
	 * set is empty the completed() signal is emitted.
	 *
	 * @note If nothing is to do you have to call tryComplete() on your own
	 * since the completed() signal is not emitted.
	 *
	 * @note It is possible to have multiple Batch instances simulatanously
	 * working but a BatchParticipant can only belong to one Batch.
	 */
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

		/**
		 * Clears the transaction set for reusing this transaction.
		 */
		void reset();

		/**
		 * The given BatchParticipant starts a job and is collected into the
		 * transaction set.
		 *
		 * @param element The working BatchParticipant.
		 */
		void increase(BatchParticipant * element);

		/**
		 * The given BatchParticipant has completed his job and is removed from
		 * the transaction set. If the transaction set is empty the complete
		 * Batch is completed and the completed() signal is emitted.
		 *
		 * @param element The completed BatchParticipant.
		 */
		void decrease(BatchParticipant * element);

		/**
		 * This method checks whether a BatchParticipant is active.
		 *
		 * @note The developer is responsible for timeout management which does
		 * not take place here.
		 *
		 * @param element The BatchParticipant to ask for an active job.
		 * @return True if the BatchParticipant is active.
		 */
		bool contains(BatchParticipant * element);

		/**
		 * This method checks whether the transaction set is empty. This may
		 * happen if nothing is to do. On the other hand pending jobs are not
		 * collected using the increase() method, yet. For this reason the
		 * method calls the QCoreApplication::processEvents() method to give
		 * queued events the chance to collect jobs later. If everything is
		 * done the method behaves like a finished decrease() method.
		 */
		void tryComplete();

		/**
		 * This method returns true if no BatchPrticipant has registered a
		 * job.
		 *
		 * @return True if everything is done.
		 */
		bool isCompleted();

		/**
		 * This method dumps all BatchParticipant elements collected in the
		 * transaction set.
		 */
		void dump();

		/**
		 * This is the signal which is emitted on job completion.
		 *
		 * @note This is not marked as a signal since a derived class may also
		 * be a QObject class which preventes double heritage of QObject.
		 */
		virtual void completed() = 0;
	};
}

#endif
