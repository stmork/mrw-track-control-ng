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

	public:
		explicit LightSignal(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element,
			const SignalType      type,
			const unsigned        light_count);

		Controller * controller() const override;

		/**
		 * This method returns the pointer to the controlling
		 * MultiplexConnection module.
		 *
		 * @return The controlling MultiplexConnection module.
		 */
		MultiplexConnection * connection() const;

		const QString & name()     const override;
		Device     *    device()         override;
		bool            valid()    const override;
		QString         toString() const override;


		inline bool unlockable() const override
		{
			// TODO: Implement!
			return true;
		}

		/**
		 * This method returns how much pins are used by the controlling
		 * MultiplexConnection module.
		 *
		 * @return The pin count used to control this LightSignal.
		 */
		size_t usedPins() const;
	};
}

#endif // MRW_MODEL_SIGNAL_H
