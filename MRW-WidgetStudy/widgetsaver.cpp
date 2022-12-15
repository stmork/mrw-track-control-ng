//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/method.h>

#include "widgetsaver.h"

using namespace mrw::util;
using namespace mrw::model;

const char * WidgetSaver::subdir = "images";

WidgetSaver::WidgetSaver(QWidget * parent) : QWidget(parent)
{
	const QString base = QCoreApplication::applicationDirPath();

	dir = base;
	dir.mkdir(subdir);
}

void WidgetSaver::save()
{
	__METHOD__;

	const QString filename = dir.absoluteFilePath(QString(subdir) + "/" + name() + ".jpg");
	widget()->grab().save(filename);
}

QChar WidgetSaver::direction(const bool dir)
{
	return dir ? 'R' : 'L';
}

QChar WidgetSaver::lockState(const mrw::model::Device::LockState & state)
{
	return Device::get(state).at(0);
}

QChar WidgetSaver::sectionState(const SectionState & state)
{
	return Section::get(state).at(0);
}
