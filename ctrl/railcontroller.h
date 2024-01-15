//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_RAILCONTROLLER_H
#define MRW_CTRL_RAILCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a simple rail. The "logic" contains
	 * information about drawing a rail bended or ended.
	 *
	 * @see BaseController
	 * @see mrw::model::Rail
	 */
	class RailController: public BaseController
	{
		Q_OBJECT

	public:
		/**
		 * This struct contains the complete Status of this RailController
		 * instance including the BaseController::Status.
		 */
		struct Status : public BaseController::Status
		{
			/**
			 * True if the Rail ends here at the <em>a</em> connection.
			 *
			 * @see RailController::aEnds()
			 */
			bool a_ends = false;

			/**
			 * True if the Rail ends here at the <em>b</em> connection.
			 *
			 * @see RailController::bEnds()
			 */
			bool b_ends = false;
		};

		explicit RailController(QObject * parent = nullptr);

		/**
		 * This method returns the complete Status of this RailController
		 * instance.
		 *
		 * @note This method calls the base class method
		 * BaseController::status() method.
		 *
		 * @param status The Status struct to fill.
		 */
		void status(RailController::Status & status) const;

	private:
		virtual bool            isExpandable() const override;

		/**
		 * The @c a connector does not have any further mrw::model::RailPart
		 * and ends here.
		 *
		 * @note Only one of the methods aEnds() and bEnds() may return true.
		 *
		 * @return True if the Rail ends here at the <em>a</em> connection.
		 */
		virtual bool  aEnds() const = 0;

		/**
		 * The @c b connector does not have any further mrw::model::RailPart
		 * and ends here.
		 *
		 * @note Only one of the methods aEnds() and bEnds() may return true.
		 *
		 * @return True if the Rail ends here at the <em>b</em> connection.
		 */
		virtual bool  bEnds() const = 0;
	};
}

#endif
