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

#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include <q3listview.h>
#include <qdir.h>
//Added by qt3to4:
#include <QKeyEvent>

class BrowserNode;

class BrowserView : public Q3ListView {
  Q_OBJECT
    
  protected:
    static BrowserView * the;
    static QDir dir;
  
  public:
    BrowserView(QWidget * parent);
virtual ~BrowserView();
  
    void set_project(QDir dir);
    void close();
    
    static void select(Q3ListViewItem * b);
    static BrowserNode * get_project();
    static QDir & get_dir() { return dir; }
    
  protected:
    virtual void keyPressEvent(QKeyEvent *);
    
  protected slots:
    void rightPressed(Q3ListViewItem *);
};

#endif
