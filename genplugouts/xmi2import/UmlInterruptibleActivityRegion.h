#ifndef _UMLINTERRUPTIBLEACTIVITYREGION_H
#define _UMLINTERRUPTIBLEACTIVITYREGION_H


#include "UmlBaseInterruptibleActivityRegion.h"
#include <q3cstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlInterruptibleActivityRegion : public UmlBaseInterruptibleActivityRegion {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlInterruptibleActivityRegion(void * id, const Q3CString & s) : UmlBaseInterruptibleActivityRegion(id, s) {
    }

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    static void init();

    //import the activity region 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
