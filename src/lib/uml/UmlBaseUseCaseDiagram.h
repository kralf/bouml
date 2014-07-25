#ifndef _UMLBASEUSECASEDIAGRAM_H
#define _UMLBASEUSECASEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <q3cstring.h>

class UmlUseCaseDiagram;
class UmlItem;

//  Manage the use case diagrams
class UmlBaseUseCaseDiagram : public UmlDiagram {
  public:
    // returns a new use case diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUseCaseDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseUseCaseDiagram(void * id, const Q3CString & n) : UmlDiagram(id, n) {};

};

#endif
