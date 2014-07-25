
#include "UmlPseudoState.h"
#include "FileOut.h"

void UmlPseudoState::memo_incoming_trans() {
  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size();
  unsigned i;
     
  for (i = 0; i != n; i += 1) {
    UmlStateItem * x = dynamic_cast<UmlStateItem *>(ch[i]);

    if (x != 0)
      x->memo_incoming_trans();
  }
}

void UmlPseudoState::add_incoming_trans(UmlTransition * tr) {
  _incoming_trans.append(tr);
}

void UmlPseudoState::write(FileOut & out) {
  out.indent();
  out << "<subvertex xmi:type=\"uml:Pseudostate\"";
  out.id(this);
  if (! name().isEmpty()) {
    out << " name=\"";
    out.quote((const char*)name());//[jasa] ambiguous call
    out << '"';
  }
  out << " kind=\"" << sKind() << "\">\n";

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
  out << "</subvertex>\n";
    
  unload(); 
}

