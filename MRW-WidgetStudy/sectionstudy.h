//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef SECTIONSTUDY_H
#define SECTIONSTUDY_H

#include <QWidget>

#include <ctrl/sectioncontrollermock.h>

namespace Ui
{
	class SectionStudy;
}

class SectionStudy : public QWidget
{
	Q_OBJECT

public:
	explicit SectionStudy(QWidget * parent = nullptr);
	~SectionStudy();

protected:
	void changeEvent(QEvent * e) override;

private:
	Ui::SectionStudy        *        ui;

	mrw::ctrl::SectionControllerMock mock;
};

#endif // SECTIONSTUDY_H
