//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_FORMSIGNAL_H
#define MRW_MODEL_FORMSIGNAL_H

#include <regex>

#include <model/signal.h>
#include <model/switchmodulereference.h>

namespace mrw::model
{
	/**
	 * This class represents the handling of several form signals. These
	 * signals may be the following in detail:
	 * - Shunting signal using two pins.
	 * - A distant signal showing aspects Vr0 and Vr1 using two pins.
	 * - A distant signal showing aspects Vr0, Vr1 and Vr2 using three pins.
	 * - A main signal showing aspects Hp0 and Hp1 using two pins.
	 * - A main signal showing aspects Hp0, Hp1 and Hp2 using three pins.
	 *
	 * @note Until now form shunting signals are not supported, yet.
	 *
	 * The form signals are connected to a SwitchModule referenced using the
	 * SwitchModuleReference class.
	 *
	 * @see SwitchModule
	 * @see SwitchModuleReference
	 * @see Device
	 */
	class FormSignal : public Signal, public SwitchModuleReference
	{
	public:
		explicit FormSignal(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element,
			const SignalType      is_main);

		// Implementations from Device
		bool                 isUnlockable() const noexcept override;
		const QString    &   name()         const noexcept override;
		mrw::can::MrwMessage configMsg(const unsigned pin) const noexcept override;

		// Implementations from AssemblyPart
		bool            valid()    const noexcept override;
		QString         toString() const noexcept override;
	};
}

#endif
