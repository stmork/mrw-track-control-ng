//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_RAILWIDGET_H
#define MRW_UI_RAILWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/railcontroller.h>

namespace mrw::ui
{
	/**
	 * The RailWidget displays a simple mrw::model::Rail without any active
	 * part like switches or signals. It can display extensions in width and
	 * may span over multiple lines vertically. It can show the following
	 * states:
	 * * Counting direction.
	 * * End point.
	 * * Horizontal extensions.
	 * * Multiple vertical lines.
	 * * mrw::model::Position::Bending rails to left, straight or right.
	 * * mrw::model::Device::LockState including fail.
	 * * mrw::model::SectionState.
	 *
	 * The RailWidget does not have direct access to the model in MVC sense.
	 * It correspndends with the mrw::ctrl::RailController which in turn
	 * correspondends with the mrw::model::Rail instance.
	 *
	 * @note The mrw::model::SectionState is taken from the enclosed
	 * mrw::model::Section instance.
	 *
	 * @see mrw::ctrl::RailController
	 * @see mrw::model::Rail
	 */
	class RailWidget : public ControllerWidget
	{
		Q_OBJECT

		/** The width of a drawn road crossing a rail. */
		static constexpr float  ROAD_WIDTH    =  60.0f;

		/** The width of a road border. */
		static constexpr float  ROAD_BORDER   =  5.0f;

		/** The width of a resulting road asphalt. */
		static constexpr float  ASPHALT_WIDTH =  ROAD_WIDTH - 2 * ROAD_BORDER;

	public:
		struct Status : public mrw::ctrl::RailController::Status
		{
			bool do_bend          = false;
			bool any_end          = false;
			bool draw_lock            = false;
			bool draw_crossing_before = false;
			bool draw_crossing_after  = false;
		};

		explicit RailWidget(
			QWidget          *          parent     = nullptr,
			mrw::ctrl::RailController * controller = nullptr);

		virtual void computeConnectors() override;
		virtual bool hasLock() const override;

	protected:
		void prepare(Status & status) const;
		void paint(QPainter & painter) override;

	private:
		void drawCrossing(
			QPainter   &   painter,
			const double   border) const;
	};
}

#endif
