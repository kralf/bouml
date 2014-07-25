#ifndef UMLUSECASEDIAGRAM_H
#define UMLUSECASEDIAGRAM_H

#include "UmlBaseUseCaseDiagram.h"
//Added by qt3to4:
#include <Q3CString>

// This class manages 'use case diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
// You can modify it as you want (except the constructor)

class UmlUseCaseDiagram : public UmlBaseUseCaseDiagram {
  public:
    UmlUseCaseDiagram(void * id, const Q3CString & n)
      : UmlBaseUseCaseDiagram(id, n) {};
  
};

#endif
