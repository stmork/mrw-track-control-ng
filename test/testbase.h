//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTBASE_H
#define MRW_TEST_TESTBASE_H

#if (QT_VERSION < QT_VERSION_CHECK(6, 3, 0))
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_EXCEPTION_THROWN(condition, exception);
#else
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_THROWS_EXCEPTION(exception, condition);
#endif

#endif
