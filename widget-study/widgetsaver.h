//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef WIDGETSAVER_H
#define WIDGETSAVER_H

#include <QWidget>
#include <QDir>

#include <model/device.h>
#include <model/section.h>

class WidgetSaver : public QWidget
{
	Q_OBJECT

	QDir dir;

	static const char * subdir;

public:
	explicit WidgetSaver(QWidget * parent = nullptr);

	virtual void save();

	virtual QWidget * widget() const = 0;
	virtual QString   name() const = 0;

	static QChar direction(const bool dir);
	static QChar lockState(const mrw::model::Device::LockState & state);
	static QChar sectionState(const mrw::model::SectionState & state);

protected:
	QString path(const QString & name) const;
};

#endif
