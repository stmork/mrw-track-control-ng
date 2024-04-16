//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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
	 *   2. Shunting go:<br/><img src="Signal____G_RLS.jpg" width="100"/>
	 * 2. Distant Signal:
	 *   1. Distant expect stop:<br/><img src="Signal___S__RUF.jpg" width="100"/>
	 *   2. Distant expect go:<br/><img src="Signal___G__RLT.jpg" width="100"/>
	 * 3. Main Signal:
	 *   1. Main stop:<br/><img src="Signal__S___RUF.jpg" width="100"/>
	 *   2. Main outgoing stop:<br/><img src="Signal__S_S_RUF.jpg" width="100"/>
	 *   3. Shunting outgoing go:<br/><img src="Signal__S_G_RLS.jpg" width="100"/>
	 * 4. Main Signal combined with distant Signal:
	 *   1. Tour go:<br/><img src="Signal__G___RLT.jpg" width="100"/>
	 *   2. Tour go expect stop:<br/><img src="Signal__GS__RLT.jpg" width="100"/>
	 *   3. Tour go expect go:<br/><img src="Signal__GG__RLT.jpg" width="100"/>
	 */
	class Signal : public AssemblyPart, public Position
	{
	public:
		/**
		 * The strategic Symbol of that Signal instance to display/show. This
		 * will be computed into a mrw::can::SignalState later depending on
		 * the SignalType.
		 */
		enum class Symbol : int
		{
			/** Will always result in SIGNAL_OFF */
			OFF  = -1,

			/** Will result in SIGNAL_HP0, SIGNAL_SH0 or SIGNAL_VR0 */
			STOP =  0,

			/**
			 * Will result in SIGNAL_HP1, SIGNAL_SH1 or SIGNAL_VR1. In case
			 * that at least two curved RailPart elements occur between two
			 * main signals a SIGNAL_VR2 or SIGNAL_HP2 may occur.
			 *
			 * @note The ESTW does not distinguish between SIGNL_VR1 or
			 * SIGNAL_VR2 and SIGNAL_HP1 or SIGNAL_HP2 respectively.
			 */
			GO   =  1
		};

		/**
		 * This enumeration represents which basic type of the Signal.
		 */
		enum class SignalType : unsigned
		{
			/**
			 * Main Signal:
			 *
			 * <img src="Signal__S___RUF.jpg" width="100"/> <img src="Signal__G___RLT.jpg" width="100"/>
			 */
			MAIN_SIGNAL = 4,

			/**
			 * Distant Signal displaying position of main signal:
			 *
			 * <img src="Signal___S__RUF.jpg" width="100"/> <img src="Signal___G__RLT.jpg" width="100"/>
			 *
			 * Distant signal displaying expected position of main signal
			 * combined with a local main signal displaying go:
			 *
			 * <img src="Signal__GS__RLT.jpg" width="100"/> <img src="Signal__GG__RLT.jpg" width="100"/>
			 */
			DISTANT_SIGNAL = 2,

			/**
			 * Signal for shunting:
			 *
			 * <img src="Signal____S_RUF.jpg" width="100"/> <img src="Signal____G_RLS.jpg" width="100"/>
			 */
			SHUNT_SIGNAL = 1,

			/**
			 * Outgoing LightSignal:
			 *
			 * <img src="Signal__S_S_RUF.jpg" width="100"/> <img src="Signal__S_G_RLS.jpg" width="100"/>
			 */
			MAIN_SHUNT_SIGNAL = MAIN_SIGNAL | SHUNT_SIGNAL
		};

	protected:

		/** The orientation referenced to the counting direction. */
		const bool          signal_direction;

		/** The signal type. */
		const SignalType    signal_type;

		mrw::can::SignalAspect signal_aspect = mrw::can::SignalAspect::SIGNAL_OFF;

		/**
		 * This callback retrieves the Signal::Symbol state from another
		 * controling state instance.
		 *
		 * @note This might be useless since some signals may be grouped
		 * together so this state can only be completed correctly by its
		 * controlling group.
		 *
		 * @return The Signal::Symbol state.
		 */
		std::function<Symbol(void)> symbol = []()
		{
			return Symbol::OFF;
		};

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element,
			const SignalType      type);

		/**
		 * This method adds a callback for retrieving the Symbol state. This
		 * is necessary because its state is somewhere else inside another
		 * controlling class.
		 *
		 * @note This might be useless since some signals may be grouped
		 * together so this state can only be completed correctly by its
		 * controlling group.
		 *
		 * @param callback The callback stored inside the symbol function.
		 * @see symbol.
		 */
		void setSymbolCallback(std::function<Symbol(void)> callback);

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
		 * This method returns the real life displayed signal aspect.
		 *
		 * @return The configured signal aspect.
		 */
		mrw::can::SignalAspect aspect() const;

		/**
		 * This method sets the real life signal aspect.
		 *
		 * @note Setting this aspect does not cause a change via sending a
		 * CAN message but hints of sending it.
		 *
		 * @param new_state The new real life signal aspect to be shown.
		 * @return True if this aspect update should cause sending of
		 * CAN messages.
		 */
		[[nodiscard]]
		bool setAspect(const mrw::can::SignalAspect new_state);

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
		[[nodiscard]]
		static bool less(Signal * left, Signal * right);

		/**
		 * This method returns the clear text QString of the Symbol this
		 * Signal is set to.
		 *
		 * @param symbol The Symbol enumeration to translate.
		 * @return The QString clear text of the given Symbol enumeration.
		 */
		[[nodiscard]]
		static QString get(const Symbol symbol);

		/**
		 * This method returns the clear text QString of the SignalType this
		 * Signal is set to.
		 *
		 * @param type The SignalType enumeration to translate.
		 * @return The QString clear text of the given SignalType enumeration.
		 */
		[[nodiscard]]
		static QString get(const SignalType type);

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
		QString typeDescr() const;

	private:
		void link() override;

		static const mrw::util::ConstantEnumerator<SignalType>    type_map;
		static const mrw::util::ConstantEnumerator<Symbol>        symbol_map;
	};
}

#endif
