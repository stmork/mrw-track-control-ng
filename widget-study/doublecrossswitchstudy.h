//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef DOUBLECROSSSWITCHSTUDY_H
#define DOUBLECROSSSWITCHSTUDY_H

#include <mock/doublecrossswitchcontrollermock.h>

#include "widgetsaver.h"

namespace Ui
{
	class DoubleCrossSwitchStudy;
}

class DoubleCrossSwitchStudy : public WidgetSaver
{
	Q_OBJECT

public:
	explicit DoubleCrossSwitchStudy(QWidget * parent = nullptr);
	~DoubleCrossSwitchStudy();

protected:
	void changeEvent(QEvent * e) override;
	QWidget * widget() const override;
	QString   name() const override;

private:
	Ui::DoubleCrossSwitchStudy        *        ui;

	mrw::ctrl::DoubleCrossSwitchControllerMock mock;

	static QString state(const mrw::model::DoubleCrossSwitch::State & state);
};

#endif
