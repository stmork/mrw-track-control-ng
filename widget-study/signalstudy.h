//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef SIGNALSTUDY_H
#define SIGNALSTUDY_H

#include <mock/signalcontrollermock.h>

#include "widgetsaver.h"

namespace Ui
{
	class SignalStudy;
}

class SignalStudy : public WidgetSaver
{
	Q_OBJECT

public:
	explicit SignalStudy(QWidget * parent = nullptr);
	~SignalStudy();

protected:
	void changeEvent(QEvent * e) override;
	QWidget * widget() const override;
	QString   name() const override;

private:
	Ui::SignalStudy        *        ui;

	mrw::ctrl::SignalControllerMock mock;

	static QString code(const bool valid, const mrw::model::Signal::Symbol & symbol);
};

#endif
