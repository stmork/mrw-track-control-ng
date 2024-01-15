//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_DEVICE_H
#define MRW_MODEL_DEVICE_H

#include <util/constantenumerator.h>
#include <can/commands.h>
#include <model/module.h>

namespace mrw::can
{
	class MrwMessage;
}

namespace mrw::model
{
	class Controller;

	/**
	 * This class represents device components for which a corresponding
	 * CAN command is existing. The CAN ID is built from the controller ID
	 * and the unit number of this Device.
	 *
	 * @see Controller::id()
	 */
	class Device
	{
		const mrw::can::UnitNo   unit_no = 0;

	public:
		/**
		 * This enumeration represents the processing state of this Device.
		 */
		enum class LockState : int
		{
			/** An error occured during pending processing. */
			FAIL = -1,

			/** The Device is unlocked and can be modified. */
			UNLOCKED = 0,

			/** The Device is actually modifying its state. */
			PENDING,

			/** The Device is in a state where state modification is prohibited. */
			LOCKED
		};

		explicit Device(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		/**
		 * This method returns the unit number which is part of the CAN
		 * identifier sent to the controller with a CAN command. It is
		 * directly used inside the CAN extended frame format ID.
		 *
		 * @note Even the extended frame format ID is 18 bits wide only
		 * 16 bits are used from this unit number.
		 *
		 * @return The unit number of this Device.
		 * @see Controller::id()
		 */
		mrw::can::UnitNo unitNo() const noexcept;

		/**
		 * This method returns the actual LockState of this Device.
		 *
		 * @return The actual LockState.
		 */
		LockState lock() const noexcept;

		/**
		 * This method sets a new LockState.
		 *
		 * @note In case the new LockState is LockState::UNLOCKED the
		 * unlockable() method has to return true. Otherwise the Device
		 * cannot be unlocked.
		 *
		 * @param input The new LockState to set.
		 * @see unlockable()
		 */
		void setLock(const LockState input) noexcept;

		/**
		 * This method returns true if it is possible to <em>unlock</em> this
		 * Device. There are some conditions to be met to ensure a safe
		 * unlock. This method is called during lock() method with a given
		 * parameter of LockState::UNLOCKED.
		 *
		 * @note This test method is meant that a locked Device is
		 * <u>un</u>lockable. It is not meant that if this Device is unlocked
		 * and not lockable!
		 *
		 * @return True if this Device is safely possible to unlock.
		 *
		 * @see lock()
		 */
		virtual bool isUnlockable() const noexcept = 0;

		/**
		 * This method returns the pointer to the managing controller.
		 *
		 * @return
		 */
		virtual Controller * controller() const noexcept = 0;

		/**
		 * This method returns the name modelled inside the EMF/XMI model
		 * file.
		 *
		 * @return The Device name modelled inside the EMF/XMI model.
		 */
		virtual const QString & name() const noexcept = 0;

		/**
		 * This convenience method creates a command mrw::can::MrwMessage and
		 * adds the mrw::can::Command. It initializes additionally the
		 * Controller id and the Device unit number.
		 *
		 * @param command The mrw::can::Command specifier.
		 * @return The resulting mrw::can::MrwCommand.
		 * @throws std::invalid_argument if the Controller was not configured.
		 *
		 * @see Controller::id()
		 * @see mrw::can::MrwMessage
		 */
		mrw::can::MrwMessage command(const mrw::can::Command command) const;

		/**
		 * This method provides a configuration message which describes the
		 * connection from the Device to the Module.
		 *
		 * @return The configuration MrwMessage ready to send.
		 */
		virtual mrw::can::MrwMessage configMsg(const unsigned pin) const = 0;

		/**
		 * This method returns the clear text QString of the LockState this
		 * Device is set to.
		 *
		 * @param state The LockState enumeration to translate.
		 * @return The QString clear text of the given LockState enumeration.
		 */
		static QString get(const LockState & state) noexcept;

	private:
		LockState                lock_state = LockState::UNLOCKED;

		static const mrw::util::ConstantEnumerator<LockState>  lock_map;
	};
}

#endif
