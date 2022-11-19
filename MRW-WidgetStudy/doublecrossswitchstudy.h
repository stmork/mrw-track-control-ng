//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef DOUBLECROSSSWITCHSTUDY_H
#define DOUBLECROSSSWITCHSTUDY_H

#include <QWidget>

#include <ctrl/doublecrossswitchcontrollermock.h>

namespace Ui
{
	class DoubleCrossSwitchStudy;
}

class DoubleCrossSwitchStudy : public QWidget
{
	Q_OBJECT

public:
	explicit DoubleCrossSwitchStudy(QWidget * parent = nullptr);
	~DoubleCrossSwitchStudy();

protected:
	void changeEvent(QEvent * e) override;

private:
	Ui::DoubleCrossSwitchStudy *               ui;

	mrw::ctrl::DoubleCrossSwitchControllerMock mock;
};

#endif // DOUBLECROSSSWITCHSTUDY_H
