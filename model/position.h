//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
	 * 1. Any 'o' or 'O' character shifts the widget to the right by one quarter
	 * size. Many occurences leads into only one shift.
	 * 2. Any 'x' or 'X' enlarges the controlling widget by a half standard
	 * widget size. Many occurences leads into the multiple of them.
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

		static constexpr unsigned FRACTION = 4;
		static constexpr unsigned MASK     = FRACTION - 1;
		static constexpr unsigned HALF     = FRACTION >> 1;
		static constexpr unsigned QUARTER  = FRACTION >> 2;

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
		 * @example 12,5,ooxxx leading in one offset shift and additional 3 half
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

		void move(const int right, const int down);

		void setX(const int x);

		/**
		 * This getter returns the half extension unit of the controlling widget
		 * size.
		 *
		 * @return The extension size in half controlling widget size.
		 */
		inline unsigned extension() const
		{
			return ext_count;
		}

		inline void toggleInclination()
		{
			inclined = !inclined;
		}

		inline void extend(const int inc = 1)
		{
			const int result = ext_count + inc;

			ext_count = std::clamp(result, 0, 40);
		}

		inline bool isInclined() const
		{
			return inclined;
		}

		/**
		 * This method returns the position. The real pixel position must be
		 * multiplied by half controlling widget standard size. It contains
		 * the optional horizontal offset.
		 *
		 * @return The widget position.
		 */
		inline const QPoint & point() const
		{
			return position;
		}

		/**
		 * This method returns a key to determine the logical coordinates from
		 * the description file.
		 *
		 * @return The key for looking up the logical coordinates
		 */
		virtual QString key() const = 0;

		inline int width() const
		{
			return FRACTION + ext_count;
		}

		/**
		 * This method compares two positions according to their coordinates.
		 * The vertical component has more priority than the horizontal one.
		 * This method can be used to sort a collection using std::sort().
		 *
		 * @code
		 * std::vector<Position *> vector;
		 *
		 * std::sort(vector.begin(), vector.end(), &Position::compare);
		 *
		 * @param left The lower Position.
		 * @param right The upper Position.
		 * @return True if left is lower than the right Position.
		 */
		inline static bool compare(const Position * left, const Position * right)
		{
			if (left->position.y() == right->position.y())
			{
				return left->position.x() < right->position.x();
			}
			return left->position.y() < right->position.y();
		}

		/**
		 * This returns the actual bending behaviour.
		 *
		 * @note This is only implemented for mrw::ui::SignalWidget and
		 * mrw::ui::SectionWidget.
		 *
		 * @return The actual Bending.
		 */
		inline Bending bending() const
		{
			return bending_state;
		}

		/**
		 * This sets the bending behaviour.
		 *
		 * @note This is only implemented for mrw::ui::SignalWidget and
		 * mrw::ui::SectionWidget.
		 *
		 * @param input
		 */
		inline void setBending(const Bending input)
		{
			bending_state = input;
		}

	private:
		static unsigned counter;

		QPoint   position;
		unsigned offset        = 0;
		unsigned ext_count     = 0;
		bool     inclined      = false;
		Bending  bending_state = Bending::STRAIGHT;
	};
}

#endif
