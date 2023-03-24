//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BASECONTROLLER_H
#define MRW_CTRL_BASECONTROLLER_H

#include <functional>

#include <QObject>

#include <model/device.h>
#include <model/section.h>

/**
 * The mrw::ctrl namespace contains all controller classes in the meaning of
 * the MVC paradigma. The model contains simply the data. The view
 * represents simply this data. Both don't talk directly but using an
 * intermediate controller which contains a sort of business logic. Generally
 * speaking the "view" BaseWidget does not see any model class.
 *
 * The model may be any class from the mrw::model namespace, The view classes
 * are in the mrw::ui namespace and the controller classes are in this
 * mrw::ctrl namespace.
 *
 * <img src="MVC-diagram.png"/>
 *
 * Actually there are four MVC groups:
 * 1. mrw::model::Rail - RailController - mrw::ui::RailWidget
 * 2. mrw::model::RegularSwitch - RegularSwitchController - mrw::ui::RegularSwitchWidget
 * 3. mrw::model::DoubleCrossSwitch - DoubleCrossSwitchController - mrw::ui::DoubleCrossSwitchWidget
 * 4. mrw::model::Signal - SignalController - mrw::ui::SignalWidget
 *
 * The MVC for signals is a little bit different because a controller may
 * represent a group of signals like a combined main and distant signal.
 *
 * As an example this scenario shows the MVC for a mrw::model::Rail.
 *
 * <img src="MVC-rail.png"/>
 *
 * The real life MRW-TrackControl software uses the RailControllerProxy
 * which uses the real railway model data. The RailControllerMock acts as a
 * test structure to test the handling of the RailWidget instead while using
 * the MRW-WidgetStudy app. This app simulates the data acces through a control
 * panel.
 *
 * The Proxy and Mock classes are used for switches and signals in the same
 * manner. These classes are not included into this library package. You can
 * find the Mock classes in the MRW-WdigetStudy scope but in the same mrw::ctrl
 * namespace. The Proxy classes are included in the MRW-TrackControl scope as
 * well.
 *
 * @note Note that the Proxy classes uses statemachines as logic. They are
 * included in the mrw::statecharts namespace.
 *
 * @see https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller
 */
namespace mrw::ctrl
{
	/**
	 * This controller base class unifies the interface for all widget
	 * controller classes. All derived classes have the role as a controller
	 * related to the MVC paradigma.
	 */
	class BaseController : public QObject
	{
		Q_OBJECT

	public:
		struct Status
		{
			float                           extensions;
			float                           lines;
			mrw::model::SectionState        section_state;
			mrw::model::Position::Bending   bending;
			mrw::model::Device::LockState   lock_state;
			bool                            direction;
		};

		explicit BaseController(QObject * parent = nullptr);

		/**
		 * This method returns the name of the controlled mrw::model::Device.
		 *
		 * @return The name of the controlled mrw::model::Device.
		 */
		virtual QString name() const = 0;

		/**
		 * This method returns the extensions of the controlled
		 * mrw::model::AssemblyPart.
		 *
		 * @return The extension count of the controlled
		 * mrw::model::AssemblyPart.
		 * @see mrw::model::Position::extensions()
		 */
		virtual float extensions() const = 0;

		/**
		 * This method returns the number of <em>extended</em> line counts. One
		 * line count is the full height of the controlled widget representing
		 * four mrw::model::Position::FRACTION units.
		 *
		 * @return The multiple height of the widget.
		 * @see mrw::model::Position::lines()
		 */
		virtual float lines() const
		{
			return 0;
		}

		/**
		 * This method returns true if the mrw::model::AssemblyPart is
		 * orientated left to right. So mrw::model::Signal instances have
		 * isDirection == true and mrw::model::RailPart instances have their
		 * "a" connector on the left side.
		 *
		 * @return True if the mrw::model::AssemblyPart is orientated left to
		 * right.
		 */
		virtual bool  isDirection() const = 0;

		/**
		 * This method returns true if the controlled widget is expandable.
		 *
		 * @note The switches are not expandable only simple rail representing
		 * widgets.
		 * @return True if the controlled widget is expandable.
		 */
		virtual bool isExpandable() const
		{
			return false;
		}

		/**
		 * This method returns the view attributes such as position, extension
		 * and orientation of the mrw::model::AssemblyPart to be rendered.
		 *
		 * @return The view attributes.
		 * @see mrw::model::Position
		 */
		virtual mrw::model::Position * position() const
		{
			return nullptr;
		}

		/**
		 * A controlled mrw::model::Device is always in conjunction with a
		 * surrounding mrw::model::Section. This method returns the
		 * mrw::model::SectionState of this mrw::model::Section.
		 *
		 * @note The controlled mrw::model::Device may be the
		 * mrw::model::Section itself.
		 *
		 * @return The mrw::model::SectionState.
		 */

		virtual mrw::model::SectionState state() const = 0;
		/**
		 * This method returns the modification state of the controlled
		 * mrw::model::Device.
		 *
		 * @return The mrw::model::Device::LockState of the controlled
		 * mrw::model::Device.
		 *
		 * @see mrw::model::Device::LockState
		 * @see mrw::model::Device::lock()
		 */
		virtual mrw::model::Device::LockState lock() const = 0;

		void status(BaseController::Status & status);

		/**
		 * This method returns the bending nature of the
		 * mrw::model::AssemblyPart for rendering purposes.
		 *
		 * @return The rendering hint concerning the mrw::model::AssemblyPart
		 * to be rendered.
		 * @see mrw::model::Position::Bending
		 */
		virtual mrw::model::Position::Bending bending() const = 0;

		template <class CTRL> static void callback(
			BaseController       *      base,
			std::function<void(CTRL *)> function)
		{
			CTRL * controller = dynamic_cast<CTRL *>(base);

			if (controller != nullptr)
			{
				function(controller);
			}
		}

	signals:
		/**
		 * This signal causes the refresh of the paired widget class.
		 *
		 * @see mrw::ui::BaseWidget
		 */
		void update();

		/**
		 * This signal causes a recomputation of the paired widget class.
		 */
		void reposition();
	};
}

#endif
