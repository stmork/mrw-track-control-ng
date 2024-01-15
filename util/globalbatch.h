//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_GLOBALBATCH_H
#define MRW_UTIL_GLOBALBATCH_H

#include <QObject>

#include <util/singleton.h>
#include <util/batch.h>

namespace mrw::util
{
	/**
	 * This singleton class provides a global Batch for fallback purposes. This
	 * may be used for initialization purposes or clean shutdown of systems.
	 */
	class GlobalBatch :
		public QObject,
		public Batch,
		public Singleton<GlobalBatch>
	{
		Q_OBJECT

	private:
		GlobalBatch();
		virtual ~GlobalBatch();

		friend class Singleton<GlobalBatch>;

	signals:
		// Implementation of mrw::util::Batch
		void completed() override;
	};
}

#endif
