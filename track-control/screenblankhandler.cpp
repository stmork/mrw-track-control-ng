//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <filesystem>
#include <regex>

#include <fcntl.h>
#include <unistd.h>

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

#include "screenblankhandler.h"

#ifdef QT_SCREEN_SAVER
#include <qpa/qplatformscreen.h>
#endif

#ifdef X11_SCREEN_SAVER
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>
#endif

namespace fs = std::filesystem;

ScreenBlankHandler::ScreenBlankHandler()
{
	screen  = QGuiApplication::primaryScreen();
#ifdef X11_SCREEN_SAVER
	Display * display = XOpenDisplay(nullptr);

	qCDebug(log, "Using X11 screen saver.");
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
#ifdef DRM_SCREEN_SAVER
		qCDebug(log, "Using DRM screen saver.");
#if 1
		findDevice();
#else
		initDevice(5);
#endif
#endif
	}
#ifdef QT_SCREEN_SAVER
	qCDebug(log, "Using Qt private API screen saver.");
#endif
}

ScreenBlankHandler::~ScreenBlankHandler()
{
#ifdef DRM_SCREEN_SAVER
	if (drm_fd >= 0)
	{
		if (lease_id != 0)
		{
			drmModeRevokeLease(drm_fd, lease_id);
		}
		close(drm_fd);
	}
#endif
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
	qDebug().noquote() << "DPMS blank:" << blank_active;
#ifdef QT_SCREEN_SAVER
	if (screen != nullptr)
	{
		QPlatformScreen * handle = screen->handle();

		if (handle != nullptr)
		{
#if 1
			handle->setPowerState(blank_active ? QPlatformScreen::PowerStateOff : QPlatformScreen::PowerStateOn);
			return;
#endif
		}
	}
#endif

#ifdef DRM_SCREEN_SAVER
	if (dpms_active)
	{
		const int res = drmModeConnectorSetProperty(lease_fd, connector_id,
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

#ifdef DRM_SCREEN_SAVER
void ScreenBlankHandler::findDevice()
{
	static const std::regex     pattern(R"(card\d+$)");
	std::smatch    match;

	for (auto const & dir_entry : fs::directory_iterator{"/dev/dri"})
	{
		const std::string filename = dir_entry.path().filename().string();

		if (std::regex_search(filename, match, pattern))
		{
			const char * device_name = dir_entry.path().c_str();

			qDebug().noquote() << "DRM opening:  " << device_name;
			int fd = open(device_name, O_RDWR);

			if (fd >= 0)
			{
				if (initDevice(fd))
				{
					return;
				}
				else
				{
					close(fd);
				}
			}
			else
			{
				qCritical().noquote() << "Cannot open DRI device" << device_name << "Error:" << strerror(errno);
			}
		}
	}
}

bool ScreenBlankHandler::initDevice(int fd)
{
	drmModeRes * mode_res = drmModeGetResources(fd);

	if (mode_res == nullptr)
	{
		qWarning("No DRM resources found.");
	}
	else
	{
		initDevice(fd, mode_res);
	}
	return mode_res != nullptr;
}

void ScreenBlankHandler::initDevice(int fd, drmModeRes * mode_res)
{
	assert(mode_res != nullptr);

	for (int i = 0; i < mode_res->count_connectors; ++i)
	{
		drmModeConnector * conn = drmModeGetConnector(fd, mode_res->connectors[i]);

		if (conn != nullptr)
		{
			if (conn->connection == DRM_MODE_CONNECTED)
			{
				findDpmsProperty(fd, conn);
			}
			drmModeFreeConnector(conn);
		}
		else
		{
			qWarning("No connector found.");
		}
	}
	drmModeFreeResources(mode_res);
}

void ScreenBlankHandler::findDpmsProperty(int fd, drmModeConnector * conn)
{
	drmModeObjectProperties * properties;
	drm_magic_t               magic{};

	qDebug().noquote() << "DRM Magic:    " << drmGetMagic(fd, &magic);
	qDebug().noquote() << "DRM is master:" << drmIsMaster(fd);

	const bool is_master = drmAuthMagic(fd, magic) != -EACCES;
	qDebug().noquote() << "DRM is auth:  " << is_master << fd << drmSetMaster(fd);

	assert(conn != nullptr);
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

					initDPMS();
				}
				drmModeFreeProperty(property_res);
			}
			else
			{
				qWarning() << "No property found for ID:" << properties->props[p];
			}
		}
		drmModeFreeObjectProperties(properties);
	}
	else
	{
		qWarning().noquote() << "No properties found for connector ID:" << conn->connector_id;
	}
}

void ScreenBlankHandler::initDPMS()
{
	if (!dpms_active)
	{
		std::vector<uint32_t> objects;

		objects.emplace_back(connector_id);
		objects.emplace_back(dpms_id);

		lease_fd = drmModeCreateLease(drm_fd, objects.data(), objects.size(), O_CREAT, &lease_id);
		if (lease_fd >= 0)
		{
			dpms_active = true;
			qDebug().noquote() << "DRM lease:" << lease_fd << lease_id;
		}
		else
		{
			qCritical().noquote() << "No DRM lease available!" << strerror(errno);
		}
	}
	else
	{
		lease_fd = drm_fd;
	}
	qDebug().noquote() << "DPMS found:   " << dpms_active;
}
#endif
