//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_POSITION_H
#define MRW_MODEL_POSITION_H

#include <QPoint>
#include <QSettings>

namespace mrw::model
{
	/**
	 * This class stores the position of the controlling widget onto the GUI.
	 * It can read coordinates from mrw::util::Settings and can store back into
	 * the same mrw::util::Settings. The coordiates are comma seperasted. A
	 * third field may contain additional behavior.
	 *
	 * The coordinates uses fractional units. The smallest unit is one
	 * Position::FRACTION. The logical height is always Position::FRACTION in
	 * height. A normal widget is intended to be quadratical. So the width
	 * ist normally also in Position::FRACTION units. For long rails it is
	 * possible to extend the witdth.
	 *
	 * When you have the position (3,5) stored in mrw::util::Settings the
	 * coordinates are (12,20) in Position::FRACTION units.
	 *
	 * When drawing the coordinates are scaled into a coordinate system of
	 * Position::SCALE units. The coordinete (0,0) is centered into a
	 * squared widget with (-SCALE, -SCALE) as the upper left corner and
	 * (SCALE, SCALE) as the lower right corner. So its easier to rotate
	 * the coordinate system in case the counting direction is inverted.
	 *
	 * The third field are command sequences to fine tune the widgets
	 * behaviour.
	 * 1. Any 'q' or 'Q' character shifts the widget to the right by one
	 * quarter size. Many occurences leads into multiple shifts.
	 * 1. Any 'h', 'o' or 'O' character shifts the widget to the right by two
	 * quarter size. Many occurences leads into multiple shifts.
	 * 2. Any 'x' or 'X' enlarges the controlling widget by a half standard
	 * widget size. Many occurences leads into the multiple of them.
	 * 3. A 'i' or 'I' draws a RegularSwitch as inclined.
	 * 4. A 'r' or 'R' draws a RegularSwitch right handed. This may differ to a
	 * real switch.
	 * 5. A 'l' or 'L' draws a RegularSwitch left handed. This may differ to a
	 * real switch.
	 * 6. A 'v' or 'V' extends a corresponding widget vertically which are
	 * always four Position::FRACTION units.
	 *
	 * @see mrw::ui::BaseWidget
	 */
	class Position
	{
	public:
		/**
		 * Bending is a special drawing of widgets where the symbolic rail
		 * swaps into another neighboured widget row. The values are related
		 * to the counting direction of the displayed component.
		 */
		enum class Bending
		{
			LEFT,      ///< Bend rail to left.
			STRAIGHT,  ///< Do not bend anyway.
			RIGHT      ///< Bend rail to right.
		};

		/** The relative fractional count of a quadratic controlled widget. */

		static constexpr unsigned FRACTION = 4;

		/** The mask to compute fractional part of Position::FRACTION width. */
		static constexpr unsigned MASK     = FRACTION - 1;

		/** The fractional units of the half controlled widget. */
		static constexpr unsigned HALF     = FRACTION >> 1;

		/** The fractional units of the quarter of controlled widget. */
		static constexpr unsigned QUARTER  = FRACTION >> 2;

		/** The widgets per line if unknown coordinates. */
		static constexpr unsigned NEW_LINE = 20;

		Position() = default;

		/**
		 * This method reads a value from the given mrw::util::Settings using
		 * the key() method as key and in case the value is not available it
		 * uses the default_value parameter instead. The read value is parsed
		 * as numerical coordinates. If the coordinates are not parseable a
		 * default post increment counter is used to locate an alternative
		 * position. The coordinates may extended by codes to produce a half
		 * shift in position and/or multiple half extends in size.
		 *
		 * @note The Settings use the "positions" group for storage.
		 *
		 * Example: 12,5,ooxxx leading in one offset shift and additional 3 half
		 * extends in size.
		 *
		 * @param settings The settings to use.
		 * @param default_value The default value to use.
		 */
		void parse(QSettings & settings, const QString & default_value);

		/**
		 * This method writes the coordinates into the given
		 * mrw::util::Settings using the key() method as key. The coordinates
		 * are extended by codes to produce a half shift in position and/or
		 * multiple half extends in size.
		 *
		 * @param settings The settings to use.
		 */
		void write(QSettings & settings);

		/**
		 * This method moves the controlled widgets in Position::FRACTION units.
		 * The given coordinates are deltas which allows negative moving.
		 *
		 * @param right The horizontal delta.
		 * @param down The vertical delta.
		 */
		void move(const int right, const int down);

		/**
		 * This sets the controlled widget to an absolut horizontal position
		 * in Position::FRACTION units.
		 *
		 * @param x The absolut x position in Position::FRACTION units.
		 */
		void setX(const int x);

		/**
		 * This getter returns the half extension unit of the controlling widget
		 * size.
		 *
		 * @return The extension size in half controlling widget size.
		 */
		unsigned extension() const;

		/**
		 * This method returns the number of <em>extended</em> line counts. One
		 * line count is the full height of the controlled widget representing
		 * four Position::FRACTION units.
		 *
		 * @return The multiple height of the widget.
		 */
		unsigned lines() const;

		/**
		 * This method inverts the inclination flag.
		 *
		 * @see isInclined()
		 */
		void toggleInclination();

		/**
		 * This method extends a rail drawing away from quadratic rendering.
		 * Negative increments are allowed but the the resulting extends does
		 * not underflow zero.
		 *
		 * @param inc The increment to add to the extension.
		 */
		void extend(const int inc = 1);

		/**
		 * This method extends a rail drawing away from quadratic rendering.
		 * Negative increments are allowed but the the resulting extends does
		 * not underflow zero.
		 *
		 * @param inc The increment to add to the extension.
		 */
		void lineup(const int inc = 1);

		/**
		 * This method returns true if the drawing of a RegularSwitch is
		 * inclined.
		 *
		 * Example of a left handed non inclined switch:
		 *
		 * <img src="RSwitch_AB_nL_RUF.jpg" width="100"/>
		 *
		 * Example of a left handed inclined switch:
		 *
		 * <img src="RSwitch_AB_IL_RUF.jpg" width="100"/>
		 *
		 * Example of a right handed inclined switch:
		 *
		 * <img src="RSwitch_AB_IR_RUF.jpg" width="100"/>
		 *
		 * @note Only a RegularSwitch may be inclined.
		 *
		 * @return True if the drawing of the RegularSwitch is
		 * inclined.
		 */
		bool isInclined() const;

		/**
		 * This method returns the position. The real pixel position must be
		 * multiplied by half controlling widget standard size. It contains
		 * the optional horizontal offset.
		 *
		 * @return The widget position.
		 */
		const QPoint & point() const;

		/**
		 * This method returns a key to determine the logical coordinates from
		 * the description file.
		 *
		 * @return The key for looking up the logical coordinates
		 */
		virtual QString key() const = 0;

		/**
		 * This method returns the width of the controlled widget in
		 * Position::FRACTION units.
		 *
		 * @return The width of the controlled widget in Position::FRACTION
		 * units.
		 */
		int width() const;

		/**
		 * This method returns the height of the controlled widget in
		 * Position::FRACTION units.
		 *
		 * @return The height of the controlled widget in Position::FRACTION
		 * units.
		 */
		int height() const;

		/**
		 * This method compares this Position instance against to another
		 * Position instance against equality.
		 *
		 * @param other The other Position instance to compare to.
		 * @return True if both instances have the same value.
		 */
		bool operator==(const Position & other) const;

		/**
		 * This method compares this Position instance against to another
		 * Position instance against unequality.
		 *
		 * @param other The other Position instance to compare to.
		 * @return True if both instances have different value.
		 */
		bool operator != (const Position & other) const;

		/**
		 * This method compares this Position instance and another Position
		 * instance positions according to their coordinates. The vertical
		 * component has more priority than the horizontal one.
		 *
		 * @param other The other Position instance to compare to.
		 * @return True if this Position is lower than the other Position.
		 */
		bool operator<(const Position & other) const;

		/**
		 * This method compares two positions according to their coordinates.
		 * The vertical component has more priority than the horizontal one.
		 * This method can be used to sort a collection using std::sort().
		 *
		 * @code
		 * std::vector<Position *> vector;
		 *
		 * std::sort(vector.begin(), vector.end(), &Position::compare);
		 * @endcode
		 *
		 * @param left The lower Position.
		 * @param right The upper Position.
		 * @return True if left is lower than the right Position.
		 */
		static bool less(const Position * left, const Position * right);

		/**
		 * This returns the actual bending behaviour.
		 *
		 * @note This is only implemented for mrw::ui::SignalWidget and
		 * mrw::ui::RailWidget.
		 *
		 * @return The actual Bending.
		 */
		Bending bending() const;

		/**
		 * This sets the bending behaviour.
		 *
		 * @note This is only implemented for mrw::ui::SignalWidget and
		 * mrw::ui::RailWidget.
		 *
		 * @param input
		 */
		void setBending(const Bending input);

	protected:
		/**
		 * This method parses a position and appearance value for this
		 * instance.
		 *
		 * @param value The value from the QSettings to parse.
		 */
		void    parse(const QString & value);

		/**
		 * This method converts the position and appearance information into
		 * a QString which can be stored using QSettings.
		 *
		 * @return The encoded Position information.
		 */
		QString value() const;

		/**
		 * The increasing counter for each Position in case no initial
		 * coordinates were configured.
		 */
		static unsigned counter;

	private:
		QPoint   position;
		unsigned ext_count     = 0;
		unsigned line_count    = 0;
		bool     inclined      = false;
		Bending  bending_state = Bending::STRAIGHT;
	};
}

#endif
