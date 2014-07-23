#ifndef _UMLEXITPOINTPSEUDOSTATE_H
#define _UMLEXITPOINTPSEUDOSTATE_H


#include <q3cstring.h>
#include "UmlBaseExitPointPseudoState.h"

class UmlExitPointPseudoState : public UmlBaseExitPointPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual Q3CString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlExitPointPseudoState(void * id, const Q3CString & s) : UmlBaseExitPointPseudoState(id, s) {
    }

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(Q3CString pfix, unsigned int rank, unsigned int level);

};

#endif
