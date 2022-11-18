//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef REGIONFORM_H
#define REGIONFORM_H

#include <vector>

#include <QWidget>

#include <model/region.h>
#include <model/signal.h>
#include <model/section.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

namespace Ui
{
	class RegionForm;
}

class RegionForm : public QWidget
{
	Q_OBJECT

private:
	std::vector<mrw::model::RegularSwitch *>     region_reg_switches;
	std::vector<mrw::model::DoubleCrossSwitch *> region_dc_switches;

public:
	explicit RegionForm(mrw::model::Region * region, QWidget * parent = nullptr);
	~RegionForm();

protected:
	void changeEvent(QEvent * e) override;

private:
	void setupSection(mrw::model::Section * section);
	void setupSignals(mrw::model::Section * section, const bool direction);

	Ui::RegionForm * ui;
	static const QPalette    background_color;
};

#endif // REGIONFORM_H
