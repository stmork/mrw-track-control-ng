//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHTSIGNAL_H
#define MRW_MODEL_LIGHTSIGNAL_H

#include <regex>

#include <model/module.h>
#include <model/signal.h>
#include <model/device.h>

namespace mrw::model
{
	class MultiplexConnection;

	/**
	 * This class represents the handling of several light signals. These
	 * signals may be the following in detail:
	 * - Shunting signal using two pins.
	 * - A distant signal showing aspects Vr0 and Vr1 using two pins.
	 * - A distant signal showing aspects Vr0, Vr1 and Vr2 using four pins.
	 * - A main block signal using two pins.
	 * - A main incoming main signal using three pins.
	 * - A main outgoing main signal using five pins.
	 *
	 * These signals are connected to a MultiplexConnection module.
	 *
	 * @see MultiplexConnection
	 * @see Device
	 */
	class LightSignal : public Signal, public Device
	{
		friend class Section;

		static const std::regex path_regex;

		const size_t          lights;

		Controller      *     signal_controller = nullptr;
		MultiplexConnection * mux_connection    = nullptr;
		LightSignal     *     main_distant_pair = nullptr;

	public:
		explicit LightSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      type,
			const unsigned        light_count);

		bool valid() const override;

		/**
		 * This method returns the pointer to the CAN Controller module.
		 *
		 * @return The CAN Controller module.
		 */
		inline Controller * controller() const override
		{
			return signal_controller;
		}

		/**
		 * This method returns the pointer to the controlling
		 * MultiplexConnection module.
		 *
		 * @return The controlling MultiplexConnection module.
		 */
		inline MultiplexConnection * connection() const
		{
			return mux_connection;
		}

		inline const QString & name() const override
		{
			return part_name;
		}

		QString toString() const override;

		/**
		 * This method returns how much pins are used by the controlling
		 * MultiplexConnection module.
		 *
		 * @return The pin count used to control this LightSignal.
		 */
		inline size_t usedPins() const
		{
			return lights;
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
		inline LightSignal * pair() const
		{
			return main_distant_pair;
		}

	private:
		void findPair(const std::vector<LightSignal *> & light_signals);
	};
}

#endif // MRW_MODEL_SIGNAL_H
