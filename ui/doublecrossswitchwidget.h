//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_DOUBLECROSSSWITCHWIDGET_H
#define MRW_UI_DOUBLECROSSSWITCHWIDGET_H

#include <ui/controllerwidget.h>
#include <model/doublecrossswitch.h>
#include <ctrl/doublecrossswitchcontroller.h>

namespace mrw::ui
{
	/**
	 * This class intends to draw all states of a mrw::model::DoubleCrossSwitch
	 * which includes:
	 * * Four states aligned to route/tour.
	 * * Branch alignment left or right.
	 * * Counting direction.
	 * * mrw::model::Device::LockState including fail.
	 * * mrw::model::SectionState.
	 *
	 * The DoubleCrossSwitchWidget does not have direct access to the model in
	 * MVC sense. It correspndends with the
	 * mrw::ctrl::DoubleCrossSwitchController which in turn correspondends with
	 * the mrw::model::DoubleCrossSwitch instance.
	 *
	 * @note The mrw::model::SectionState is taken from the enclosed
	 * mrw::model::Section instance.
	 *
	 * @image html XSwitch_AC_RUF.jpg width=200
	 *
	 * @see mrw::ctrl::DoubleCrossSwitchController
	 * @see mrw::model::DoubleCrossSwitch
	 */
	class DoubleCrossSwitchWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchWidget(
			QWidget                 *                parent     = nullptr,
			mrw::ctrl::DoubleCrossSwitchController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void paint(QPainter & painter) override;
		virtual bool isLockPending() const override;

	private:
		bool isA(const mrw::ctrl::DoubleCrossSwitchController::Status & status) const;
		bool isB(const mrw::ctrl::DoubleCrossSwitchController::Status & status) const;
		bool isC(const mrw::ctrl::DoubleCrossSwitchController::Status & status) const;
		bool isD(const mrw::ctrl::DoubleCrossSwitchController::Status & status) const;
	};
}

#endif
