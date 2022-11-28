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
	const QString  pos_key = key().replace(" ", "");
	const QString  value   = settings.value(pos_key, default_value).toString();

	parse(value);
}

void Position::write(QSettings & settings)
{
	QString pos_key = key().replace(" ", "");

	settings.setValue(pos_key, value());
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

void Position::parse(const QString & value)
{
	QStringList tokens  = value.split(",");
	bool        ok1     = false;
	bool        ok2     = false;
	int         x       = 0;
	int         y       = 0;
	int         offset  = 0;

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

	inclined      = false;
	ext_count     = 0;
	line_count    = 0;
	bending_state = Bending::STRAIGHT;
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

			case 'v':
			case 'V':
				line_count++;
				break;

			case 'h':
			case 'o':
			case 'O':
				offset  = HALF;
				break;

			case 'q':
				offset += QUARTER;
				break;

			case 'l':
			case 'L':
				bending_state = Bending::LEFT;
				break;
			case 'r':
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

QString Position::value() const
{
	const int offset = position.x() & MASK;
	QString   value   = QString("%1,%2").
		arg(position.x() / FRACTION).
		arg(position.y() / FRACTION);
	QString   ext;

	// Add fractional offset
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

	// Add extensions
	for (unsigned i = 0; i < ext_count; i++)
	{
		ext += 'X';
	}

	// Add extensions
	for (unsigned i = 0; i < line_count; i++)
	{
		ext += 'V';
	}

	// Add inclination
	if (inclined)
	{
		ext += 'i';
	}

	// Add bending
	switch (bending_state)
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

	return value;
}
