#ifndef _UMLCLASSDIAGRAM_H
#define _UMLCLASSDIAGRAM_H


#include "UmlBaseClassDiagram.h"
#include <q3cstring.h>

// This class manages 'class diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlClassDiagram : public UmlBaseClassDiagram {
  public:
    UmlClassDiagram(void * id, const Q3CString & n) : UmlBaseClassDiagram(id, n) {};

};

#endif
