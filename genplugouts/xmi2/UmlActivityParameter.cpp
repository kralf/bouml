
#include "UmlActivityParameter.h"
#include "FileOut.h"

void UmlActivityParameter::write(FileOut & out) {
  // write parameter def

  out.indent();
  out << "<ownedParameter name=\"";
  out.quote(name());
  out << '"';
  out.id(this);
  write_dir(out);
  write_effect(out);
  write_flags(out);
  out << ">\n";
  
  out.indent(+1);
  
  write_description_properties(out);
  write_multiplicity(out, multiplicity(), this);
  write_default_value(out, defaultValue(), this);
  UmlItem::write_type(out, type());
  
  out.indent(-1);

  out.indent();
  out << "</ownedParameter>\n";

  //write parameter node
  out.indent();
  out << "<node xmi:type=\"uml:ActivityParameterNode\" name =\"";
  out.quote(name());
  out << '"';
  out.id_prefix(this, "PARAMETER_NODE_");
  if (isControlType())
    out << " isControlType=\"true\"";
  write_ordering(out);
  write_selection(out);
  write_in_state(out);
  out << ">\n";
  out.indent(+1);

  UmlItem::write_type(out, type());

  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);

  out.indent(-1);
  out.indent();
  out << "</node>\n";

  unload();
}

void UmlActivityParameter::write_effect(FileOut & out) {
 switch (effect()) {
 case createEffect:
   out << " effect=\"create\"";
   break;
 case readEffect:
   out << " effect=\"read\"";
   break;
 case updateEffect:
   out << " effect=\"update\"";
   break;
 case deleteEffect:
   out << " effect=\"delete\"";
   break;
 default:
   break;
 }
}

