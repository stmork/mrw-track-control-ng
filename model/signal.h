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
	 *
	 * 1. Shunting stop:
	 * @image html Signal____S_RUF.jpg width=100
	 * 2. Distant expect stop:
	 * @image html Signal___S__RUF.jpg width=100
	 * 3. Main stop:
	 * @image html Signal__S___RUF.jpg width=100
	 * 4. Main outgoing stop:
	 * @image html Signal__S_S_RUF.jpg width=100
	 * 5. Shunting go:
	 * @image html Signal____G_RUS.jpg width=100
	 * 6. Shunting outgoing go:
	 * @image html Signal__S_G_RUS.jpg width=100
	 * 7. Tour go:
	 * @image html Signal__G___RUT.jpg width=100
	 * 8. Tour go expect stop.
	 * @image html Signal__GS__RUT.jpg width=100
	 * 9. Tour go expect go.
	 * @image html Signal__GG__RUT.jpg width=100
	 */
	class Signal : public AssemblyPart, public Position
	{
	public:
		/**
		 * The strategic Symbol of that Signal instance to display/show. This
		 * will be computed into a mrw::can::SignalState later depending on
		 * the SignalType.
		 */
		enum Symbol : int
		{
			/** Will always result in SIGNAL_OFF */
			OFF  = -1,

			/** Will result in SIGNAL_HP0, SIGNAL_SH0 or SIGNAL_VR0 */
			STOP =  0,

			/** Will result in SIGNAL_HP1, SIGNAL_SH1 or SIGNAL_VR1 */
			GO   =  1,

			/** Will result in SIGNAL_HP2 or SIGNAL_VR2 */
			SLOW =  2
		};

		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum SignalType
		{
			/**
			 * Main Signal.
			 * @image html Signal__S___RUF.jpg width=100
			 * @image html Signal__G___RUT.jpg width=100
			 */
			MAIN_SIGNAL = 4,

			/**
			 * Distant Signal displaying position of main signal.
			 * @image html Signal___S__RUT.jpg width=100
			 * @image html Signal___G__RUT.jpg width=100
			 *
			 * Distant signal displaying expected position of main signal
			 * combined with a local main signal.
			 * @image html Signal__GS__RUT.jpg width=100
			 * @image html Signal__GG__RUT.jpg width=100
			 */
			DISTANT_SIGNAL = 2,

			/**
			 * Signal for shunting.
			 * @image html Signal____S_RUF.jpg width=100
			 * @image html Signal____G_RUS.jpg width=100
			 */
			SHUNT_SIGNAL = 1,

			/**
			 * Outgoing LightSignal.
			 * @image html Signal__S_S_RUF.jpg width=100
			 * @image html Signal__S_G_RUS.jpg width=100
			 */
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

		/**
		 * This method returns @c true if the Signal is aligned counting
		 * direction and @c false otherwise.
		 *
		 * @return The alignment of the Signal to the counting direction.
		 */
		bool direction() const;

		/**
		 * This returns the SignalType of this Signal instance.
		 *
		 * @return The SignalType of this instance.
		 */
		SignalType type() const;

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

		QString key() const override;

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
