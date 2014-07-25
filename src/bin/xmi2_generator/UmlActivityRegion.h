#ifndef _UMLACTIVITYREGION_H
#define _UMLACTIVITYREGION_H


#include "UmlBaseActivityRegion.h"
#include "UmlActivityItem.h"
#include "FlowContainer.h"
#include <q3cstring.h>

class UmlActivityRegion : public UmlBaseActivityRegion, public UmlActivityItem, public FlowContainer {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityRegion(void * id, const Q3CString & s) : UmlBaseActivityRegion(id, s) {
    }


  public:
    //goes among tree to memorize incoming flow
    
    virtual void memo_incoming_flow();

};

#endif
