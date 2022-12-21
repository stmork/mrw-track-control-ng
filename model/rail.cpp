//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/rail.h"

using namespace mrw::model;

Rail::Rail(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	RailPart(model_railway, model_section, element),
	is_main(  ModelRailway::boolean(element, "istHauptgleis")),
	is_branch(ModelRailway::boolean(element, "istAbzweig"))
{
}

void Rail::link()
{
	a = resolve("a");
	b = resolve("b");

	if ((a == nullptr) && (b == nullptr))
	{
		part_model->error("Rail not connected: " + name());
	}

	if (a != nullptr)
	{
		advance(aIsDir()).insert(RailInfo(a));
	}

	if (b != nullptr)
	{
		advance(!aIsDir()).insert(RailInfo(b));
	}
}

bool Rail::valid() const
{
	return (
			((a != nullptr) && a->contains(this,  aIsDir())) ||
			((b != nullptr) && b->contains(this, !aIsDir()))
		);
}

bool Rail::isBranch() const
{
	return is_branch;
}

const QString & Rail::name() const
{
	return partName();
}

QString Rail::toString() const
{
	static const QString R = String::format(String::BOLD_ON + String::RED_ON, "R");

	return QString("      # %1%2 %3--%4 :        %5").
		arg(valid() ? "V" : "-").
		arg(reserved() ? R : "-").
		arg(aIsDir() ?
			(b == nullptr ? " |" : " b") :
			(a == nullptr ? " |" : " a")).
		arg(aIsDir() ?
			(a == nullptr ? "| " : "a ") :
			(b == nullptr ? "| " : "b ")).
		arg(name());
}

QString Rail::key() const
{
	return "Gleis" + name();
}

void Rail::setState(const RailPart * left, const RailPart * right)
{
	Q_UNUSED(left);
	Q_UNUSED(right);

	// This RailPart has no changing "switch" state.
}
