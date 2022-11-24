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
		 * This method returns the name of the controlled Device.
		 *
		 * @return The name of the controlled Device.
		 */
		virtual QString name() const = 0;

		/**
		 * A controlled Device is always in conjunction with a surrounding
		 * Section. This method returns the SectionState of this Section.
		 *
		 * @note The controlled Device may be the Section itself.
		 *
		 * @return The SectionState.
		 */
		virtual mrw::model::SectionState state() const = 0;

		/**
		 * This method returns the modification state of the controlled Device.
		 *
		 * @return The LockState of the controlled Device.
		 *
		 * @see mrw::model::Device::LockState
		 * @see mrw::model::Device::lock()
		 */
		virtual mrw::model::Device::LockState lock() const = 0;

		virtual float extensions()   const = 0;

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

		virtual bool isExpandable() const
		{
			return false;
		}

		virtual mrw::model::Position * position() const
		{
			return nullptr;
		}

		virtual mrw::model::Position::Bending bending() const = 0;

	signals:
		/**
		 * This signal causes the refresh of the paired widget class.
		 *
		 * @see mrw::ui::BaseWidget
		 */
		void update();
		void reposition();
	};
}

#endif
