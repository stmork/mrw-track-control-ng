//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
		static const int   USER_ROLE = Qt::UserRole + 0;

		explicit ControllerWidget(
			QWidget          *          parent = nullptr,
			mrw::ctrl::BaseController * ctrl   = nullptr);

		void setController(mrw::ctrl::BaseController * ctrl);

		bool isConnector(const QPoint & point) const;

	signals:
		void clicked(QListWidgetItem * item);

	public slots:
		void reposition();
		void extend();

		virtual void computeConnectors()
		{
			connector_list.clear();
		}

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

		void drawConnectors(QPainter & painter);

		/**
		 * This method returns the extensions of the controlled
		 * mrw::model::AssemblyPart.
		 *
		 * @return The extension count of the controlled
		 * mrw::model::AssemblyPart.
		 * @see mrw::model::Position::extensions()
		 */
		inline float extensions() const
		{
			return base_controller->extensions();
		}

		/**
		 * This method returns the number of <em>extended</em> line counts. One
		 * line count is the full height of the controlled widget representing
		 * four mrw::model::Position::FRACTION units.
		 *
		 * @return The multiple height of the widget.
		 * @see mrw::model::Position::lines()
		 */
		inline float lines() const
		{
			return base_controller->lines();
		}

		template<class C> C * controller() const
		{
			return dynamic_cast<C *>(base_controller);
		}

		mrw::ctrl::BaseController * base_controller = nullptr;
		QList<QPoint>               connector_list;

	private:
		unsigned           counter = 0;
		QListWidgetItem    list_item;
	};
}

#endif
