//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	is_curve(ModelRailway::boolean(element, "istAbzweig"))
{
}

void Rail::link() noexcept
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

bool Rail::valid() const noexcept
{
	return (
			((a != nullptr) && a->contains(this,  aIsDir())) ||
			((b != nullptr) && b->contains(this, !aIsDir()))
		);
}

bool Rail::isCurved() const noexcept
{
	return is_curve;
}

bool Rail::isMain(const Rail * rail) noexcept
{
	return rail->is_main;
}

const QString & Rail::name() const noexcept
{
	return partName();
}

QString Rail::toString() const noexcept
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

QString Rail::key() const noexcept
{
	return "Gleis" + name();
}

bool Rail::setState(const RailPart * prev, const RailPart * succ) noexcept
{
	Q_UNUSED(prev);
	Q_UNUSED(succ);

	// This RailPart has no changing "switch" state.
	return true;
}
