//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef ROUTE_H
#define ROUTE_H

#include <QObject>

#include <model/railpart.h>

class Route : public QObject
{
	Q_OBJECT

private:
	bool direction;
	std::vector<mrw::model::RailPart *> track;

public:
	explicit Route(mrw::model::RailPart * first, bool dir, QObject * parent = nullptr);

	void extend(mrw::model::RailPart * rail);

signals:

};

#endif // ROUTE_H
