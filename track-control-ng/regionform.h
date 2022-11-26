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
#include <ctrl/sectioncontroller.h>

namespace Ui
{
	class RegionForm;
}

class RegionForm : public QWidget
{
	Q_OBJECT

private:
	mrw::model::Region             *             form_region;

	QSize fields;

public:
	explicit RegionForm(mrw::model::Region * region, QWidget * parent = nullptr);
	~RegionForm();

	inline mrw::model::Region * region() const
	{
		return form_region;
	}

	void line(std::vector<mrw::model::Position *> & positions, const int y) const;

protected:
	void changeEvent(QEvent * e) override;

private:
	void setupSize(mrw::model::Region * region);
	void setupRails(mrw::ctrl::SectionController * controller);
	void setupSignals(mrw::ctrl::SectionController * controller, const bool direction);
	void setupRegularSwitches(mrw::ctrl::SectionController * controller);
	void setupDoubleCrossSwitches(mrw::ctrl::SectionController * controller);

	Ui::RegionForm * ui;
	static const QPalette    background_color;
};

#endif // REGIONFORM_H
