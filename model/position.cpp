//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QStringList>

#include <model/position.h>

using namespace mrw::model;

unsigned Position::counter = 0;

void Position::parse(QSettings & settings, const QString & default_value)
{
	QString     pos_key = key().replace(" ", "");
	QString     value   = settings.value(pos_key, default_value).toString();
	QStringList tokens  = value.split(",");
	QPoint      pos;
	bool        ok1 = false;
	bool        ok2 = false;
	int         x   = 0;
	int         y   = 0;

	if (tokens.size() >= 2)
	{
		x = tokens[0].toInt(&ok1);
		y = tokens[1].toInt(&ok2);
	}

	if (!(ok1 && ok2))
	{
		x = counter % 40;
		y = counter / 40;

		counter++;
	}

	if (tokens.size() >= 3)
	{
		for (QChar c : tokens[2])
		{
			switch (c.toLatin1())
			{
			case 'x':
			case 'X':
				extension++;
				break;

			case 'o':
			case 'O':
				offset = 1;
				break;

			default:
				// Intentionally do nothing.
				break;
			}
		}
	}
	position.setX(x * 2 + offset);
	position.setY(y * 2);
}

void Position::write(QSettings & settings)
{
	QString pos_key = key().replace(" ", "");
	QString value   = QString("%1,%2").arg(position.x() / 2).arg(position.y() / 2);
	QString ext;

	offset = position.x() & 1;
	if (offset != 0)
	{
		ext += 'o';
	}
	for (unsigned i = 0; i < extension; i++)
	{
		ext += 'X';
	}
	if (!ext.isEmpty())
	{
		value += "," + ext;
	}

	settings.setValue(pos_key, value);
}
