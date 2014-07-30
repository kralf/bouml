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





#include <qlayout.h>
#include <qlabel.h>
#include <q3textview.h> 
#include <qpushbutton.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QPixmap>
#include <Q3HBoxLayout>

#include "About.h"
#include "Config.h"
#include "bp_xpm.xpm"
#include "UmlDesktop.h"
#include "translate.h"

AboutDialog::AboutDialog() : QDialog(0, "About " PROJECT_NAME, TRUE) {
  setCaption(TR("About " PROJECT_NAME));
  //move(p);
  
  Q3VBoxLayout * vbox = new Q3VBoxLayout(this);  
  Q3HBoxLayout * hbox; 
  
  vbox->setMargin(5);
  
  hbox = new Q3HBoxLayout(vbox); 
  hbox->setMargin(5);

  QPixmap bp((const char **) bp_xpm);
  QLabel * lbp = new QLabel(this);
  
 // lbp->setPixmap(bp);
//  hbox->addWidget(lbp);
  hbox->addWidget(new QLabel("  ", this));

  QString htmltext;
  htmltext.sprintf("%s <b>%d.%d-%d</b> release <b>%s</b><br>\n"
                   "<br>\n"
                   "<i>%s</i><br>\n"
                   "<br>\n"
                   "%s (<i>%s</i>)",
    PROJECT_NAME, PROJECT_MAJOR, PROJECT_MINOR, PROJECT_REVISION, PROJECT_RELEASE,
    PROJECT_HOME, PROJECT_ADMIN, PROJECT_CONTACT);
                            
  Q3TextView * tx =
    new Q3TextView(htmltext, QString::null, this);
  QFont fnt = tx->font();
  
  fnt.setItalic(TRUE);
  
  QFontMetrics fm(fnt);
  
  tx->setVScrollBarMode(Q3ScrollView::AlwaysOff);
  tx->setHScrollBarMode(Q3ScrollView::AlwaysOff);
  tx->setMinimumSize(fm.size(0, htmltext));
  hbox->addWidget(tx);
  
  hbox = new Q3HBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * ok = new QPushButton(TR("&OK"), this);
  
  ok->setDefault( TRUE );
  
  hbox->addWidget(new QLabel(this));
  hbox->addWidget(ok);
  hbox->addWidget(new QLabel(this));
  
  if (UmlDesktop::fixed())
    UmlDesktop::tocenter(this);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
}

