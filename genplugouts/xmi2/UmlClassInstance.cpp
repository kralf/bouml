
#include "UmlClassInstance.h"
#include "FileOut.h"

#include "UmlClass.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
//Added by qt3to4:
#include <Q3ValueList>
void UmlClassInstance::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";

  out.indent(); 
  out << "<" << k << " xmi:type=\"uml:InstanceSpecification\"";
  out.id(this); 
  out << " name=\"";
  out.quote(name());
  if (_gen_eclipse) { 
    out << '"';
    out.ref(type(), "classifier");
    out << ">\n"; 
  }
  else
    out << "\">\n"; 
  out.indent(+1); 
  
  if (! _gen_eclipse) {
    out.indent();
    out << "<classifier ";
    out.idref(type());
    out << "/>\n"; 
  }
    
  Q3ValueList<SlotAttribute> attrs;
  Q3ValueList<SlotAttribute>::Iterator a_iter;
  unsigned rank;
  
  attributesValue(attrs);
  for (a_iter = attrs.begin(), rank = 0;
       a_iter != attrs.end();
       ++a_iter, rank += 1) {
    SlotAttribute & slot = *a_iter;
    
    out.indent();
    out << "<slot";
    out.ref(slot.attribute, "definingFeature");
    out.id_prefix(this, "ASLOT", rank);
    out << " xmi:type=\"uml:Slot\">\n";

    out.indent();
    out << "\t<value xmi:type=\"uml:LiteralString\"";
    out.id_prefix(this, "ASLOT_VALUE", rank);
    out << " value=\"";
    out.quote(slot.value);
    out << "\"/>\n";
    
    out.indent();
    out << "</slot>\n";
  }

  Q3ValueList<SlotRelation> rels;
  Q3ValueList<SlotRelation>::Iterator r_iter;
  
  relationsValue(rels);
  for (r_iter = rels.begin(), rank = 0;
       r_iter != rels.end();
       ++r_iter, rank += 1) {
    SlotRelation & slot = *r_iter;
    
    out.indent();
    out << "<slot";
    out.ref(slot.relation, "definingFeature");
    out.id_prefix(this, "RSLOT", rank);
    out << " xmi:type=\"uml:Slot\"";
    out.ref(slot.value, "value");
    out << "/>\n";
  }

  write_description_properties(out);

  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n"; 

  unload();
}

