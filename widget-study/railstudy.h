//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef RAILSTUDY_H
#define RAILSTUDY_H

#include <mock/railcontrollermock.h>

#include "widgetsaver.h"

namespace Ui
{
	class RailStudy;
}

class RailStudy : public WidgetSaver
{
	Q_OBJECT

public:
	explicit RailStudy(QWidget * parent = nullptr);
	~RailStudy();

protected:
	void changeEvent(QEvent * e) override;
	QWidget * widget() const override;
	QString   name() const override;

private slots:
	void updateLines(const int new_ext_lines);

private:
	static void resize(QWidget * widget, const float rel);

	Ui::RailStudy          *         ui;
	mrw::ctrl::RailControllerMock    mock;

	static QString code(const bool a_end, const bool b_end);
	static QString code(const mrw::model::Position::Bending bending);
};

#endif
