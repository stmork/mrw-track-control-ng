//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

/**
 * The mrw::ui namespace contains all custom widget classes needed for the
 * MRW next generation track control. All widgets are derived from the
 * BaseWidget class. Special classes like the BarWidget or OpModeWidget are
 * directly derived.
 *
 * All classes derived from ControllerWidget are view classes in the meaning of
 * the MVC paradigma. The model contains simply the data. The view represents
 * simply this data. Both don't talk directly but using an intermediate
 * controller which contains a sort of business logic. Generally speaking the
 * "view" BaseWidget does not see any model class.
 *
 * The model may be any class from the mrw::model namespace, The view classes
 * are in the mrw::ui namespace and the controller classes are in this
 * mrw::ctrl namespace.
 *
 * <img src="MVC-diagram.png"/>
 *
 * Actually there are four MVC groups:
 * 1. mrw::model::Rail - mrw::ctrl::RailController - RailWidget
 * 2. mrw::model::RegularSwitch - mrw::ctrl::RegularSwitchController - RegularSwitchWidget
 * 3. mrw::model::DoubleCrossSwitch - mrw::ctrl::DoubleCrossSwitchController - DoubleCrossSwitchWidget
 * 4. mrw::model::Signal - mrw::ctrl::SignalController - SignalWidget
 *
 * @note The MVC for signals is a little bit different because a controller
 * may represent a group of signals like a combined main and distant signal.
 *
 * @note The Section elements are not represented in a MVC manner.
 *
 * @see https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller
 */
namespace mrw::ui
{
	/**
	 * The BaseWidget gives basic funtionallity to all derived Widget classes.
	 * All derived widgets have a unified coordinate system indepent of the
	 * real pixel size. A normal quadratic widget has the horizontal and
	 * vertical coordinate range of [-100..100]. Except from the SignalWidget
	 * and the RailWidget the widgets may have horizontal oversize - the
	 * so called extension. The absolute maximum of 100 is stored in the
	 * constant BaseWidget::SCALE.
	 */
	class BaseWidget : public QWidget
	{
		Q_OBJECT

	public:
		/** The recommended pixel size for a widget. */
		static constexpr int    SIZE        =  40;

		/**
		 * The transformed half width representing mrw::model::Position::HALF
		 * units.
		 */
		static constexpr float  SCALE       = 100.0f;

		/** The font height inclusive descender. */
		static constexpr int    FONT_HEIGHT =  55;

		/** The font size used for QFont::setSize(). */
		static constexpr int    FONT_SIZE   = FONT_HEIGHT * 0.8;

		/** The width of a drawn rail. */
		static constexpr float  RAIL_WIDTH  =  20.0f;

		/** The inclination angle for branches and rails. */
		static constexpr float  RAIL_SLOPE  =   2.0f;

		/** The width of a lock to be drawn. */
		static constexpr int    LOCK_WIDTH  =  40;

		/** The height of a lock to be drawn. */
		static constexpr int    LOCK_HEIGHT =  22;

		static const     QColor GREEN;
		static const     QColor WHITE;
		static const     QColor RED;
		static const     QColor YELLOW;
		static const     QColor BLUE;
		static const     QColor RED_LIGHT;

		explicit BaseWidget(QWidget * parent = nullptr);

		static void setVerbose(const bool activate = false);

	protected:

		/**
		 * The overloaded paintEvent method  setup the painter and after that
		 * calls the paint() method. In case the verbose mode is activated the
		 * widget border is painted light grey for user orintation while
		 * editing.
		 *
		 * @param event The paint event.
		 */
		virtual void paintEvent(QPaintEvent * event) override;

		/**
		 * This method must be overloaded to draw its content. The given
		 * painter is already initialized by the paintEvent() callback.
		 *
		 * @note You are yourself responsible to initialize coordinate
		 * transformation by calling the rescale() methods.
		 *
		 * @param painter The QPainter to draw into.
		 */
		virtual void paint(QPainter & painter) = 0;

		/**
		 * This method returns true if a pending state has to be displayed.
		 * Mostly some bliking elements has to be drawn.
		 *
		 * @return True if there is a pending state to be drawn.
		 */
		virtual bool hasLock() const;

		/**
		 * This method returns the color corresponding to a given
		 * mrw::model::SectionState.
		 *
		 * @param state The mrw::model::SectionState to convert into color.
		 * @return The resulting color.
		 */
		static QColor sectionColor(const mrw::model::SectionState state);

		/**
		 * This method computes a more general coordinate transformation where
		 * the origin of coordinates is free moveable. The first transformation
		 * is a translation to the given xPos and yPos coordinate center. After
		 * that scaling takes place.
		 *
		 * @note The wanted width and height is the target size whereas the
		 * position is in original pixel coordinates.
		 *
		 * @param painter The QPainter to assign the coordinate transformation to.
		 * @param xSize The wanted unified width. You need 2 * BaseWidget::SCALE
		 * if you want the range of [-100..100].
		 * @param ySize The wanted unified height. You need 2 *
		 * BaseWidget::SCALE if you want the range of [-100..100].
		 * @param xPos The horizontal origin of coordinates in pixels.
		 * @param yPos The vertical origin of coordinates in pixels.
		 *
		 * @see https://doc.qt.io/qt-6/qtwidgets-painting-transformations-example.html
		 */
		void rescale(
			QPainter  & painter,
			const float xSize, const float ySize,
			const float xPos,
			const float yPos);

		/**
		 * This method computes a coordinate transformation for uniques
		 * widget rendering. Calling this method using its default values
		 * computes a coordinate transformation with horizontal and
		 * vertical coordinate range of [-100..100].
		 *
		 * @param painter The QPainter to assign the coordinate transformation to.
		 * @param xSize The wanted unified width. You need 2 * BaseWidget::SCALE
		 * if you want the range of [-100..100].
		 * @param ySize The wanted unified height. You need 2 *
		 * BaseWidget::SCALE if you want the range of [-100..100].
		 * @param center True if the origin of coordinates is in the center of
		 * the widget and false if the upper left corner is the origin.
		 *
		 * @see https://doc.qt.io/qt-6/qtwidgets-painting-transformations-example.html
		 */
		void rescale(
			QPainter  & painter,
			const float xSize = SCALE * 2.0,
			const float ySize = SCALE * 2.0, const bool center = true);

		/**
		 * This flag hints a border drawing for orientation purposes while
		 * editing the widgets.
		 *
		 * @see setVerbose()
		 */
		static bool verbose;

	private:
		static const std::unordered_map<mrw::model::SectionState, QColor> color_map;

	};
}

#endif
