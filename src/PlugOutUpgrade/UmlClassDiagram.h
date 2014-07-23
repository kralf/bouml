#ifndef UMLCLASSDIAGRAM_H
#define UMLCLASSDIAGRAM_H

#include "UmlBaseClassDiagram.h"
//Added by qt3to4:
#include <Q3CString>

// This class manages 'class diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
// You can modify it as you want (except the constructor)

class UmlClassDiagram : public UmlBaseClassDiagram {
  public:
    UmlClassDiagram(void * id, const Q3CString & n) 
      : UmlBaseClassDiagram(id, n) {};
};

#endif
