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
#include <model/position.h>
#include <model/region.h>

namespace mrw::model
{
	/**
	 * This class represents all Signal variants. It may represent
	 * LightSignal parts as well as FormSignal parts.
	 *
	 * @note In fact a Section can only have six Signal parts maximum: Every
	 * SignalType in both directions.
	 */
	class Signal : public AssemblyPart, public Position
	{
	public:
		enum Symbol : int
		{
			OFF  = -1,
			STOP =  0,
			GO   =  1,
			SLOW =  2
		};

		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum SignalType
		{
			/** Main signal. */
			MAIN_SIGNAL = 4,

			/** Distant signal displaying position of main signal. */
			DISTANT_SIGNAL = 2,

			/** Signal for shunting. */
			SHUNT_SIGNAL = 1,

			/** Outgoing light signal. */
			MAIN_SHUNT_SIGNAL = MAIN_SIGNAL | SHUNT_SIGNAL
		};

	protected:
		Signal       *      main_distant_pair = nullptr;

		/** The orientation referenced to the counting direction. */
		const bool          signal_direction;

		/** The signal type. */
		const SignalType    signal_type;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element,
			const SignalType      type);

		inline bool direction() const
		{
			return signal_direction;
		}

		inline SignalType type() const
		{
			return signal_type;
		}

		/**
		 * If there is a combined signal (main and distant) counting in the
		 * same direction this method returns the paired LightSignal. This
		 * has some implications:
		 * 1. This instance has to bei a main signal.
		 * 2. The paired signal is a distant signal.
		 * 3. Both are LightSignal instances.
		 *
		 * @return The paired distant LightSignal.
		 */
		inline Signal * pair() const
		{
			return main_distant_pair;
		}

		// TODO: Move into SignalController!
		void findPair(const std::vector<Signal *> & section_signals);

		virtual QString key() const override
		{
			return section()->region()->name() + partName();
		}

		static bool less(Signal * left, Signal * right);

		/**
		 * Since Device is not a direct base class this method returns a
		 * pointer to the Device of this Signal.
		 *
		 * @return The Device belonging to this Signal instance.
		 */
		virtual Device * device() = 0;

		/**
		 * This method returns the clear text QString of the Symbol this
		 * Signal is set to.
		 *
		 * @param symbol The Symbol enumeration to translate.
		 * @return The QString clear text of the given Symbol enumeration.
		 */
		static QString get(const Symbol & symbol);

		/**
		 * This method returns the clear text QString of the SignalType this
		 * Signal is set to.
		 *
		 * @param type The SignalType enumeration to translate.
		 * @return The QString clear text of the given SignalType enumeration.
		 */
		static QString get(const SignalType & type);

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

		static const mrw::util::ConstantEnumerator<mrw::can::SignalState> signal_constants;
		static const mrw::util::ConstantEnumerator<SignalType>            type_map;
		static const mrw::util::ConstantEnumerator<Symbol>                symbol_map;
	};
}

#endif // MRW_MODEL_SIGNAL_H
