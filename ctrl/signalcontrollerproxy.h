//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLERPROXY_H
#define MRW_CTRL_SIGNALCONTROLLERPROXY_H

#include <model/signal.h>
#include <ctrl/signalcontroller.h>

namespace mrw::ctrl
{
	class SignalControllerProxy : public SignalController
	{
		Q_OBJECT

	private:
		mrw::model::Signal * signal = nullptr;

	public:
		explicit SignalControllerProxy(QObject * parent = nullptr);

		void setSignal(mrw::model::Signal * new_signal);

		virtual bool isDirection() const override;
		virtual bool hasShunting() const override;
		virtual bool hasDistant() const override;
		virtual bool hasMain() const override;
		virtual QString name() const override;
	};
}

#endif
