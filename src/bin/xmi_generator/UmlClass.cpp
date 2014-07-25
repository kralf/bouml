
#include "UmlClass.h"
#include "FileOut.h"

void UmlClass::ref(FileOut & out) {
  if ((stereotype() == "actor") ||
      ((parent()->kind() != aClassView) &&
       (parent()->kind() != aClass)))
    out << "<UML:Actor";
  else if (stereotype() == "interface")
    out << "<UML:Interface";
  else
    out << "<UML:Class";

  out.idref(this);
  out << "/>";
}

void UmlClass::write(FileOut & out) {
  if (! _written) {
    _written = TRUE;

    // parent already written
    
    out.indent();
    out << "<UML:Classifier.feature>\n";
    out.indent(+1);
  }
}

bool UmlClass::write_if_needed(FileOut & out) {
  // even if the class doen't have children
  parent()->write(out);
  
  if ((stereotype() == "actor") ||
      ((parent()->kind() != aClassView) &&
       (parent()->kind() != aClass)))
    // force it to be an actor
    write_actor(out);
  else {
    switch (_lang) {
    case Cpp:
      if (cppDecl().isEmpty())
	return FALSE;
      break;
    case Java:
      if (javaDecl().isEmpty())
	return FALSE;
    default:
      break;
    }
    
    bool interf = (stereotype() == "interface");

    out.indent();
    out << ((interf) ? "<UML:Interface name=\"" : "<UML:Class name=\"");
    out.quote(name());
    out << '"';
    out.id(this);
    out << " visibility=\"public\" isAbstract=\""
        << ((isAbstract()) ? "true" : "false")
        << "\" isActive=\"false\" >\n";
    out.indent(+1);
    if (! interf)
      write_stereotype(out);
    write_annotation(out);
    write_description_properties(out);
    
    const Q3PtrVector<UmlItem> ch = children();
    unsigned n = ch.size();
    bool used = FALSE;
    bool haveRel= FALSE;
    
    for (unsigned i = 0; i != n; i += 1) {
      switch (ch[i]->kind()) {
      case aNcRelation:
	break;
      case aRelation:
	haveRel = TRUE;
	break;
      default:
        used |= ch[i]->write_if_needed(out);
      }
    }
      
    if (used) {
      out.indent(-1);
      out.indent();
      out << "</UML:Classifier.feature>\n";
    }

  
    out.indent(-1);
    out.indent();
    out << ((interf) ? "</UML:Interface>\n" : "</UML:Class>\n");

    if (haveRel) {
      for (unsigned i = 0; i != n; i += 1) 
        if (ch[i]->kind() == aRelation)
          used |= ch[i]->write_if_needed(out);
    }
  }

  unload();
 
  return TRUE;
}

void UmlClass::write_actor(FileOut & out) {
  out.indent(); 
  out << "<UML:Actor name=\"" << name() << '"'; 
  out.id(this); 
  out << " visibility=\"public\" isAbstract=\"" 
      << ((isAbstract()) ? "true" : "false") 
      << "\" isActive=\"false\" >\n"; 
  out.indent(+1); 
  if (stereotype() != "actor") 
    write_stereotype(out); 
  write_description_properties(out);
  out.indent(-1); 
  out.indent();     
  out << "</UML:Actor>\n"; 

  const Q3PtrVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1) 
    if (ch[i]->kind() == aRelation) 
      ch[i]->write_if_needed(out);
}

