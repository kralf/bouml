
#include "UmlNode.h"
#include "FileOut.h"

void UmlNode::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent(); 
  out << "<" << k << " xmi:type=\"" 
    << ((stereotype() == "device\"") ? "uml:Device" : "uml:Node\"");
  out.id(this); 
  out << " name=\"";
  out.quote((const char*)name());//[jasa] ambiguous call
  out << "\">\n"; 
  out.indent(+1); 
  
  write_description_properties(out); 
 
  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1) 
    ch[i]->write(out); 
 
  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n"; 

  unload(); 
}

