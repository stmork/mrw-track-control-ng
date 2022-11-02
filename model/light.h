//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHT_H
#define MRW_MODEL_LIGHT_H

#include <QDomElement>
#include <model/module.h>

namespace mrw::model
{
	class Light
	{
	protected:
		const QString  name;
		const UnitNo   unit_no;
		const unsigned threshold;

	public:
		explicit Light(const QDomElement & element);
	};
}

#endif
