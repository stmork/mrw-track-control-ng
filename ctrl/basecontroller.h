//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BASECONTROLLER_H
#define MRW_CTRL_BASECONTROLLER_H

#include <QObject>

#include <model/device.h>
#include <model/section.h>

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
