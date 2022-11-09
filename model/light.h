//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHT_H
#define MRW_MODEL_LIGHT_H

#include <QDomElement>

#include <model/module.h>
#include <model/device.h>

namespace mrw::model
{
	class Light : public Device
	{
		Controller  *  light_controller;

	protected:
		const QString  light_name;
		const unsigned threshold;

	public:
		explicit Light(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
		virtual ~Light() = default;

		inline const QString & name() const override
		{
			return light_name;
		}

		inline Controller * controller() const override
		{
			return light_controller;
		}
	};
}

#endif
