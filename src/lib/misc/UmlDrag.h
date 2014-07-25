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

#ifndef UMLDRAG_H
#define UMLGRAG_H

#include <q3dragobject.h>
#include <qstring.h>
//Added by qt3to4:
#include <QDragMoveEvent>
#include <QDropEvent>

#include "UmlEnum.h"

class BrowserNode;

class UmlDrag : public Q3StoredDrag {
  private:
    static QString postfix;
    static bool ro;
  
  public:
    static const QString Key;

    UmlDrag(BrowserNode * bn, QWidget * parent = 0, const char * name = 0);
virtual ~UmlDrag() {};

    static bool canDecode(QDragMoveEvent * e, UmlCode type,
			  bool withpostfix = FALSE,
			  bool evenro = FALSE);
    static bool canDecode(QDragMoveEvent * e, const QString & type);
    static BrowserNode * decode(QDropEvent * e, UmlCode type, bool withpostfix = FALSE);
    static BrowserNode * decode(QDropEvent * e, const QString & type);
};

#endif
