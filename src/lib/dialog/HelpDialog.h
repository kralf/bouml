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

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <qdialog.h>

class Q3TextBrowser;

// singleton

class HelpDialog : public QDialog {
  Q_OBJECT
    
  public:
    static void show(QString);
    
  private:
    HelpDialog();
    virtual ~HelpDialog();
    
    static HelpDialog * the;
  
    Q3TextBrowser * br;
};

extern QString manual_dir();
extern void set_manual_dir(QString);

extern QString navigator_path();
extern void set_navigator_path(QString);

#endif
