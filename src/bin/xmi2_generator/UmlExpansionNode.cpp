
#include "UmlExpansionNode.h"
#include "FileOut.h"

void UmlExpansionNode::write(FileOut & out) {
  const char * k = (parent()->kind() == anActivity)
    ? "node" : "containedNode";
    
  out.indent();
  out << '<' << k << " xmi:type=\"uml:ExpansionNode\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  if (isControlType())
    out << " isControlType=\"true\"";
  write_ordering(out);
  write_selection(out);
  write_in_state(out);
  out << ">\n";
  
  out.indent(+1);
  
  write_description_properties(out);
  write_multiplicity(out, multiplicity(), this);
  UmlItem::write_type(out, type());

  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
  
  write_incoming_flows(out);
  
  out.indent(-1);

  out.indent();
  out << "</" << k << ">\n";

  unload();
}

