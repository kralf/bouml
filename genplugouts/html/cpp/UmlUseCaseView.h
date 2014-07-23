#ifndef _UMLUSECASEVIEW_H
#define _UMLUSECASEVIEW_H


#include "UmlBaseUseCaseView.h"
#include <q3cstring.h>

// This class manages 'use case view'
//
// You can modify it as you want (except the constructor)
class UmlUseCaseView : public UmlBaseUseCaseView {
  public:
    UmlUseCaseView(void * id, const Q3CString & n) : UmlBaseUseCaseView(id, n) {};

    //returns a string indicating the king of the element
    virtual Q3CString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(Q3CString pfix, unsigned int rank, unsigned int level);

    virtual bool chapterp();

};

#endif
