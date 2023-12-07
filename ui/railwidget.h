//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

	public:
		struct Status : public mrw::ctrl::RailController::Status
		{
			bool do_bend = false;
			bool any_end = false;
		};

		explicit RailWidget(
			QWidget          *          parent     = nullptr,
			mrw::ctrl::RailController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void prepare(Status & status) const;
		void paint(QPainter & painter) override;
	};
}

#endif
