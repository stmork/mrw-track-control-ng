//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
	 * - A pre signal with Vr0 and Vr1 using two pins.
	 * - A pre signal with Vr0, Vr1 and Vr2 using three pins.
	 * - A main signal with Hp0 and Hp1 using two pins.
	 * - A main signal with Hp0, Hp1 and Hp2 using three pins.
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
		const unsigned inductors;

	public:
		explicit FormSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      is_main);

		inline const QString & name() const override
		{
			return part_name;
		}
	};
}

#endif
