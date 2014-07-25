#ifndef _UMLBASEUSECASE_H
#define _UMLBASEUSECASE_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <q3cstring.h>

class UmlUseCase;
class UmlUseCaseDiagram;

// Manage the use cases
class UmlBaseUseCase : public UmlItem {
  public:
    // returns a new use case named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUseCase * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated diagram
    UmlUseCaseDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlUseCaseDiagram * d);

    // return the extension points
    const Q3CString & extensionPoints();

    // set the extension points
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_ExtensionPoints(const char * v);


  private:
    Q3CString _extension_points;

    UmlUseCaseDiagram * _assoc_diagram;


  protected:
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseUseCase(void * id, const Q3CString & n) : UmlItem(id, n) {};

};

#endif
