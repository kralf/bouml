
#include "UmlExpansionRegion.h"
#include "FileOut.h"

#include "UmlFlow.h"
void UmlExpansionRegion::write(FileOut & out) {
  out.indent();
  out << "<group xmi:type=\"uml:ExpansionRegion\" name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  if (isMustIsolate())
    out << " mustIsolate=\"true\"";
  out << " mode=\"";
  switch (mode()) {
  case parallelExecution:
    out << "parallel";
    break;
  case iterativeExecution:
    out << "iterative";
    break;
  default:
    // streamExecution
    out << "stream";
    break;
  }
  out << "\">\n";
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

