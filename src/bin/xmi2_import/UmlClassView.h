#ifndef _UMLCLASSVIEW_H
#define _UMLCLASSVIEW_H


#include "UmlBaseClassView.h"
#include <q3cstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'class view'
//
// You can modify it as you want (except the constructor)
class UmlClassView : public UmlBaseClassView {
  public:
    UmlClassView(void * id, const Q3CString & n) : UmlBaseClassView(id, n) {};

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

};

#endif
