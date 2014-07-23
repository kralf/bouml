#ifndef _DIALOG_H
#define _DIALOG_H


#include <q3cstring.h>

#include <qdialog.h>
class QLineEdit;
class QCheckBox;
//A dialog to set the check-in/out command and set the recursive flag

class Dialog : public QDialog {
  Q_OBJECT

  public:
    Dialog(bool ci, Q3CString & cmd, BooL & rec, BooL & reload);


  protected:
    Q3CString & _cmd;

    BooL & _rec;

    BooL & _reload;

    QCheckBox * rec_cb;

    QLineEdit * cmd_ed;

    QCheckBox * reload_cb;

protected slots:
    virtual void polish();


  public:
    virtual void accept();

};

#endif
