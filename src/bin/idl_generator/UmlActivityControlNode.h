#ifndef _UMLACTIVITYCONTROLNODE_H
#define _UMLACTIVITYCONTROLNODE_H


#include "UmlBaseActivityControlNode.h"
#include <q3cstring.h>

class UmlActivityControlNode : public UmlBaseActivityControlNode {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityControlNode(void * id, const Q3CString & s) : UmlBaseActivityControlNode(id, s) {
    }

};

#endif
