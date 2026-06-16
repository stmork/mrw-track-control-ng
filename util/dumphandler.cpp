//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include "dumphandler.h"

using namespace mrw::util;

DumpHandler::DumpHandler(SignalCallback dump_callback) :
	SigCallbackHandler( { SIGQUIT }, dump_callback )
{
}
