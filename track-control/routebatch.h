//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef ROUTEBATCH_H
#define ROUTEBATCH_H

#include <QObject>
#include <QDebug>

#include <ctrl/batch.h>

class RouteBatch : public QObject, public mrw::ctrl::Batch
{
	Q_OBJECT

public:
	explicit RouteBatch(QObject * parent = nullptr);

	virtual ~RouteBatch();

	inline std::size_t operator()(const RouteBatch * batch) const noexcept
	{
		return batch->id;
	}

signals:
	void completed() const override;
};

#endif
