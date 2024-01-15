//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef SWITCHSTUDY_H
#define SWITCHSTUDY_H

#include <mock/regularswitchcontrollermock.h>

#include "widgetsaver.h"

namespace Ui
{
	class SwitchStudy;
}

class SwitchStudy : public WidgetSaver
{
	Q_OBJECT

public:
	explicit SwitchStudy(QWidget * parent = nullptr);
	~SwitchStudy();

protected:
	void changeEvent(QEvent * e) override;
	QWidget * widget() const override;
	QString   name() const override;

private:
	Ui::SwitchStudy            *           ui;

	mrw::ctrl::RegularSwitchControllerMock mock;

	static QString state(const bool is_left);
};

#endif
