//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_REGULARSWITCHWIDGET_H
#define MRW_UI_REGULARSWITCHWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/regularswitchcontroller.h>

namespace mrw::ui
{
	/**
	 * This class intends to draw all states of a mrw::model::RegularSwitch
	 * which includes:
	 * * Inclination.
	 * * Left or right branched.
	 * * Extension in mrw::model::Position meaning.
	 * * Switch state left or right.
	 * * Counting direction.
	 * * mrw::model::Device::LockState including fail.
	 * * mrw::model::SectionState.
	 *
	 * Additionally like the RailWidget the SignalWidget is able to show
	 * extension in width.
	 *
	 * The RegularSwitchWidget does not have direct access to the model in
	 * MVC sense. It correspndends with the mrw::ctrl::RegularSwitchController
	 * which in turn correspondends with the mrw::model::RegularSwitch instance.
	 *
	 * @note The mrw::model::SectionState is taken from the enclosed
	 * mrw::model::Section instance.
	 *
	 * @image html RSwitch_AB_nL_RUF.jpg width=200
	 *
	 * @see mrw::ctrl::RegularSwitchController
	 * @see mrw::model::RegularSwitch
	 */
	class RegularSwitchWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		struct Status : public mrw::ctrl::RegularSwitchController::Status
		{
			QColor section_color = YELLOW;
			QColor outside_color = YELLOW;

			bool   pending       = false;
			bool   is_turn_out   = false;
		};

		explicit RegularSwitchWidget(
			QWidget               *              parent     = nullptr,
			mrw::ctrl::RegularSwitchController * controller = nullptr);

		virtual void computeConnectors() override;

		void prepare(Status & status) const;

	protected:
		virtual void paint(QPainter & painter) override;
		virtual bool hasLock() const override;
	};
}

#endif
