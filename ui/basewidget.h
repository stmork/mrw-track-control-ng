//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_BASEWIDGET_H
#define MRW_UI_BASEWIDGET_H

#include <QWidget>
#include <QPainter>

namespace mrw::ui
{
	class BaseWidget : public QWidget
	{
		Q_OBJECT

	public:
		static const int SIZE = 40;

		explicit BaseWidget(QWidget * parent = nullptr);

	protected:
		virtual void paintEvent(QPaintEvent * event) override;
		virtual void paint(QPainter & painter) = 0;

		void rescale(
				QPainter & painter,
				const float xSize, const float ySize,
				const float xPos,
				const float yPos);

		void rescale(
				QPainter & painter,
				const float xSize, const float ySize, const bool center = true);
	};
}

#endif
