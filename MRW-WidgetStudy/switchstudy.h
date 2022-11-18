//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef SWITCHSTUDY_H
#define SWITCHSTUDY_H

#include <QWidget>

#include <ctrl/regularswitchcontrollermock.h>

namespace Ui
{
	class SwitchStudy;
}

class SwitchStudy : public QWidget
{
	Q_OBJECT

public:
	explicit SwitchStudy(QWidget * parent = nullptr);
	~SwitchStudy();

protected:
	void changeEvent(QEvent * e) override;

private:
	Ui::SwitchStudy            *           ui;

	mrw::ctrl::RegularSwitchControllerMock mock;
};

#endif // SWITCHSTUDY_H
