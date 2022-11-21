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
	class Position
	{
		QPoint   position;
		unsigned offset    = 0;
		unsigned extension = 0;

		static unsigned counter;

	public:
		Position() = default;

		void parse(QSettings & settings, const QString & default_value);
		void write(QSettings & settings);

		inline unsigned extend() const
		{
			return extension;
		}

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
