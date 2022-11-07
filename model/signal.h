//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SIGNAL_H
#define MRW_MODEL_SIGNAL_H

#include "model/module.h"
#include "model/assemblypart.h"

namespace mrw::model
{
	/**
	 * This class represents all Signal representations. It may represent
	 * LightSignal parts as well as FormSignal parts.
	 *
	 * @note In fact a Section can only have six Signal parts maximum: Every
	 * SignalType in both directions.
	 */
	class Signal : public AssemblyPart
	{
	public:
		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum SignalType
		{
			MAIN_SIGNAL, ///< Main signal.
			PRE_SIGNAL,  ///< Pre signal.
			SHUNT_SIGNAL ///< Signal for shunting.
		};

	protected:
		/** The orientation referenced to the counting direction. */
		const bool          direction;

		/** The signal type. */
		const SignalType    signal_type;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      type);

		QString toString() const override;

	private:
		void link() override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
