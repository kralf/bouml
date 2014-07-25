#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <q3cstring.h>

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const Q3CString & n) : UmlBasePackage(id, n) {};

    virtual void uml2cpp(bool rec);

    virtual void uml2java(bool rec);

    virtual void uml2idl(bool rec);

    virtual void uml2php(bool rec);

    virtual void uml2python(bool rec);

};

#endif
