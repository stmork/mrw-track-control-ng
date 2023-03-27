//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SIGNALWIDGET_H
#define MRW_UI_SIGNALWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/signalcontroller.h>

namespace mrw::ui
{
	/**
	 * This class intends to draw all states of a combination of
	 * mrw::model::Signal instances. It can include three different signals
	 * in one orientation:
	 * * Main signal
	 * * Distant signal
	 * * Shunt signal
	 *
	 * Together the following states are drawn with this widget:
	 * * Counting direction.
	 * * mrw::model::Position::Bending rails to left or right.
	 * * mrw::model::Device::LockState including fail.
	 * * mrw::model::SectionState.
	 * * Shunt signal available and in state GO or STOP if existant.
	 * * Distant signal available and in state GO or STOP if existant.
	 * * Main signal available and in state GO or STOP if existant.
	 *
	 * The last three points decides the complex appearance of the signal
	 * inside the track control application.
	 *
	 * Additionally like the RailWidget the SignalWidget is able to show
	 * extension in width.
	 *
	 * The SignalWidget does not have direct access to the model in MVC sense.
	 * It correspndends with the mrw::ctrl::SignalController which in turn
	 * correspondends with the mrw::model::Signal instances.
	 *
	 * @note The mrw::model::SectionState is taken from the enclosed
	 * mrw::model::Section instance.
	 *
	 * @image html Signal__S_S_RUF.jpg width=200
	 *
	 * @see mrw::ctrl::SignalController
	 * @see mrw::model::Signal
	 * @see mrw::model::LightSignal
	 * @see mrw::model::FormSignal
	 */
	class SignalWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		struct Status : public mrw::ctrl::SignalController::Status
		{
			QColor section_color = YELLOW;

			// Signal colors.
			QColor main_color    = RED;
			QColor distant_color = YELLOW;
			QColor shunt_color   = RED;

			// Draw priorities.
			QColor mast_color    = RED;
			bool   draw_shunt    = false;
			bool   draw_distant  = false;

			bool   draw_lock     = false;
		};

		explicit SignalWidget(
			QWidget           *           parent     = nullptr,
			mrw::ctrl::SignalController * controller = nullptr);

		virtual void computeConnectors() override;

		void prepare(Status & status) const;

	protected:
		virtual void paint(QPainter & painter) override;
		virtual bool hasLock() const override;
	};
}

#endif
