//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "dumphandler.h"

using namespace mrw::util;

DumpHandler::DumpHandler(SignalCallback dump_callback) :
	SignalHandler( { SIGQUIT }, dump_callback)
{
}
