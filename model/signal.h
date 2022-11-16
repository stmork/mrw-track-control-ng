//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SIGNAL_H
#define MRW_MODEL_SIGNAL_H

#include <QPoint>

#include <can/commands.h>
#include <util/constantenumerator.h>
#include <model/module.h>
#include <model/assemblypart.h>

namespace mrw::model
{
	/**
	 * This class represents all Signal variants. It may represent
	 * LightSignal parts as well as FormSignal parts.
	 *
	 * @note In fact a Section can only have six Signal parts maximum: Every
	 * SignalType in both directions.
	 */
	class Signal : public AssemblyPart
	{
		static const mrw::util::ConstantEnumerator<mrw::can::SignalState> signal_constants;

	public:
		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum SignalType
		{
			/** Main signal. */
			MAIN_SIGNAL,

			/** Distant signal displaying position of main signal. */
			DISTANT_SIGNAL,

			/** Signal for shunting. */
			SHUNT_SIGNAL
		};

	protected:
		/** The orientation referenced to the counting direction. */
		const bool          direction;

		/** The signal type. */
		const SignalType    signal_type;

		/** The logical position on the track control screen. */
		QPoint              signal_position;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      type);

		/**
		 * This method sets the logical coordinates for the track control
		 * screen.
		 *
		 * @param pos The new logical position.
		 */
		inline void setPosition(const QPoint & pos)
		{
			signal_position = pos;
		}

	protected:
		/**
		 * This method returns a brief symbol representing the direction and
		 * SignalType regardless if it is a LightSignal or a FormSignal:
		 * - <pre> --O</pre> or <pre>O-- </pre> Main signal.
		 * - <pre> -o </pre> or <pre> o- </pre> Distant signal.
		 * - <pre> ]  </pre> or <pre>  [ </pre> Shunting signal.
		 *
		 * @return The brief symbol of what signal is configured.
		 */
		QString symbol() const;

	private:
		void link() override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
