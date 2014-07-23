
#include "UmlInterruptibleActivityRegion.h"
#include "FileOut.h"

#include "UmlFlow.h"
void UmlInterruptibleActivityRegion::write(FileOut & out) {
  out.indent();
  out << "<group xmi:type=\"uml:InterruptibleActivityRegion\"";
  out.id(this);
  out << ">\n";
  out.indent(+1);
  
  write_description_properties(out); 
  
  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

  write_flows(out);
  
  out.indent(-1);
  out.indent();
  out << "</group>\n";

  unload();
}

