#ifndef _UMLUSECASEVIEW_H
#define _UMLUSECASEVIEW_H


#include "UmlBaseUseCaseView.h"
#include <q3cstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'use case view'
//
// You can modify it as you want (except the constructor)
class UmlUseCaseView : public UmlBaseUseCaseView {
  public:
    UmlUseCaseView(void * id, const Q3CString & n) : UmlBaseUseCaseView(id, n) {};

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

};

#endif
