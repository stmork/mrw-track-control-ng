//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BATCHPARTICIPANT_H
#define MRW_CTRL_BATCHPARTICIPANT_H

#include <QString>

namespace mrw::ctrl
{
	class Batch;

	class BatchParticipant
	{
		Batch * batch = nullptr;
		Batch * open_tx = nullptr;

	public:
		BatchParticipant();
		void increase();
		void decrease();
		void setBatch(Batch * batch);

		virtual QString name() const = 0;
	};
}

#endif
