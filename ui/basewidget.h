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

#include <model/device.h>
#include <model/section.h>
#include <ctrl/basecontroller.h>

namespace mrw::ui
{
	class BaseWidget : public QWidget
	{
		Q_OBJECT

	public:
		static constexpr int   SIZE        =  40;
		static constexpr float SCALE       = 100.0f;
		static constexpr int   FONT_HEIGHT =  50;
		static constexpr float RAIL_WIDTH  =  20.0f;
		static constexpr float RAIL_SLOPE  =   2.0f;
		static constexpr int   LOCK_WIDTH  =  40;
		static constexpr int   LOCK_HEIGHT =  22;

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
		virtual bool isLockTransit() const
		{
			return false;
		}

		static QColor sectionColor(const mrw::model::SectionState state);
		bool drawLock(const mrw::model::Device::LockState state) const;

		void rescale(
			QPainter & painter,
			const float xSize, const float ySize,
			const float xPos,
			const float yPos);

		void rescale(
			QPainter & painter,
			const float xSize = SCALE * 2.0,
			const float ySize = SCALE * 2.0, const bool center = true);

		void drawLock(
			QPainter  & painter,
			QColor      color,
			const float x, const float y);
		void drawSheared(
			QPainter  & painter,
			QColor      color,
			const float x, const float y,
			const float height,
			const float slope = RAIL_SLOPE);

		void prepareFailed(
				QPainter  & painter,
				const bool  fail);

	private:
		static const std::unordered_map<mrw::model::SectionState, QColor> color_map;

		const bool verbose;
		unsigned counter = 0;
	};
}

#endif
