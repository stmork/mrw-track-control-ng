//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <model/modelrailway.h>

using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	for (int i = 1; i < argc; i++)
	{
		ModelRailway model(argv[i]);

		qInfo().noquote() << "===== Modelrailway:" << argv[i];
		model.info();
	}
	return EXIT_SUCCESS;
}
