#ifndef _UMLENTRYPOINTPSEUDOSTATE_H
#define _UMLENTRYPOINTPSEUDOSTATE_H


#include "UmlBaseEntryPointPseudoState.h"
#include <q3cstring.h>

class UmlEntryPointPseudoState : public UmlBaseEntryPointPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlEntryPointPseudoState(void * id, const Q3CString & s) : UmlBaseEntryPointPseudoState(id, s) {
    }

    //  call at end of import : try to solve referenced entry point
    
    virtual void solve(Q3CString idref);

};

#endif
