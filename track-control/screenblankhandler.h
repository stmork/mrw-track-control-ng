//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef SCREENBLANKHANDLER_H
#define SCREENBLANKHANDLER_H

#ifdef DRM_SCREEN_SAVER
#include <xf86drm.h>
#include <xf86drmMode.h>
#endif

#include <util/self.h>

#include <statecharts/OperatingModeStatechart.h>

class QScreen;

class ScreenBlankHandler :
	public mrw::util::Self<mrw::statechart::OperatingModeStatechart::Screen::OperationCallback>
{
	QScreen * screen       = nullptr;
#ifdef DRM_SCREEN_SAVER
	int       drm_fd       = 0;
	int       lease_fd     = 0;
	uint32_t  lease_id     = 0;
	uint32_t  connector_id = 0;
	uint32_t  dpms_id      = 0;
	bool      dpms_active  = false;
#endif

public:
	ScreenBlankHandler();
	virtual ~ScreenBlankHandler();

	operator QScreen * () const;

protected:
	// Implementation for Screen::OperationCallback
	virtual void          autoBlank(bool active) override;
	virtual void          resetBlank() override;
	virtual void          blank(bool active) override;

#ifdef DRM_SCREEN_SAVER
private:
	void findDevice();
	bool initDevice(int fd);
	void initDevice(int fd, drmModeRes * mode_res);
	void findDpmsProperty(int fd, drmModeConnector * conn);
	void initDPMS();

#endif
};

#endif
