//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CONTROLLERWIDGET_H
#define MRW_UI_CONTROLLERWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	class ControllerWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit ControllerWidget(QWidget * parent = nullptr);

	protected:
		bool drawLock(const mrw::model::Device::LockState state) const;
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
		unsigned counter = 0;
	};
}

#endif
