//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_BASEWIDGET_H
#define MRW_UI_BASEWIDGET_H

#include <unordered_map>

#include <QWidget>
#include <QPainter>
#include <QColor>

#include <model/section.h>

namespace mrw::ui
{
	class BaseWidget : public QWidget
	{
		Q_OBJECT

	public:
		static constexpr int   SIZE  = 40;
		static constexpr float SCALE = 100.0f;
		static constexpr int   FONT_HEIGHT = 50;

		static const QColor GREEN;
		static const QColor WHITE;
		static const QColor RED;
		static const QColor YELLOW;
		static const QColor BLUE;
		static const QColor RED_LIGHT;

		explicit BaseWidget(QWidget * parent = nullptr);

	protected:
		virtual void paintEvent(QPaintEvent * event) override;
		virtual void paint(QPainter & painter) = 0;

		static QColor sectionColor(const mrw::model::SectionState state);

		void rescale(
			QPainter & painter,
			const float xSize, const float ySize,
			const float xPos,
			const float yPos);

		void rescale(
			QPainter & painter,
			const float xSize = SCALE * 2.0,
			const float ySize = SCALE * 2.0, const bool center = true);

	private:
		static const std::unordered_map<mrw::model::SectionState, QColor> color_map;

		const bool verbose;
	};
}

#endif
