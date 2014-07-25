#ifndef UMLDIAGRAM_H
#define UMLDIAGRAM_H

#include "UmlBaseDiagram.h"
//Added by qt3to4:
#include <Q3CString>

// This class globaly manages any 'diagram', each specialized diagram have
// its own specialized class.
// You can modify it as you want (except the constructor)

class UmlDiagram : public UmlBaseDiagram {
  public:
    UmlDiagram(void * id, const Q3CString & n) 
      : UmlBaseDiagram(id, n) {};
};

#endif
