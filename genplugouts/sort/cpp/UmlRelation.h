#ifndef _UMLRELATION_H
#define _UMLRELATION_H


#include "UmlBaseRelation.h"
#include <q3cstring.h>

// This class manages 'relations' between classes, notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const Q3CString & n) : UmlBaseRelation(id, n) {};

    virtual int orderWeight();

};

#endif
