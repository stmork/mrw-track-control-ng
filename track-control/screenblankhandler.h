//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef SCREENBLANKHANDLER_H
#define SCREENBLANKHANDLER_H

#include <util/self.h>

#include <statecharts/OperatingModeStatechart.h>

class QScreen;

class ScreenBlankHandler :
	public mrw::util::Self<mrw::statechart::OperatingModeStatechart::Screen::OperationCallback>
{
	QScreen * screen  = nullptr;

public:
	ScreenBlankHandler();
	virtual ~ScreenBlankHandler() = default;

	operator QScreen * () const;

protected:
	// Implementation for Screen::OperationCallback
	virtual void          resetBlank() override;
	virtual void          blank(bool active) override;
	virtual void          autoBlank(bool active) override;
};

#endif
