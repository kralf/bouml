
#include "UmlComponent.h"
#include "FileOut.h"

void UmlComponent::write(FileOut & out) {
  const char * k = (parent()->kind() == anUseCase)
    ? "ownedUseCase"
    : ((_uml_20) ? "ownedMember" : "packagedElement");
  
  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:Component\"";
  out.id(this); 
  out << " name=\"";
  out.quote((const char*)name());//[jasa] ambiguous call
  out << "\">\n";
  out.indent(+1); 
  write_description_properties(out); 
     
  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
  unsigned index;
     
  for (index = 0; index != n; index += 1) 
    ch[index]->write(out); 
  
  // provided
  
  const Q3PtrVector< UmlClass > & prov = providedClasses();
  
  n = prov.size();
  
  for (index = 0; index != n; index += 1) {
    UmlClass * cl = prov[index];
    
    out.indent();
    out << "<interfaceRealization xmi:type=\"uml:InterfaceRealization\"";
    out.id_prefix(this, "PROV_", index);
    out.ref(cl, "supplier");
    out.ref(this, "client");
    out.ref(cl, "contract");
    out << "/>\n";
  }
  
  // realizing
  
  const Q3PtrVector< UmlClass > & rea = realizingClasses();
  
  n = rea.size();
  
  for (index = 0; index != n; index += 1) {
    UmlClass * cl = rea[index];
    
    out.indent();
    out << "<realization xmi:type=\"uml:ComponentRealization\"";
    out.id_prefix(this, "REA_", index);
    out.ref(cl, "supplier");
    out.ref(this, "client");
    out.ref(cl, "realizingClassifier");
    out << "/>\n";
  }
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 
  
  // required
  
  const Q3PtrVector< UmlClass > & req = requiredClasses();
  
  n = req.size();
  
  for (index = 0; index != n; index += 1) {
    UmlClass * cl = req[index];
    
    out.indent();
    out << "<" << k << " xmi:type=\"uml:Usage\"";
    out.id_prefix(this, "REQ_", index);
    out.ref(cl, "supplier");
    out.ref(this, "client");
    out << "/>\n";
  }

  unload(); 

}

