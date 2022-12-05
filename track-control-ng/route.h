//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef ROUTE_H
#define ROUTE_H

#include <list>

#include <QObject>
#include <QListWidgetItem>

#include <model/section.h>
#include <model/railpart.h>

class Route : public QObject
{
	Q_OBJECT

private:
	const bool                          direction;
	const mrw::model::SectionState      state = mrw::model::SHUNTING;
	std::list<mrw::model::RailPart *>   track;
	mrw::model::Section        *        first_section = nullptr;
	QListWidgetItem                     list_item;

public:
	static const int                    USER_ROLE = Qt::UserRole + 1;

	explicit Route(
		const bool                       dir,
		const mrw::model::SectionState   wanted_state,
		mrw::model::RailPart      *      first,
		QObject             *            parent = nullptr);

	operator QListWidgetItem * ();

	bool extend(mrw::model::RailPart * rail);
	void prepare();
};

#endif // ROUTE_H
