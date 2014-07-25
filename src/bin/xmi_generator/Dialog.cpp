
#include "Dialog.h"

#include <stdlib.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <q3filedialog.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <q3hbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <q3buttongroup.h>
//Added by qt3to4:
#include <Q3CString>
#include <Q3VBoxLayout>

#include "SmallPushButton.h"

Dialog::Dialog(Q3CString & path, Q3CString & encoding, Q3CString & genview, int & taggedvalue, Language & lang)
  : QDialog(0, 0, TRUE), _path(path), _encoding(encoding), _genview(genview), _taggedvalue(taggedvalue), _lang(lang) {
  Q3VBoxLayout * vbox = new Q3VBoxLayout(this);
  Q3HBox * htab;
  
  vbox->setMargin(5);
  
  // get xmi pathname
  
  htab = new Q3HBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel("generated \nXMI file : ", htab);
  
  ed = new QLineEdit(htab);
  ed->setText(_path);
  
  new QLabel(" ", htab);
  
  SmallPushButton * br = new SmallPushButton("browse", htab);
  
  connect(br, SIGNAL(clicked ()), this, SLOT(browse()));

  // to choose encoding
  
  htab = new Q3HBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel("Encoding : ", htab);
  
  QString charset = getenv("BOUML_CHARSET");
  int index = 0;
  
  while ((index = charset.find('_')) != -1) {
    charset[index] = '-';
    index += 1;
  }

  encoding_cb = new QComboBox(TRUE, htab);
  encoding_cb->insertItem(QString(_encoding));
  if (!charset.isEmpty() && (_encoding != (const char *) charset))
    encoding_cb->insertItem(charset);
#ifdef WIN32
  if ((_encoding != "windows-1252") && (charset != "windows-1252"))
    encoding_cb->insertItem("windows-1252");
  if ((_encoding != "ISO-8859-1") && (charset != "ISO-8859-1"))
    encoding_cb->insertItem("ISO-8859-1");
#else
  if ((_encoding != "ISO-8859-1") && (charset != "ISO-8859-1"))
    encoding_cb->insertItem("ISO-8859-1");
  if ((_encoding != "windows-1252") && (charset != "windows-1252"))
    encoding_cb->insertItem("windows-1252");
#endif
  
  // generate view checkbox
  
  htab = new Q3HBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  gen_cb = new QCheckBox("Generate views as package", htab);
  if (_genview == "yes")
    gen_cb->setChecked(TRUE);
    
  // tagged value generation mode
    
  htab = new Q3HBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  Q3ButtonGroup * bg =
    new Q3ButtonGroup(3, Qt::Vertical, "Tagged values generation", htab);
  
  tg_0 = new QRadioButton("Not generated", bg);
  tg_1 = new QRadioButton("<UML:TaggedValue tag=\"..\" value=\"..\"/>", bg);
  tg_2 = new QRadioButton("<UML:TaggedValue>\n"
			  "    <Uml:TaggedValue.tag>..</Uml:TaggedValue.tag>\n"
			  "    <Uml:TaggedValue.value>..</Uml:TaggedValue.value>\n"
			  "</UML:TaggedValue>",
			  bg);
  switch (taggedvalue) {
  case 1: tg_1->setChecked(TRUE); break;
  case 2: tg_2->setChecked(TRUE); break;
  default: tg_0->setChecked(TRUE); break;
  }
  
  // uml , c++, java, cancel buttons
  
  htab = new Q3HBox(this);
  htab->setMargin(5);
  vbox->addWidget(htab);
  
  new QLabel(htab);
  QPushButton * uml = new QPushButton("&Uml", htab);
  new QLabel(htab);
  QPushButton * cpp = new QPushButton("&C++", htab);
  new QLabel(htab);
  QPushButton * java = new QPushButton("&Java", htab);
  new QLabel(htab);
  QPushButton * cancel = new QPushButton("&Cancel", htab);
  new QLabel(htab);
  QSize bs(cancel->sizeHint());
  
  uml->setFixedSize(bs);
  cpp->setFixedSize(bs);
  java->setFixedSize(bs);
  
  connect(uml, SIGNAL(clicked()), this, SLOT(accept_uml()));
  connect(cpp, SIGNAL(clicked()), this, SLOT(accept_cpp()));
  connect(java, SIGNAL(clicked()), this, SLOT(accept_java()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void Dialog::browse() {
  QString s = Q3FileDialog::getSaveFileName(_path, "*.xmi", 0);
  
  if (! s.isEmpty()) {
    if (s.right(4).lower() != ".xmi")
      ed->setText(s + ".xmi");
    else
      ed->setText(s);
  }
}

void Dialog::accept_cpp() {
  _lang = Cpp;
  
  accept();
}

void Dialog::accept_uml() {
  _lang = Uml;
  accept();
}

void Dialog::accept_java() {
  _lang = Java;
  accept();
}

void Dialog::accept() {
  _path = ed->text();
  _encoding = encoding_cb->currentText();
  _genview = (gen_cb->isChecked()) ? "yes" : "no";
  if (tg_0->isChecked())
    _taggedvalue = 0;
  else if (tg_1->isChecked())
    _taggedvalue = 1;
  else if (tg_2->isChecked())
    _taggedvalue = 2;
  
  QDialog::accept();
}

