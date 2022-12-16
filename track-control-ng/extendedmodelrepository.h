//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef EXTENDEDMODELREPOSITORY_H
#define EXTENDEDMODELREPOSITORY_H

#include <model/modelrepository.h>

class ExtendedModelRepository : public mrw::model::ModelRepository
{
public:
	explicit ExtendedModelRepository(const QString & modelname);
};

#endif // EXTENDEDMODELREPOSITORY_H
