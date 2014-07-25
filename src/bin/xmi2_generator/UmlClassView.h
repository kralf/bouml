#ifndef _UMLCLASSVIEW_H
#define _UMLCLASSVIEW_H


#include "UmlBaseClassView.h"
#include <q3cstring.h>
#include <q3ptrlist.h>

class FileOut;
class UmlItem;

// This class manages 'class view'
//
// You can modify it as you want (except the constructor)
class UmlClassView : public UmlBaseClassView {
  public:
    UmlClassView(void * id, const Q3CString & n) : UmlBaseClassView(id, n) {};

    virtual void write(FileOut & out);

    virtual void search_class_assoc();

    //Memorize the relation in the package (or the view in case the views
    //are generated as package) containing the current element, to generate
    //the relation at the package/view level
    
    virtual void memo_relation(UmlItem * r);


  protected:
    Q3PtrList<UmlItem> _relations;

};

#endif
