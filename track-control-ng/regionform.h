//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef REGIONFORM_H
#define REGIONFORM_H

#include <QWidget>

#include <model/region.h>

namespace Ui
{
	class RegionForm;
}

class RegionForm : public QWidget
{
	Q_OBJECT

public:
	explicit RegionForm(mrw::model::Region * region, QWidget * parent = nullptr);
	~RegionForm();

protected:
	void changeEvent(QEvent * e) override;

private:
	Ui::RegionForm * ui;
	static const QPalette    background_color;
};

#endif // REGIONFORM_H
