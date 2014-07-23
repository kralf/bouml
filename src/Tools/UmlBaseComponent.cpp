
#include "UmlBaseComponent.h"
#include "UmlComponent.h"
#include "UmlComponentView.h"
#include "UmlComponentDiagram.h"
#include "UmlClass.h"

#include "UmlCom.h"
UmlComponent * UmlBaseComponent::create(UmlComponentView * parent, const char * s)
{
  return (UmlComponent *) parent->create_(aComponent, s);
}

anItemKind UmlBaseComponent::kind() {
  return aComponent;
}

UmlComponentDiagram * UmlBaseComponent::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseComponent::set_AssociatedDiagram(UmlComponentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

const Q3PtrVector<UmlClass> & UmlBaseComponent::realizingClasses() {
  read_if_needed_();
  
  return _realizing;
}

const Q3PtrVector<UmlClass> & UmlBaseComponent::providedClasses() {
  read_if_needed_();
  
  return _provided;
}

const Q3PtrVector<UmlClass> & UmlBaseComponent::requiredClasses() {
  read_if_needed_();
  
  return _required;
}

bool UmlBaseComponent::set_AssociatedClasses(const Q3PtrVector<UmlClass> & realizing, const Q3PtrVector<UmlClass> & provided, const Q3PtrVector<UmlClass> & required) {
  UmlCom::send_cmd(_identifier, setAssocClassesCmd,
		   realizing, provided, required);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _realizing = realizing;
      _provided = provided;
      _required = required;
    }
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseComponent::unload(bool rec, bool del) {
  _realizing.clear();
  _provided.clear();
  _required.clear();

  UmlBaseItem::unload(rec, del);

}

void UmlBaseComponent::read_uml_() {
  _assoc_diagram = (UmlComponentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  _realizing.resize(n);
    
  for (index = 0; index != n; index += 1)
    _realizing.insert(index, (UmlClass *) UmlBaseItem::read_());

  n = UmlCom::read_unsigned();
  _provided.resize(n);
    
  for (index = 0; index != n; index += 1)
    _provided.insert(index, (UmlClass *) UmlBaseItem::read_());

  n = UmlCom::read_unsigned();
  _required.resize(n);
    
  for (index = 0; index != n; index += 1)
    _required.insert(index, (UmlClass *) UmlBaseItem::read_());
}

