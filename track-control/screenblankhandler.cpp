//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <fcntl.h>
#include <unistd.h>

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

#include <xf86drm.h>
#include <xf86drmMode.h>

#include "screenblankhandler.h"

#ifdef X11_SCREEN_SAVER
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>
#endif

ScreenBlankHandler::ScreenBlankHandler()
{
	screen  = QGuiApplication::primaryScreen();
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	if (display != nullptr)
	{
		qDebug().noquote() << "Init display:" << display;
		qDebug().noquote() << "Has DPMS:"  << DPMSCapable(display);

		XCloseDisplay(display);
		display = nullptr;
	}
	else
#endif
	{
		find("/dev/dri/card0");
	}
}

ScreenBlankHandler::~ScreenBlankHandler()
{
	if (drm_fd >= 0)
	{
		close(drm_fd);
	}
}

ScreenBlankHandler::operator QScreen * () const
{
	return screen;
}

void ScreenBlankHandler::autoBlank(bool auto_blank_active)
{
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	if (display != nullptr)
	{
		int result;

		if (auto_blank_active)
		{
			result = DPMSEnable(display);
		}
		else
		{
			result = DPMSDisable(display);
		}
		qDebug().noquote() << "Auto blank:" << auto_blank_active << display << result;

		XCloseDisplay(display);
	}
	else
	{
		qDebug("No X11 screen saver!");
	}
#endif
}

void ScreenBlankHandler::resetBlank()
{
}

void ScreenBlankHandler::blank(bool blank_active)
{
#if 0
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
#else
	if (dpms_active)
	{
		const int res = drmModeConnectorSetProperty(drm_fd, connector_id,
				dpms_id, blank_active ? DRM_MODE_DPMS_OFF : DRM_MODE_DPMS_ON);

		if (res == 0)
		{
			qDebug().noquote() << "Auto blank:" << blank_active;
		}
		else
		{
			qCritical() << connector_id << dpms_id << strerror(errno) << res;
		}
	}
#endif
}

void ScreenBlankHandler::find(const char * dri_device)
{
	int fd = open(dri_device, O_RDWR);
	if (fd >= 0)
	{
		drmModeRes * mode_res = drmModeGetResources(fd);

		const bool is_master = drmAuthMagic(fd, 0) != -EACCES;
		qDebug().noquote() << "Is master:" << is_master << fd;

		if (mode_res != nullptr)
		{
			for (int i = 0; i < mode_res->count_connectors; ++i)
			{
				drmModeConnector * conn = drmModeGetConnector(fd, mode_res->connectors[i]);

				if (conn != nullptr)
				{
					if (conn->connection == DRM_MODE_CONNECTED)
					{
						drmModeObjectProperties * properties;

						properties = drmModeObjectGetProperties(fd, conn->connector_id, DRM_MODE_OBJECT_CONNECTOR);
						if (properties != nullptr)
						{
							for (unsigned p = 0; p < properties->count_props; p++)
							{
								drmModePropertyRes * property_res = drmModeGetProperty(fd, properties->props[p]);

								if (property_res != nullptr)
								{
									if (strcmp(property_res->name, "DPMS") == 0)
									{
										drm_fd       = fd;
										connector_id = conn->connector_id;
										dpms_id      = property_res->prop_id;
										dpms_active  = is_master;
										qDebug().noquote() << "DPMS found:" << dpms_active;
									}
									drmModeFreeProperty(property_res);
								}
								else
								{
									qDebug() << "No property found for ID:" << properties->props[p];
								}
							}
							drmModeFreeObjectProperties(properties);
						}
						else
						{
							qDebug().noquote() << "No properties found for connector ID:" << conn->connector_id;
						}
					}
					drmModeFreeConnector(conn);
				}
				else
				{
					qDebug("No connector found.");
				}
			}
			drmModeFreeResources(mode_res);
		}
		else
		{
			qDebug("No resources found.");
		}

		if (drm_fd < 0)
		{
			close(fd);
		}
	}
}
