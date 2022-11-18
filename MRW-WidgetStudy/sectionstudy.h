//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef SECTIONSTUDY_H
#define SECTIONSTUDY_H

#include <QWidget>

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
	Ui::SectionStudy * ui;
};

#endif // SECTIONSTUDY_H
