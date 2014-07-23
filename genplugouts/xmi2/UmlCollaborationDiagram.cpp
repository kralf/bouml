
#include "UmlCollaborationDiagram.h"
#include "FileOut.h"

#include "UmlCollaborationDiagramDefinition.h"
void UmlCollaborationDiagram::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent();
  out << "<" << k << " xmi:type=\"uml:Collaboration\"";
  out.id_prefix(this, "COLLABORATION_");
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1);
  write_description_properties(out);

  out.indent();
  out << "<ownedBehavior xmi:type=\"uml:Interaction\"";
  out.id_prefix(this, "INTERACTION_");
  out << " name=\"";
  out.quote(name());
  out << "\">\n";
  out.indent(+1);
  
  definition()->write(out, this);

  out.indent(-1);
  out.indent();
  out << "</ownedBehavior>\n";
  
  UmlClassInstanceReference::write_attributes(out, this, definition()->instances());
  
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";

  // note : events generated at project level
}

