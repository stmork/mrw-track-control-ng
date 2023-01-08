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
	/**
	 * This is the base class for all derived widget classes which follow the
	 * MVC paradigma. It provides the connection to the
	 * mrw::ctrl::BaseController class.
	 *
	 * This class provides support for a QListWidget supplying a
	 * QListWidgetInfo instance inside this instance.
	 */
	class ControllerWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		static const int   USER_ROLE = Qt::UserRole + 0;

		explicit ControllerWidget(
			QWidget          *          parent = nullptr,
			mrw::ctrl::BaseController * ctrl   = nullptr);

		/**
		 * This setter provides the corresponding mrw::ctrl::BaseController.
		 * This may nessecary if the controller is not known during widget
		 * construction. Since this widget provides support to be listed in
		 * a QListWidget the QListWidgetInfo class is also initialized.
		 *
		 * @param ctrl The mrw::ctrl::BaseController to set.
		 * @
		 */
		void setController(mrw::ctrl::BaseController * ctrl);

		/**
		 * This method returns true if the given point is equal to any
		 * contained connector.
		 *
		 * @param point The point in Position::FRACTION units.
		 * @return True if this BaseWidget contains the connection point.
		 */
		bool isConnector(const QPoint & point) const;

	signals:
		/**
		 * This signal is emitted if a mouse button was pressed down for
		 * selection.
		 *
		 * @param item The QListWidgetInfo for direct usage inside a
		 * QListWidget.
		 */
		void clicked(QListWidgetItem * item);

	public slots:
		/**
		 * The position in the model has changed and need to be redrawn.
		 */
		void reposition();

		/**
		 * The extension in the model has changed and need to be redrawn.
		 */
		void extend();

		/**
		 * The orientation of branches has changed and need to be recomputed
		 * and redrawn.
		 */
		virtual void computeConnectors();

	protected:
		virtual void mousePressEvent(QMouseEvent * event) override;

		/**
		 * This method shows the actual blinking state of a pending movement
		 * of the gadget to show. It depends on a timer, the state
		 * mrw::model::Device::LockState::PENDING. If the result is false the
		 * gadget do not need to be drawn.
		 *
		 * @param state The actual mrw::model::Device::LockState.
		 * @return True if the gadget has to be drawn.
		 */
		bool lockVisible(const mrw::model::Device::LockState state) const;

		/**
		 * This method draws a lock symbol with the specified color at the
		 * specified position.
		 *
		 * @param painter The QPainter to draw into.
		 * @param color The color to use.
		 * @param x The horizontal center position of the lock to draw.
		 * @param y The vertical center position of the lock to draw.
		 */
		void drawLock(
			QPainter  & painter,
			QColor      color,
			const float x, const float y);

		/**
		 * This method draws a sheared rectangle along a centerline from a
		 * starting point x and y with the given height and slope. The width
		 * of the sheared rectangle is RAIL_WIDTH in transformed coordinates.
		 * The given height is signed based with the vertical ascending order
		 * from top to down. So positive height values are from top to down.
		 * The slope defines the horizontal offset by default with the
		 * inclination of RAIL_SLOPE. Positive values moves the horizontal
		 * offset from right to left. To change the orientation you have to use
		 * a negative slope.
		 *
		 * @note For a unique appearance use only +RAIL_SLOPE or -RAIL_SLOPE
		 * for slope values.
		 *
		 * @param painter The QPainter to draw into.
		 * @param color The color to use.
		 * @param x The centered horizontal start position.
		 * @param y The centered vertical start position.
		 * @param height The signed based height of the sheared rectangle.
		 * @param slope The slope to compute the horizontal offset.
		 */
		void drawSheared(
			QPainter  & painter,
			QColor      color,
			const float x, const float y,
			const float height,
			const float slope = RAIL_SLOPE);

		/**
		 * This method prepares the text label to signal a failed state or a
		 * normal state to be drawn.
		 *
		 * @param painter The QPainter to draw the text.
		 * @param fail True if the failed state should be drawn or the normal
		 * state.
		 *
		 * @see mrw::ctrl::BaseController::name()
		 * @see mrw::model::SectionState::FAILED
		 */
		void prepareFailed(
			QPainter  & painter,
			const bool  fail);

		/**
		 * This method draws the branching connectors if the verbose flag is
		 * set to true.
		 *
		 * @param painter The QPainter to draw into.
		 */
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

		/**
		 * This method is a convenience template funtion to cast to the final
		 * controller type.
		 */
		template<class C> C * controller() const
		{
			return dynamic_cast<C *>(base_controller);
		}

		/**
		 * The corresponding mrw::ctrl::BaseController in the sense of the
		 * MVC paradigma.
		 */
		mrw::ctrl::BaseController * base_controller = nullptr;

		/**
		 * The list of possible connection points.
		 */
		QList<QPoint>               connector_list;

	private:
		unsigned           counter = 0;
		QListWidgetItem    list_item;
	};
}

#endif
