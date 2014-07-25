
#include "UmlExitPointPseudoState.h"
#include "FileOut.h"

void UmlExitPointPseudoState::write(FileOut & out) {
  UmlExitPointPseudoState * ref = reference();
  
  out.indent();
  if (ref != 0)
    out << "<connection";
  else
    out << "<subvertex xmi:type=\"uml:Pseudostate\"";
  out.id(this);
  if (! name().isEmpty()) {
    out << " name=\"";
    out.quote(name());
    out << '"';
  }
  if (ref != 0) {
    out.ref(ref, "exit");
    out << ">\n";
  }
  else
    out << " kind=\"exitPoint\">\n";

  out.indent(+1);
  
  write_description_properties(out); 
     
  while (! _incoming_trans.isEmpty())
    _incoming_trans.take(0)->write_in(out);
  
  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1)
    ch[i]->write(out);

  out.indent(-1);
  out.indent();
  if (ref != 0)
    out << "</connection>\n";
  else
    out << "</subvertex>\n";
    
  unload(); 
}

const char * UmlExitPointPseudoState::sKind() const {
  return "exitPoint";
}

