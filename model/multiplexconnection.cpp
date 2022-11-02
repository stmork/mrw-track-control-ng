//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QStringList>

#include "util/stringutil.h"
#include "model/mrwmodel.h"
#include "model/multiplexconnection.h"
#include "model/lightsignal.h"

using namespace mrw::model;

MultiplexConnection::MultiplexConnection(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	model(model_railway),
	reference(element),
	connection_id(ModelRailway::value(element, "nummer"))
{
}

void mrw::model::MultiplexConnection::link()
{
	QStringList references = reference.attribute("lichtsignale").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & signal_reference : references)
	{
		LightSignal * signal = static_cast<LightSignal *>(RailPart::resolve(model, signal_reference));

		light_signals.push_back(signal);
	}
}
