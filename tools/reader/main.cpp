//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/method.h>
#include <model/modelrailway.h>

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Method::pattern();
	for (int i = 1; i < argc; i++)
	{
		ModelRailway model(argv[i]);

		qInfo().noquote() << "===== Modelrailway:" << argv[i];
		model.info();
	}
	return EXIT_SUCCESS;
}
