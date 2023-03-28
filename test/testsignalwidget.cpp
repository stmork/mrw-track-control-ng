//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testsignalwidget.h"

using namespace mrw::test;
using namespace mrw::model;
using namespace mrw::ui;

using LockState = Device::LockState;
using Bending   = Position::Bending;
using Symbol    = Signal::Symbol;

TestSignalWidget::TestSignalWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
}

void TestSignalWidget::testPrepare()
{
	for (const LockState lock :
		{
			LockState::FAIL, LockState::UNLOCKED, LockState::PENDING, LockState::LOCKED
		})
	{
		mock.setLock(lock);
		for (const SectionState state :
			{
				FREE, SHUNTING, TOUR, OCCUPIED, PASSED
			})
		{
			mock.setSectionState(state);
			for (const Bending bending :
				{
					Bending::LEFT, Bending::STRAIGHT, Bending::RIGHT
				})
			{
				mock.setBending(bending);
				for (const bool dir :
					{
						true, false
					})
				{
					mock.setDirection(dir);
					for (int ext = 0; ext < 5; ext++)
					{
						mock.setExtension(ext);

						widget.prepare(status);
						QCOMPARE(status.direction, dir);
						QCOMPARE(status.section_state, state);
						QCOMPARE(status.lock_state, lock);
						QCOMPARE(status.bending, bending);
						QCOMPARE(status.extensions, ext);
						QCOMPARE(status.lines, 0);
					}
				}
			}
		}
	}
}

void TestSignalWidget::testSh0()
{
	mock.setMain(false);
	mock.setDistant(false);
	mock.setShunting();
	mock.setShuntStop();

	for (const Symbol distant :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setDistantSymbol(distant);

		for (const Symbol main :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setMainSymbol(main);

			widget.prepare(status);

			QCOMPARE(status.has_main,     false);
			QCOMPARE(status.has_distant,  false);
			QCOMPARE(status.has_shunting, true);

			QCOMPARE(status.main_state,    main);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   Symbol::STOP);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   true);

			QCOMPARE(status.mast_color,    BaseWidget::RED);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testSh1()
{
	mock.setMain(false);
	mock.setDistant(false);
	mock.setShunting();
	mock.setShuntGo();

	for (const Symbol distant :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setDistantSymbol(distant);

		for (const Symbol main :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setMainSymbol(main);

			widget.prepare(status);

			QCOMPARE(status.has_main,     false);
			QCOMPARE(status.has_distant,  false);
			QCOMPARE(status.has_shunting, true);

			QCOMPARE(status.main_state,    main);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   Symbol::GO);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   true);

			QCOMPARE(status.mast_color,    BaseWidget::RED);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::WHITE);
		}
	}
}

void TestSignalWidget::testVr0()
{
	mock.setSectionState(TOUR);
	mock.setMain(false);
	mock.setDistant();
	mock.setShunting(false);
	mock.setDistantStop();

	for (const Symbol shunt :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setShuntSymbol(shunt);

		for (const Symbol main :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setMainSymbol(main);

			widget.prepare(status);

			QCOMPARE(status.has_main,     false);
			QCOMPARE(status.has_distant,  true);
			QCOMPARE(status.has_shunting, false);

			QCOMPARE(status.main_state,    main);
			QCOMPARE(status.distant_state, Symbol::STOP);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, true);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::YELLOW);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testVr1()
{
	mock.setSectionState(TOUR);
	mock.setMain(false);
	mock.setDistant();
	mock.setShunting(false);
	mock.setDistantGo();

	for (const Symbol shunt :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setShuntSymbol(shunt);

		for (const Symbol main :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setMainSymbol(main);

			widget.prepare(status);

			QCOMPARE(status.has_main,     false);
			QCOMPARE(status.has_distant,  true);
			QCOMPARE(status.has_shunting, false);

			QCOMPARE(status.main_state,    main);
			QCOMPARE(status.distant_state, Symbol::GO);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, true);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::GREEN);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::GREEN);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp0()
{
	mock.setSectionState(TOUR);
	mock.setMain();
	mock.setDistant(false);
	mock.setShunting(false);
	mock.setMainStop();

	for (const Symbol shunt :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setShuntSymbol(shunt);

		for (const Symbol distant :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setDistantSymbol(distant);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  false);
			QCOMPARE(status.has_shunting, false);

			QCOMPARE(status.main_state,    Symbol::STOP);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::RED);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp1()
{
	mock.setSectionState(TOUR);
	mock.setMain();
	mock.setDistant(false);
	mock.setShunting(false);
	mock.setMainGo();

	for (const Symbol shunt :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setShuntSymbol(shunt);

		for (const Symbol distant :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setDistantSymbol(distant);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  false);
			QCOMPARE(status.has_shunting, false);

			QCOMPARE(status.main_state,    Symbol::GO);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::GREEN);
			QCOMPARE(status.main_color,    BaseWidget::GREEN);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp0WithDistant()
{
	mock.setSectionState(TOUR);
	mock.setMain();
	mock.setDistant();
	mock.setMainStop();
	mock.setShuntStop();

	for (const bool has_shunt :
		{
			false, true
		} )
	{
		mock.setShunting(has_shunt);

		for (const Symbol distant :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setDistantSymbol(distant);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  true);
			QCOMPARE(status.has_shunting, has_shunt);

			QCOMPARE(status.main_state,    Symbol::STOP);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   Symbol::STOP);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   has_shunt);

			QCOMPARE(status.mast_color,    BaseWidget::RED);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp1Vr0()
{
	mock.setSectionState(TOUR);
	mock.setMain();
	mock.setDistant();
	mock.setDistantStop();
	mock.setMainGo();

	for (const bool has_shunt :
		{
			false, true
		} )
	{
		mock.setShunting(has_shunt);

		for (const Symbol shunt :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setShuntSymbol(shunt);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  true);
			QCOMPARE(status.has_shunting, has_shunt);

			QCOMPARE(status.main_state,    Symbol::GO);
			QCOMPARE(status.distant_state, Symbol::STOP);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, true);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::YELLOW);
			QCOMPARE(status.main_color,    BaseWidget::GREEN);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp1Vr1()
{
	mock.setSectionState(TOUR);
	mock.setMain();
	mock.setDistant();
	mock.setDistantGo();
	mock.setMainGo();

	for (const bool has_shunt :
		{
			false, true
		} )
	{
		mock.setShunting(has_shunt);

		for (const Symbol shunt :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setShuntSymbol(shunt);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  true);
			QCOMPARE(status.has_shunting, has_shunt);

			QCOMPARE(status.main_state,    Symbol::GO);
			QCOMPARE(status.distant_state, Symbol::GO);
			QCOMPARE(status.shunt_state,   shunt);

			QCOMPARE(status.draw_distant, true);
			QCOMPARE(status.draw_shunt,   false);

			QCOMPARE(status.mast_color,    BaseWidget::GREEN);
			QCOMPARE(status.main_color,    BaseWidget::GREEN);
			QCOMPARE(status.distant_color, BaseWidget::GREEN);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp00()
{
	mock.setMain();
	mock.setShunting();
	mock.setMainStop();
	mock.setShuntStop();

	for (const bool has_distant :
		{
			false, true
		} )
	{
		mock.setDistant(has_distant);

		for (const Symbol distant :
			{
				Symbol::OFF, Symbol::STOP, Symbol::GO
			})
		{
			mock.setDistantSymbol(distant);

			widget.prepare(status);

			QCOMPARE(status.has_main,     true);
			QCOMPARE(status.has_distant,  has_distant);
			QCOMPARE(status.has_shunting, true);

			QCOMPARE(status.main_state,    Symbol::STOP);
			QCOMPARE(status.distant_state, distant);
			QCOMPARE(status.shunt_state,   Symbol::STOP);

			QCOMPARE(status.draw_distant, false);
			QCOMPARE(status.draw_shunt,   true);

			QCOMPARE(status.mast_color,    BaseWidget::RED);
			QCOMPARE(status.main_color,    BaseWidget::RED);
			QCOMPARE(status.distant_color, BaseWidget::YELLOW);
			QCOMPARE(status.shunt_color,   BaseWidget::RED);
		}
	}
}

void TestSignalWidget::testHp0Sh1()
{
	mock.setSectionState(SHUNTING);
	mock.setMain();
	mock.setDistant(false);
	mock.setShunting();
	mock.setMainStop();
	mock.setShuntGo();

	for (const Symbol distant :
		{
			Symbol::OFF, Symbol::STOP, Symbol::GO
		})
	{
		mock.setDistantSymbol(distant);

		widget.prepare(status);

		QCOMPARE(status.has_main,     true);
		QCOMPARE(status.has_distant,  false);
		QCOMPARE(status.has_shunting, true);

		QCOMPARE(status.main_state,    Symbol::STOP);
		QCOMPARE(status.distant_state, distant);
		QCOMPARE(status.shunt_state,   Symbol::GO);

		QCOMPARE(status.draw_distant, false);
		QCOMPARE(status.draw_shunt,   true);

		QCOMPARE(status.mast_color,    BaseWidget::RED);
		QCOMPARE(status.main_color,    BaseWidget::RED);
		QCOMPARE(status.distant_color, BaseWidget::YELLOW);
		QCOMPARE(status.shunt_color,   BaseWidget::WHITE);
	}
}