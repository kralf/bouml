#ifndef _UMLBASECOMPONENT_H
#define _UMLBASECOMPONENT_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <q3ptrvector.h>
#include <q3cstring.h>

#include "UmlClass.h"	// to avoid destructor problem
class UmlComponent;
class UmlComponentView;
class UmlComponentDiagram;
class UmlClass;

//  Manage the components.
class UmlBaseComponent : public UmlItem {
  public:
    // returns a new component named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlComponent * create(UmlComponentView * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated diagram
    UmlComponentDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlComponentDiagram * d);

    // returns (in Java a copy of) the optional realizing classes
    const Q3PtrVector<UmlClass> & realizingClasses();

    // returns (in Java a copy of) the optional provided classes
    const Q3PtrVector<UmlClass> & providedClasses();

    // returns (in Java a copy of) the optional required classes
    const Q3PtrVector<UmlClass> & requiredClasses();


  private:
    UmlComponentDiagram * _assoc_diagram;

    Q3PtrVector<UmlClass> _realizing;

    Q3PtrVector<UmlClass> _provided;

    Q3PtrVector<UmlClass> _required;


  protected:
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseComponent(void * id, const Q3CString & n);

};

inline UmlBaseComponent::UmlBaseComponent(void * id, const Q3CString & n) : UmlItem(id, n) {
  _assoc_diagram = 0;
}

#endif
