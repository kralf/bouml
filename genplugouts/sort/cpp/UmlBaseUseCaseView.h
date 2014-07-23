#ifndef _UMLBASEUSECASEVIEW_H
#define _UMLBASEUSECASEVIEW_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <q3cstring.h>

class UmlUseCaseView;

//  Manage the use case views
class UmlBaseUseCaseView : public UmlItem {
  public:
    // returns a new use case view named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUseCaseView * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseUseCaseView(void * id, const Q3CString & n) : UmlItem(id, n) {};

};

#endif
