//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef SIGNALSTUDY_H
#define SIGNALSTUDY_H

#include <QWidget>

#include <ctrl/signalcontrollermock.h>

namespace Ui
{
	class SignalStudy;
}

class SignalStudy : public QWidget
{
	Q_OBJECT

public:
	explicit SignalStudy(QWidget * parent = nullptr);
	~SignalStudy();

protected:
	void changeEvent(QEvent * e) override;

private:
	Ui::SignalStudy        *        ui;

	mrw::ctrl::SignalControllerMock mock;
};

#endif // SIGNALSTUDY_H
