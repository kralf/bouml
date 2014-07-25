
#include "UmlDeploymentView.h"
#include "FileOut.h"

bool UmlDeploymentView::write_if_needed(FileOut & out) {
  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
  bool used = FALSE; 
   
  for (unsigned i = 0; i != n; i += 1) 
    used |= ch[i]->write_if_needed(out); 
     
  if (used && _gen_views) { 
    out.indent(-1); 
    out.indent(); 
    out << "</UML:Namespace.ownedElement>\n"; 
     
    out.indent(-1); 
    out.indent(); 
    out << "</UML:Package>\n"; 
  } 
 
  return used; 
}

void UmlDeploymentView::write(FileOut & out) {
  if (!_written) { 
    _written = TRUE; 
     
    parent()->write(out); 
     
    if (_gen_views) {
      out.indent(); 
      out << "<UML:Package"; 
      out.id(this); 
      out << " name =\"Deployment View ";
      out.quote(name());
      out << '"'; 
      out << ">\n"; 
      out.indent(+1); 
      write_stereotype(out); 
      write_description_properties(out); 
      out.indent(); 
      out << "<UML:Namespace.ownedElement>\n"; 
      out.indent(+1); 
    }
  } 
 
}

