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
			case 'i':
			case 'I':
				inclined = true;
				break;

			case 'x':
			case 'X':
				ext_count++;
				break;

			case 'h':
			case 'o':
			case 'O':
				offset  = HALF;
				break;

			case 'q':
				offset += QUARTER;
				break;

			case 'L':
				bending_state = Bending::LEFT;
				break;
			case 'R':
				bending_state = Bending::RIGHT;
				break;

			default:
				// Intentionally do nothing.
				break;
			}
		}
	}
	position.setX(x * FRACTION + offset);
	position.setY(y * FRACTION);
}

void Position::write(QSettings & settings)
{
	QString pos_key = key().replace(" ", "");
	QString value   = QString("%1,%2").
		arg(position.x() / FRACTION).
		arg(position.y() / FRACTION);
	QString ext;

	offset = position.x() & MASK;
	if (inclined)
	{
		ext += 'i';
	}
	if (offset == HALF)
	{
		ext += 'h';
	}
	else
	{
		for (unsigned o = 0; o < (offset & MASK); o++)
		{
			ext += 'q';
		}
	}
	for (unsigned i = 0; i < ext_count; i++)
	{
		ext += 'X';
	}
	switch(bending_state)
	{
	case Bending::LEFT:
		ext += 'L';
		break;
	case Bending::RIGHT:
		ext += 'R';
		break;

	default:
		// Intentionally do nothing.
		break;
	}

	if (!ext.isEmpty())
	{
		value += "," + ext;
	}

	settings.setValue(pos_key, value);
}

void Position::move(const int right, const int down)
{
	const QPoint increment(right, down);

	position += increment;
}

void Position::setX(const int x)
{
	position.setX(x);
}
