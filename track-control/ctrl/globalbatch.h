//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_GLOBALBATCH_H
#define MRW_CTRL_GLOBALBATCH_H

#include <QObject>

#include <util/singleton.h>
#include <ctrl/batch.h>

namespace mrw::ctrl
{
	class GlobalBatch :
		public QObject,
		public Batch,
		public mrw::util::Singleton<GlobalBatch>
	{
		Q_OBJECT

	private:
		GlobalBatch();
		virtual ~GlobalBatch();

		friend class Singleton<GlobalBatch>;

	signals:
		// Implementation of mrw::ctrl::Batch
		void completed() override;
	};
}

#endif
