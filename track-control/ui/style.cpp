//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <QApplication>
#include <QPalette>
#include <QWidget>

#include "style.h"

using namespace mrw::ui;

const QString Style::css(R"(
QListView {
	border: 1px solid white;
}

QTabBar::tab {
	border: 1px solid gray;
	padding: 2px;
}

QTabBar::tab:selected {
	border: 2px solid white;
}

QTabWidget::pane {
	border: 1px solid white;
}

QPushButton {
	border: 2px solid white;
}

QPushButton::disabled {
	border: 1px solid white;
}
)");

// set black background
// Qt::black / "#000000" / "black"
const QPalette Style::background_color(QPalette::Window, Qt::black);

void Style::setEstwStyle(QApplication & app)
{
	QApplication::setStyle("Fusion");

	QPalette p;

	p.setColor(QPalette::Window, Qt::black);
	p.setColor(QPalette::WindowText, Qt::white);

	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::AlternateBase, Qt::black);

	p.setColor(QPalette::Text, Qt::white);
	p.setColor(QPalette::PlaceholderText, Qt::white);

	p.setColor(QPalette::Button, Qt::black);
	p.setColor(QPalette::ButtonText, Qt::white);

	p.setColor(QPalette::Light, QColor(128,128,128));
	p.setColor(QPalette::Midlight, QColor(96,96,96));
	p.setColor(QPalette::Mid, QColor(64,64,64));
	p.setColor(QPalette::Dark, QColor(32,32,32));
	p.setColor(QPalette::Shadow, Qt::black);

	p.setColor(QPalette::Highlight, QColor(0,120,215));
	p.setColor(QPalette::HighlightedText, Qt::white);

	p.setColor(QPalette::Link, QColor(80,160,255));
	p.setColor(QPalette::LinkVisited, QColor(180,100,255));

	// deaktivierte Widgets
	p.setColor(QPalette::Disabled, QPalette::WindowText, Qt::gray);
	p.setColor(QPalette::Disabled, QPalette::Text, Qt::gray);
	p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::gray);

	app.setPalette(p);
	app.setStyleSheet(css);
}

void Style::setEstwStyle(QWidget * widget)
{
	widget->setPalette(background_color);
}
