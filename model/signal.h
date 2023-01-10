//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	 * These symbols appears on the trck control application:
	 * 1. Shunting Signal:
	 *   1. Shunting stop:<br/><img src="Signal____S_RUF.jpg" width="100"/>
	 *   2. Shunting go:<br/><img src="Signal____G_RUS.jpg" width="100"/>
	 * 2. Distant Signal:
	 *   1. Distant expect stop:<br/><img src="Signal___S__RUF.jpg" width="100"/>
	 *   2. Distant expect go:<br/><img src="Signal___G__RUF.jpg" width="100"/>
	 * 3. Main Signal:
	 *   1. Main stop:<br/><img src="Signal__S___RUF.jpg" width="100"/>
	 *   2. Main outgoing stop:<br/><img src="Signal__S_S_RUF.jpg" width="100"/>
	 *   3. Shunting outgoing go:<br/><img src="Signal__S_G_RUS.jpg" width="100"/>
	 * 4. Main Signal combined with distant Signal:
	 *   1. Tour go:<br/><img src="Signal__G___RUT.jpg" width="100"/>
	 *   2. Tour go expect stop:<br/><img src="Signal__GS__RUT.jpg" width="100"/>
	 *   3. Tour go expect go:<br/><img src="Signal__GG__RUT.jpg" width="100"/>
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
			GO   =  1
		};

		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum SignalType
		{
			/**
			 * Main Signal:
			 *
			 * <img src="Signal__S___RUF.jpg" width="100"/> <img src="Signal__G___RUT.jpg" width="100"/>
			 */
			MAIN_SIGNAL = 4,

			/**
			 * Distant Signal displaying position of main signal:
			 *
			 * <img src="Signal___S__RUT.jpg" width="100"/> <img src="Signal___G__RUT.jpg" width="100"/>
			 *
			 * Distant signal displaying expected position of main signal
			 * combined with a local main signal displaying go:
			 *
			 * <img src="Signal__GS__RUT.jpg" width="100"/> <img src="Signal__GG__RUT.jpg" width="100"/>
			 */
			DISTANT_SIGNAL = 2,

			/**
			 * Signal for shunting:
			 *
			 * <img src="Signal____S_RUF.jpg" width="100"/> <img src="Signal____G_RUS.jpg" width="100"/>
			 */
			SHUNT_SIGNAL = 1,

			/**
			 * Outgoing LightSignal:
			 *
			 * <img src="Signal__S_S_RUF.jpg" width="100"/> <img src="Signal__S_G_RUS.jpg" width="100"/>
			 */
			MAIN_SHUNT_SIGNAL = MAIN_SIGNAL | SHUNT_SIGNAL
		};

	protected:

		/** The orientation referenced to the counting direction. */
		const bool          signal_direction;

		/** The signal type. */
		const SignalType    signal_type;

		mrw::can::SignalState signal_state = mrw::can::SIGNAL_OFF;

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

		mrw::can::SignalState state() const;

		void setState(const mrw::can::SignalState new_state);

		QString key() const override;

		/**
		 * This comparator method returns true if the "left" Signal has
		 * higher priority than the "right" Signal. Left or right is not a
		 * topology but a mathmatical meaning. It means left from and right
		 * from "<" sign. There are two criteria to sort:
		 * 1. The orientation related to counting direction
		 * 2. The SignalType where a main Signal has higher priority than
		 * a distant Signal and a shunting Signal.
		 *
		 * @param left The left of "<" Signal.
		 * @param right The right of "<" Signal.
		 * @return True if left has higher priority than right.
		 */
		static bool less(Signal * left, Signal * right);

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
