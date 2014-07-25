// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************





#include <qpainter.h>

#include "MenuTitle.h"

MenuTitle::MenuTitle(const QString & s, QFont f) : str(s), font(f) {
  font.setBold(TRUE);
}

void MenuTitle::paint(QPainter * p, const QColorGroup& /*cg*/, bool /*act*/,
		      bool /*enabled*/, int x, int y, int w, int h) {
  p->setFont (font);
  p->drawText( x, y, w, h, Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip, str);
}

QSize MenuTitle::sizeHint() {
  return QFontMetrics(font).size(Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip, str);
}
