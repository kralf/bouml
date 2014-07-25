
#ifdef WIN32
#include <stdlib.h>
#endif
#include "ConstrDestrCopyDialog.h"
#include "UmlClass.h"

#include <qlist.h>
#include <q3ptrlist.h>
#include <q3hgroupbox.h> 
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <q3hbox.h>
#include <q3vbox.h> 
#include <qdir.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
ConstrDestrCopyDialog::ConstrDestrCopyDialog(UmlClass * cl, bool have_constructor,
                                  bool have_destructor, bool have_copy,
                                  bool have_const_copy, bool have_assignment,
                                  bool have_const_assignment)
   : QDialog(0, 0, TRUE), target(cl) {
  Q3VBoxLayout * vbox = new Q3VBoxLayout(this);
  Q3HGroupBox * g;
  Q3PtrList<QLabel> labels;
  QLabel * lbl;

  vbox->setSpacing(5);
  vbox->setMargin(5);
  
  lbl = new QLabel(QString("<big><b>C++ utilities for the class <i><tt>") + 
		   (const char *) cl->name() + "</tt></i></b></big>",
		   this);
  lbl->setAlignment(Qt::AlignCenter);
  vbox->addWidget(lbl);

  // constructor
  
  g = new Q3HGroupBox(this);
  vbox->addWidget(g);
  
  labels.append(new QLabel((have_constructor)
			   ? "the class already have contructor  "
			   : "the class doesn't have contructor  ",
			   g));
  
  Q3HBox * h = new Q3HBox(g);
    
  add_constr = new QCheckBox("add constructor", h);
  constr_explicit = new QCheckBox("explicit", h);
  
  // destructor
  
  if (! have_destructor) {
    g = new Q3HGroupBox(this);
    vbox->addWidget(g);
  
    labels.append(new QLabel("the class doesn't have destructor  ",
			     g));
    
    h = new Q3HBox(g);
    
    add_destr = new QCheckBox("add destructor", h);
    virtual_destr = new QCheckBox("virtual", h);
  }
  else
    add_destr = 0;
  
  // copy contructor

  if (have_copy) {
    add_copy = 0;
    if (!have_const_copy) {
      g = new Q3HGroupBox(this);
      vbox->addWidget(g);
      
      labels.append(new QLabel("the class doesn't have copy contructor  \nwith const argument  ",
			       g));
      add_const_copy = new QCheckBox("add copy constructor\nwith const argument",
				     g);
    }
    else
      add_const_copy = 0;
  }
  else if (!have_const_copy) {
    g = new Q3HGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have copy contructor  ",
			     g));
    
    Q3VBox * v = new Q3VBox(g);
    
    add_const_copy = new QCheckBox("add copy constructor\nwith const argument",
				   v);      
    add_copy = new QCheckBox("add copy constructor\nwith non const argument",
			     v);
  }
  else {
    g = new Q3HGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have copy contructor  \nwith non const argument  ",
			     g));
    add_copy = new QCheckBox("add copy constructor\nwith non const argument",
			     g);
    add_const_copy = 0;
  }

  // assignment

  if (have_assignment) {
    add_assign = 0;
    if (!have_const_assignment) {
      g = new Q3HGroupBox(this);
      vbox->addWidget(g);
    
      labels.append(new QLabel("the class doesn't have assignment\noperator with const argument  ",
			       g));
      add_const_assign = new QCheckBox("add assignment\nwith const argument",
				       g);
    }
    else
      add_const_assign = 0;
  }
  else if (!have_const_assignment) {
    g = new Q3HGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have assignment operator  ", g));
    
    Q3VBox * v = new Q3VBox(g);
    
    add_const_assign = new QCheckBox("add assignment\nwith const argument",
				     v);      
    add_assign = new QCheckBox("add assignment\nwith non const argument",
			       v);
  }
  else {
    g = new Q3HGroupBox(this);
    vbox->addWidget(g);
    
    labels.append(new QLabel("the class doesn't have assignment operator  \nwith non const argument  ",
			     g));
    add_assign = new QCheckBox("add assignment\nwith non const argument",
			       g);
    add_const_assign = 0;
  }

  // use the same width for all the labels on the first column

  QSize sz(labels.first()->sizeHint());
  
  while ((lbl = labels.next()) != 0) {
    if (lbl->sizeHint().width() > sz.width())
      sz.setWidth(lbl->sizeHint().width());
  }

  for (lbl = labels.first(); lbl != 0; lbl = labels.next()) {
    sz.setHeight(lbl->sizeHint().height());
    lbl->setFixedSize(sz);
  }
    
  // ok & cancel buttons
  
  Q3HBox * hbox = new Q3HBox(this);
  
  vbox->addWidget(hbox);
  
  QPushButton * ok = new QPushButton("&OK", hbox);
  QPushButton * cancel = new QPushButton("&Cancel", hbox);

  QSize bs(cancel->sizeHint());
  
  ok->setDefault(TRUE);
  
  ok->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ConstrDestrCopyDialog::polish() {
  QDialog::polish();
  
  // try to read .boumlrc
  // note : QFile fp(QDir::home().absFilePath(".boumlrc")) doesn't work
  // if the path contains non latin1 characters, for instance cyrillic !
  QString s = QDir::home().absFilePath(".boumlrc");
  FILE * fp = fopen((const char *) s, "r");

#ifdef WIN32
  if (fp == 0) {
    QString hd = getenv("USERPROFILE");
    
    if (! hd.isEmpty()) {
      QDir d(hd);
      QString s2 = d.absFilePath(".boumlrc");
      
      fp = fopen((const char *) s2, "r");
    }
  }
#endif
  
  if (fp != 0) {
    char line[512];
      
    while (fgets(line, sizeof(line) - 1, fp) != 0) {
      if (!strncmp(line, "DESKTOP ", 8)) {
	int l, t, r, b;
	
	if (sscanf(line+8, "%d %d %d %d", &l, &t, &r, &b) == 4) {
	  if (!((r == 0) && (t == 0) && (r == 0) && (b == 0)) &&
	      !((r < 0) || (t < 0) || (r < 0) || (b < 0)) &&
	      !((r <= l) || (b <= t))) {
	    int cx = (r + l) /2;
	    int cy = (t + b) / 2;
	    
	    move(x() + cx - (x() + width() / 2), 
		 y() + cy - (y() + height() / 2));
	  }
	}
	  
	break;
      }
    }
    
    fclose(fp);
  }
}

void ConstrDestrCopyDialog::accept() {
  if (add_constr->isChecked())
    target->addContructor(constr_explicit->isChecked());
    
  if ((add_destr != 0) && add_destr->isChecked())
    target->addDestructor(virtual_destr->isChecked());

  if ((add_copy != 0) && add_copy->isChecked())
    target->addCopy(FALSE);

  if ((add_const_copy != 0) && add_const_copy->isChecked())
    target->addCopy(TRUE);

    if ((add_assign != 0) && add_assign->isChecked())
    target->addAssign(FALSE);

  if ((add_const_assign != 0) && add_const_assign->isChecked())
    target->addAssign(TRUE);
    
  QDialog::accept();
}

