//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef RAILSTUDY_H
#define RAILSTUDY_H

#include <QWidget>

#include <ctrl/railcontrollermock.h>

namespace Ui
{
	class RailStudy;
}

class RailStudy : public QWidget
{
	Q_OBJECT

public:
	explicit RailStudy(QWidget * parent = nullptr);
	~RailStudy();

protected:
	void changeEvent(QEvent * e) override;

private slots:
	void updateLines(const int new_ext_lines);

private:
	static void resize(QWidget * widget, const float rel);

	Ui::RailStudy          *         ui;
	mrw::ctrl::RailControllerMock    mock;
};

#endif // SECTIONSTUDY_H
