//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_BATCHPARTICIPANT_H
#define MRW_UTIL_BATCHPARTICIPANT_H

#include <QString>

namespace mrw::util
{
	class Batch;

	/**
	 * This class acts as a job registrand to a Batch processing registry. By
	 * default the GlobalBatch acts as a Batch. It is also possible to use
	 * another Batch. It is not allowed to switch a Batch during job processing
	 * which takes place between the increase() and decrease() call.
	 */
	class BatchParticipant
	{
		Batch * base_tx = nullptr;
		Batch * open_tx = nullptr;

	public:
		BatchParticipant();
		virtual ~BatchParticipant();

		/**
		 * This marks a job start at the previously configured Batch. Until
		 * calling decrease() a Batch change is not allowed.
		 *
		 * @note A batch may be reused in the same transaction.
		 *
		 * @return True on success.
		 */
		bool increase() noexcept;

		/**
		 * This marks a findished job at the previoulsy configured Batch.
		 * Changing a batch is now allowed again.
		 *
		 * @note A batch may be reused in the same transaction.
		 *
		 * @return True on success.
		 */
		bool decrease() noexcept;

		/**
		 * This method returns the actually configured Batch.
		 *
		 * @return The configured batch.
		 */
		[[nodiscard]]
		Batch * batch() const noexcept;

		/**
		 * This method configures a Batch to be used by the methods increase()
		 * and decrease(). If the parameter is @c nullptr the GlobalBatch is
		 * being configured.
		 *
		 * @param batch The Batch to use.
		 * @see GlobalBatch
		 */
		void setBatch(Batch * batch) noexcept;

		/**
		 * This method gives the chance for dumping a clear text for debugging
		 * purposes.
		 *
		 * @return The clear text of this BatchParticipant.
		 */
		virtual const QString & name() const noexcept = 0;
	};
}

#endif
