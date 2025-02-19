//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

#include "screenblankhandler.h"

#ifdef X11_SCREEN_SAVER
#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#endif

ScreenBlankHandler::ScreenBlankHandler()
{
	screen  = QGuiApplication::primaryScreen();
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	if (display != nullptr)
	{
		qDebug().noquote() << "Init display:" << display;

		XCloseDisplay(display);
	}
#endif
}

ScreenBlankHandler::operator QScreen * () const
{
	return screen;
}

void ScreenBlankHandler::blank(bool blank_active)
{
	qDebug().noquote() << "DPMS blank:" << blank_active;
	if (screen != nullptr)
	{
		QPlatformScreen * handle = screen->handle();

		if (handle != nullptr)
		{
#if 0
			handle->setPowerState(blank_active ? QPlatformScreen::PowerOff : QPlatformScreen::PowerOn);
#endif
		}
	}
}

void ScreenBlankHandler::autoBlank(bool auto_blank_active)
{
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	if (display != nullptr)
	{
		qDebug().noquote() << "Auto blank:" << auto_blank_active << display;

		XScreenSaverSuspend(display, auto_blank_active ? False : True);
		XCloseDisplay(display);
	}
#endif
}

void ScreenBlankHandler::resetBlank()
{
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	if (display != nullptr)
	{
		qDebug().noquote() << "Resetting screen saver..." << display;

		XResetScreenSaver(static_cast<Display *>(display));
		XCloseDisplay(display);
	}
#endif
}
