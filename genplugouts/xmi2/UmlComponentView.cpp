
#include "UmlComponentView.h"
#include "FileOut.h"
#include "UmlItem.h"

void UmlComponentView::write(FileOut & out) {
  if (_gen_views) {
    out.indent(); 
    out << ((_uml_20) ? "<ownedMember" : "<packagedElement") 
      << " xmi:type=\"uml:Package\""; 
    out.id(this); 
    out << " name =\"component view ";
    out.quote((const char*)name());//[jasa] ambiguous call
    out << "\">\n"; 
    out.indent(+1); 
    write_description_properties(out);   
  }

  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->write(out);
    
  if (_gen_views) {
    while (! _relations.isEmpty())
      _relations.take(0)->write(out, FALSE);

    out.indent(-1); 
    out.indent(); 
    out << ((_uml_20) ? "</ownedMember>\n" : "</packagedElement>\n");
  }
}

void UmlComponentView::memo_relation(UmlItem * r) {
  if (_gen_views)
	_relations.append(r);
  else
    parent()->memo_relation(r);
}

