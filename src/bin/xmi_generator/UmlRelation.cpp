
#include "UmlRelation.h"
#include "FileOut.h"

#include "UmlClass.h"
//Added by qt3to4:
#include <Q3CString>
bool UmlRelation::write_if_needed(FileOut & out) {
  // class already generated
  switch (relationKind()) {
  case aGeneralisation:
  case aRealization:
    write_generalization(out);
    break;
  case aDependency:
    write_dependency(out);
    break;
  default:
    // don't generate them for actors
    {
      UmlItem * p = parent();
      
      do {
	p = p->parent();
      } while (p->kind() == aClass);
      if (p->kind() == aClassView)
	write_relation(out);
    }
    break;
  }

  return TRUE;
}

void UmlRelation::write_generalization(FileOut & out) {
  out.indent();
  out << "<UML:Generalization";
  out.id(this);
  out << " isSpecification=\"false\">\n";
  out.indent();
  out << "\t<UML:Generalization.child>";
  parent()->ref(out);
  out << "</UML:Generalization.child>\n";
  out.indent();
  out << "\t<UML:Generalization.parent>";
  roleType()->ref(out);
  out << "</UML:Generalization.parent>\n";
  out.indent(+1); 
  write_stereotype(out);  
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</UML:Generalization>\n";
}

void UmlRelation::write_dependency(FileOut & out) {
  out.indent();
  out << "<UML:Dependency";
  out.id(this);
  if (!name().isEmpty() && (((const char *) name())[0] != '<'))
    out << " name = \"" << name() << "\"";
  write_visibility(out);
  out << " isSpecification=\"false\" client=\"";
  out.ref(parent());
  out << "\" supplier=\"";
  out.ref(roleType());
  out << "\">\n";
  out.indent(+1); 
  write_stereotype(out);  
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</UML:Dependency>\n";
}

void UmlRelation::write_relation(FileOut & out) {
  if (side(TRUE) != this)
    return;
    
  out.indent();
  out << "<UML:Association";
  if (!name().isEmpty() &&
      (((const char *) name())[0] != '<') &&
      (name().right(2) != ">)"))
    out << " name=\"" << name() << '"';
  out.id(this);
  out << ">\n";
  
  out.indent();
  out << "\t<UML:Association.connection>\n";

  write_relation(out, anAssociation);
   
  UmlRelation * other = side(FALSE);
			     
  if (other != 0)
    other->write_relation(out, relationKind());
  else {
    out.indent();
    out << "\t\t<UML:AssociationEnd";
    write_visibility(out);
    out << " aggregation=\"";
    
    switch (relationKind()) {
    case anAggregation:
    case aDirectionalAggregation:
      out << "aggregate";
      break;
    case anAggregationByValue:
    case aDirectionalAggregationByValue:
      out << "composite";
      break;
    default:
      out << "none";
    }
    
    out << "\" isOrdered=\"false\" isNavigable=\"false\" type=\"";
    out.ref(parent());
    out << "\">\n";
    out.indent();
    out << "\t\t\t<UML:AssociationEnd.participant>\n";
    out.indent();
    out << "\t\t\t\t";
    parent()->ref(out);
    out << '\n';
    out.indent();
    out << "\t\t\t</UML:AssociationEnd.participant>\n";
    out.indent();
    out << "\t\t</UML:AssociationEnd>\n";
  }
  
  out.indent();
  out << "\t</UML:Association.connection>\n";
  
  out.indent();
  out << "</UML:Association>\n";
}

void UmlRelation::write_relation(FileOut & out, aRelationKind k) {
  out.indent();
  out << "\t\t<UML:AssociationEnd";
  write_visibility(out);
  write_scope(out);   
  out << " name=\"" << roleName() << "\" aggregation=\"";

  switch (k) {
  case anAggregation:
  case aDirectionalAggregation:
    out << "aggregate";
    break;
  case anAggregationByValue:
  case aDirectionalAggregationByValue:
    out << "composite";
    break;
  default:
    out << "none";
  }

  out << "\" isOrdered=\"false\" isNavigable=\"true\" type=\"";
  out.ref(roleType());
  out << "\">\n";

  if (!multiplicity().isEmpty()) {
    Q3CString min;
    Q3CString max;
    int index = multiplicity().find("..");

    if (index != -1) {
      min = multiplicity().left(index).stripWhiteSpace();
      max = multiplicity().mid(index+2).stripWhiteSpace();
    }
    else
      min = max = multiplicity().stripWhiteSpace();

    if (min == "*")
      min = "0";
    if (max == "*")
      max = "-1";

    out.indent();
    out << "\t\t\t<UML:AssociationEnd.multiplicity>\n";
    out.indent();
    out << "\t\t\t\t<UML:Multiplicity>\n";
    out.indent();
    out << "\t\t\t\t\t<UML:Multiplicity.range>\n";
    out.indent();
    out << "\t\t\t\t\t\t<UML:MultiplicityRange lower=\""
        << min << "\" upper=\"" << max << "\"/>\n";
    out.indent();
    out << "\t\t\t\t\t</UML:Multiplicity.range>\n";
    out.indent();
    out << "\t\t\t\t</UML:Multiplicity>\n";
    out.indent();
    out << "\t\t\t</UML:AssociationEnd.multiplicity>\n";
  }

  out.indent();
  out << "\t\t\t<UML:AssociationEnd.participant>\n";
  out.indent();
  out << "\t\t\t\t";
  roleType()->ref(out);
  out << '\n';
  out.indent();
  out << "\t\t\t</UML:AssociationEnd.participant>\n";
  out.indent(+3); 
  write_stereotype(out);  
  write_annotation(out);
  write_description_properties(out);
  out.indent(-3);
  out.indent();
  out << "\t\t</UML:AssociationEnd>\n";
}

