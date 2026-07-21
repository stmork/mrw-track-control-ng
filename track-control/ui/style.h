//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_STYLE_H
#define MRW_UI_STYLE_H

#include <QApplication>

namespace mrw::ui
{
	class Style
	{
		static const QString   css;
		static const QPalette  background_color;

	public:
		static void setEstwStyle(QApplication & app);
		static void setEstwStyle(QWidget * widget);
	};
}

#endif
