#ifndef _UMLACTIVITYPARTITION_H
#define _UMLACTIVITYPARTITION_H


#include "anItemKind.h"
#include <q3cstring.h>
#include "UmlBaseActivityPartition.h"
#include "UmlActivityItem.h"

class UmlItem;
class Token;
class FileIn;

class UmlActivityPartition : public UmlBaseActivityPartition, public UmlActivityItem {
  public:
    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //  call at end of import : try to solve represented element
    virtual void solve(Q3CString idref);

    static void init();

    //import the activity region 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPartition(void * id, const Q3CString & s) : UmlBaseActivityPartition(id, s) {
    }

};

#endif
