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
	 * third field may contain additional behavior. The coordinates are in
	 * standard positions so you have to multiply the position by the pixel
	 * size of the controlling widget.
	 * 1. Any 'o' or 'O' character shifts the widget to the right by one half
	 * size. Many occurences leads into only one shift.
	 * 2. Any 'x' or 'X' enlarges the controlling widget by a half standard
	 * widget size. Many occurences leads into the multiple of them.
	 *
	 * @see mrw::ui::BaseWidget
	 */
	class Position
	{
		QPoint   position;
		unsigned offset    = 0;
		unsigned extension = 0;

		static unsigned counter;

	public:
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

		/**
		 * This getter returns the half extension unit of the controlling widget
		 * size.
		 *
		 * @return The extension size in half controlling widget size.
		 */
		inline unsigned extend() const
		{
			return extension;
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
	};
}

#endif
