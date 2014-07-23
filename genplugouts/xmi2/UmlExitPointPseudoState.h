#ifndef _UMLEXITPOINTPSEUDOSTATE_H
#define _UMLEXITPOINTPSEUDOSTATE_H


#include "UmlBaseExitPointPseudoState.h"
#include <q3cstring.h>

class FileOut;

class UmlExitPointPseudoState : public UmlBaseExitPointPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlExitPointPseudoState(void * id, const Q3CString & s) : UmlBaseExitPointPseudoState(id, s) {
    }

    virtual void write(FileOut & out);

    virtual const char * sKind() const;

};

#endif
