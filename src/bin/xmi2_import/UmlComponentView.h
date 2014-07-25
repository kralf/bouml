#ifndef _UMLCOMPONENTVIEW_H
#define _UMLCOMPONENTVIEW_H


#include "UmlBaseComponentView.h"
#include <q3cstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'component view'
//
// You can modify it as you want (except the constructor)
class UmlComponentView : public UmlBaseComponentView {
  public:
    UmlComponentView(void * id, const Q3CString & n) : UmlBaseComponentView(id, n) {};

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

};

#endif
