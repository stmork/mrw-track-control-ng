//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CONTROLLERWIDGET_H
#define MRW_UI_CONTROLLERWIDGET_H

#include <QListWidgetItem>

#include <ui/basewidget.h>

namespace mrw::ui
{
	class ControllerWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit ControllerWidget(
				QWidget * parent = nullptr,
				mrw::ctrl::BaseController * ctrl = nullptr);

		void setController(mrw::ctrl::BaseController * ctrl);

	signals:
		void clicked(QListWidgetItem * item);

	public slots:
		void extend();

	protected:
		virtual void mousePressEvent(QMouseEvent * event) override;

		bool lockVisible(const mrw::model::Device::LockState state) const;
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

		inline float extensions() const
		{
			return base_controller->extensions();
		}

		template<class C> C * controller() const
		{
			return dynamic_cast<C *>(base_controller);
		}

		mrw::ctrl::BaseController * base_controller = nullptr;

	private:
		unsigned        counter = 0;
		QListWidgetItem list_item;

	};
}

#endif
