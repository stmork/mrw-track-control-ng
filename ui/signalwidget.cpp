//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QPainter>
#include <QPainterPath>

#include <ui/signalwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

SignalWidget::SignalWidget(
	QWidget      *     parent,
	SignalController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SignalWidget::computeConnectors()
{
	const Bending  bending = base_controller->bending();
	const unsigned ext     = base_controller->extensions();

	connector_list.clear();
	if ((bending != Bending::STRAIGHT) && (ext >= Position::FRACTION))
	{
		if (base_controller->isDirection())
		{
			connector_list.append(QPoint(
					Position::FRACTION + ext - Position::QUARTER,
					bending == Bending::RIGHT ? 4 : 0));
		}
		else
		{
			connector_list.append(QPoint(
					Position::QUARTER,
					bending == Bending::LEFT ? 4 : 0));
		}
	}
}

static const QVector<QPointF> points
{
	QPointF(  -5.0,   50.0),
	QPointF(  15.0,   30.0),
	QPointF(  55.0,   70.0),
	QPointF(  35.0,   90.0)
};

void SignalWidget::paint(QPainter & painter)
{
	SignalController * signal_controller = controller<SignalController>();
	QPainterPath       path;
	QPen               pen;
	QFont              font       = painter.font();
	const LockState    lock_state = base_controller->lock();
	const Bending      bending    = extensions() >= Position::FRACTION ?
		base_controller->bending() : Bending::STRAIGHT;

	const float        shift  = SCALE * extensions() / Position::FRACTION;
	const float        border = SCALE + shift;
	const float        start  = SCALE - border;

	const Signal::Symbol main_state    = signal_controller->main();
	const Signal::Symbol distant_state = signal_controller->distant();
	const Signal::Symbol shunt_state   = signal_controller->shunt();

	const bool has_main     = signal_controller->hasMain();
	const bool has_distant  = signal_controller->hasDistant();
	const bool has_shunting = signal_controller->hasShunting();
	const bool pending      = lockVisible(lock_state);

	// Unify coordinates
	rescale(painter, (Position::FRACTION + extensions()) * SCALE / Position::HALF);

	// Draw switch name before rotating to prevent rotated font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_SIZE);

	const QFontMetrics metrics(font);
	const QString      name = base_controller->name();
	const float        text_width = metrics.horizontalAdvance(name) + 10;

	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() ? -border : border - text_width,
			base_controller->isDirection() ? -80 : 30, text_width, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, name);

	if (!base_controller->isDirection())
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	const QColor section_color = sectionColor(base_controller->state());

	// Draw straight part of rail
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(section_color);
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);

	if ((lock_state == LockState::PENDING) || (lock_state == LockState::LOCKED))
	{
		painter.drawLine( -border,    0.0f, start - 25.0f, 0.0f);
		painter.drawLine( start + 25.0f, 0.0f, bending == Bending::STRAIGHT ? border : border - SCALE, 0.0f);

		// Draw point lock
		if (pending)
		{
			drawLock(
				painter,
				base_controller->lock() == LockState::LOCKED ?
				section_color : WHITE,
				start, 0);
		}
	}
	else
	{
		painter.drawLine( -border, 0.0f, bending == Bending::STRAIGHT ? border : border - SCALE, 0.0f);
	}

	// Rail bending to neighbour.
	if (bending != Bending::STRAIGHT)
	{
		const float height = SCALE + RAIL_WIDTH / 2;
		const float x      = border - SCALE / Position::HALF;

		if (bending == Bending::RIGHT)
		{
			drawSheared(painter, pen.color(), x,  SCALE, -height, -RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -SCALE,  height,  RAIL_SLOPE);
		}
	}

	// Predefine signal colors.
	const QColor main_color    = main_state    == Symbol::GO ? GREEN : RED;
	const QColor distant_color = distant_state == Symbol::GO ? GREEN : YELLOW;
	const QColor shunt_color   = shunt_state   == Symbol::GO ? WHITE : RED;

	// My change later depending on signal combinations.
	QColor       mast_color    = RED;
	bool         draw_shunt    = false;
	bool         draw_distant  = false;

	if (has_main)
	{
		mast_color = main_color;
		if (main_state == Signal::Symbol::GO)
		{
			draw_distant = has_distant;
			if (draw_distant)
			{
				mast_color = distant_color;
			}
		}
		else
		{
			draw_shunt = has_shunting;
		}
	}
	else
	{
		if (!has_shunting ||
			(base_controller->state() != SHUNTING) ||
			(shunt_state != Symbol::GO))
		{
			draw_distant = has_distant;
		}

		if (draw_distant)
		{
			mast_color = distant_color;
		}
		else
		{
			draw_shunt = has_shunting;
		}
	}

	// Draw mast foot
	pen.setColor(mast_color);
	pen.setWidth(8);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(start - 30, 35, start - 30, 75);

	// Draw mast
	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(start - 30, 55, start + (draw_distant || draw_shunt ? 30 : 70), 55);

	if (draw_shunt)
	{
		painter.fillRect(start + 10, 35, 40, 40, shunt_color);
	}

	if (draw_distant)
	{
		path.addPolygon(points);
		path.closeSubpath();
		path.translate(start, 0);
		painter.fillPath(path, QBrush(distant_color));
	}

	if (has_main)
	{
		pen.setColor(main_color);
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(main_color));
		painter.drawEllipse(start + 50, 35, 40, 40);
	}

	// Draw connector markers
	drawConnectors(painter);
}

bool SignalWidget::isLockPending() const
{
	return base_controller->lock() == LockState::PENDING;
}
