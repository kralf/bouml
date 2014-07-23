#ifndef _UMLREGION_H
#define _UMLREGION_H


#include "UmlBaseRegion.h"
#include "UmlStateItem.h"
#include <q3cstring.h>

class UmlClass;
class UmlState;

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const Q3CString & s) : UmlBaseRegion(id, s) {
    }

    //  initialization before generation
    
    virtual void init(UmlClass * mother, Q3CString path, Q3CString pretty_path, UmlState * state);

};

#endif
