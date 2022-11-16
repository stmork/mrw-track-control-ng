//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SIGNALWIDGET_H
#define MRW_UI_SIGNALWIDGET_H

#include <QWidget>

namespace mrw::ui
{
	class SignalWidget : public QWidget
	{
		Q_OBJECT

	private:
		bool direction      = true;
		bool shunting       = false;
		bool distant_signal = false;
		bool main_signal    = true;

	public:
		explicit SignalWidget(QWidget * parent = nullptr);

		void setDirection(const bool dir = true);
		void setShunting(const bool shunt = true);
		void setDistant(const bool distant = true);
		void setMain(const bool main = true);

	protected:
		void paintEvent(QPaintEvent * event) override;
		bool isDirection() const;
		bool hasShunting() const;
		bool hasDistant() const;
		bool hasMain() const;
	};
}

#endif
