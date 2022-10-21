//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHT_H
#define MRW_MODEL_LIGHT_H

#include <QDomElement>

namespace mrw::model
{
	class Light
	{
		const QString  name;
		const unsigned type;
		const unsigned threshold;

	public:
		explicit Light(const QDomElement & element);
	};
}

#endif
