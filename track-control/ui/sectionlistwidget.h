//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SECTIONLISTWIDGET_H
#define MRW_UI_SECTIONLISTWIDGET_H

#include <QListWidget>

#include <ctrl/basecontroller.h>
#include <ctrl/railpartinfo.h>

namespace mrw::ui
{
	typedef std::function<void(
		mrw::ctrl::BaseController *)> ControllerCallback;
	typedef std::function<void(
		mrw::ctrl::BaseController *,
		mrw::model::Position *)> PositionCallback;

	class SectionListWidget : public QListWidget
	{
		Q_OBJECT
	public:
		explicit SectionListWidget(QWidget * parent = nullptr);

		void traverse(ControllerCallback  callback);
		void traverse(PositionCallback    callback);
		void collect(std::vector<mrw::ctrl::RailPartInfo *> & rails);


		template <class T> size_t countType()
		{
			size_t count = 0;

			traverse([&] (mrw::ctrl::BaseController * ctrl)
			{
				const T * ptr = dynamic_cast<T *>(ctrl);

				if (ptr != nullptr)
				{
					count++;
				}
			}
			);
			return count;
		}
	};
}

#endif
