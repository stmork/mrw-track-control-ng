//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef WIDGETSTUDY_H
#define WIDGETSTUDY_H

#include "widgetsaver.h"

namespace Ui
{
	class WidgetStudy;
}

class WidgetStudy : public WidgetSaver
{
	Q_OBJECT

public:
	explicit WidgetStudy(QWidget * parent = nullptr);
	~WidgetStudy();

protected:
	void changeEvent(QEvent * e) override;

private slots:
	void on_stationEdit_textChanged(const QString & text);
	void on_opModeEdit_textChanged(const QString & text);
	void on_blinkChecker_clicked();

private:
	virtual void save() override;

	virtual QWidget * widget() const override;
	virtual QString   name() const override;

	Ui::WidgetStudy * ui;
};

#endif
