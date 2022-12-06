//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef WIDGETROUTE_H
#define WIDGETROUTE_H

#include <QObject>
#include <QListWidgetItem>

#include <model/section.h>
#include <model/route.h>

class WidgetRoute : public mrw::model::Route
{
	Q_OBJECT

private:
	QListWidgetItem list_item;

public:
	static const int        USER_ROLE = Qt::UserRole + 1;

	explicit WidgetRoute(
		const bool                       dir,
		const mrw::model::SectionState   wanted_state,
		mrw::model::RailPart      *      first,
		QObject             *            parent = nullptr);

	operator QListWidgetItem * ();
};

#endif // WIDGETROUTE_H
