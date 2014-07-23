
#include "UmlSequenceDiagram.h"
#include "FileOut.h"

#include "UmlSequenceDiagramDefinition.h"
void UmlSequenceDiagram::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent();
  out << "<" << k << " xmi:type=\"uml:Collaboration\"";
  out.id_prefix(this, "COLLABORATION_");
  out << " name=\"";
  out.quote((const char*)name());//[jasa] ambiguous call
  out << "\">\n";
  out.indent(+1);
  write_description_properties(out);

  out.indent();
  out << "<ownedBehavior xmi:type=\"uml:Interaction\"";
  out.id_prefix(this, "INTERACTION_");
  out << " name=\"";
  out.quote((const char*)name());//[jasa] ambiguous call
  out << "\">\n";
  out.indent(+1);
  
  UmlSequenceDiagramDefinition * def = definition();
  
  def->write(out, this);

  UmlSequenceMessage::write_them(out, this, def->messages());
  
  out.indent(-1);
  out.indent();
  out << "</ownedBehavior>\n";
  
  UmlClassInstanceReference::write_attributes(out, this, definition()->instances());
  
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";

  // note : events generated at project level
}

