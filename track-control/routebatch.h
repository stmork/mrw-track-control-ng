//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef ROUTEBATCH_H
#define ROUTEBATCH_H

#include <QObject>
#include <QDebug>

#include <util/batch.h>

class RouteBatch : public QObject, public mrw::util::Batch
{
	Q_OBJECT

	size_t count = 0;
	const size_t limit = 1;
	bool auto_unblock = false;

public:
	explicit RouteBatch(
		const bool  has_signal,
		const bool  is_auto_unblock,
		QObject  *  parent = nullptr);

	virtual ~RouteBatch();

	inline std::size_t operator()(const RouteBatch * batch) const noexcept
	{
		return batch->id;
	}

signals:
	void unlock();
	void tryUnblock();

private:
	void completed() override;
};

#endif
