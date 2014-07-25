
#include "UmlAttribute.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlRelation.h"
#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>
void UmlAttribute::init()
{
  declareFct("ownedattribute", "uml:Property", &importIt);
  declareFct("ownedattribute", "", &importIt);	// Eclipse .uml
  declareFct("ownedliteral", "uml:EnumerationLiteral", &importIt);
  declareFct("ownedliteral", "", &importIt);	// eclipse
}

void UmlAttribute::importIt(FileIn & in, Token & token, UmlItem * where)
{
  if (!token.valueOf("association").isNull())
    UmlRelation::importAsAttribute(in, token, where);
  else {
    where = where->container(anAttribute, token, in);
    
    if (where != 0) {
      Q3CString s = token.valueOf("name");
      
      if (s.isEmpty()) {
	static unsigned n = 0;
	
	s.sprintf("anonymous_attribute_%u", ++n);
      }
      else
        s = legalName(s);
      
      UmlAttribute * att = create((UmlClass *) where, s);
      
      if (att == 0)
	in.error("cannot create attribute '" + s +
		 "' in '" + where->name() + "'");
      
      att->addItem(token.xmiId(), in);
      
      if (token.what() == "ownedliteral")
	att->set_Visibility(PublicVisibility);
      else
	att->setVisibility(token.valueOf("visibility"));
      
      if (token.valueOf("isreadonly") == "true")
	 att->set_isReadOnly(TRUE);
      
      if (token.valueOf("isderived") == "true")
	 att->set_isDerived(TRUE, (token.valueOf("isderivedunion") == "true"));
      
      if (token.valueOf("isordered") == "true")
	 att->set_isOrdered(TRUE);
      
      if (token.valueOf("isunique") == "true")
	 att->set_isUnique(TRUE);
      
      if (!(s = token.valueOf("type")).isEmpty()) {
	UmlTypeSpec ts;
	
	if (att->setType(s, ts))
	  att->set_Type(ts);
      }
      
      if (!(s = token.valueOf("defaultvalue")).isEmpty())
	att->set_DefaultValue(s);
      
      if (token.valueOf("isstatic") == "true")
	att->set_isClassMember(TRUE);
	  
      if (! token.closed()) {
	Q3CString k = token.what();
	const char * kstr = k;
	
	while (in.read(), !token.close(kstr)) {
	  s = token.what();
	  
	  if (s == "type") {
	    UmlTypeSpec ts;
	    
	    if (att->setType(token, ts))
	      att->set_Type(ts);
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if (s == "defaultvalue") {
	    att->set_DefaultValue(token.valueOf("value"));
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if (s == "lowervalue")
	    att->importMultiplicity(in, token, FALSE);
	  else if (s == "uppervalue")
	    att->importMultiplicity(in, token, TRUE);
	  else if (s == "upperbound") {
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if ((s == "specification") && (k == "ownedliteral")) {
	    if (! token.closed())
	      in.finish(s);
	  }
	  else if (s == "ownedrule")
	    att->set_Constraint(UmlItem::readConstraint(in, token));
	  else
	    att->UmlItem::import(in, token);
	}
      }
    }
  }
}

void UmlAttribute::solve(Q3CString idref) {
  UmlTypeSpec ts;
  
  if (getType(idref, ts))
    set_Type(ts);
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("attribute : unknown type reference '" + idref + "'<br>");
}

void UmlAttribute::importMultiplicity(FileIn & in, Token & token, bool upper) {
  Q3CString s = token.valueOf("value");
  
  if (!s.isEmpty() && 
      (s != "Unspecified")) {	// VP
    Q3CString m = multiplicity();
    
    if (m.isEmpty())
      m = s;
    else if (m != s) {
      if (upper)
	m += ".." + s;
      else
	m = s + ".." + m;
    }
    
    set_Multiplicity(m);
  }

  if (! token.closed())
    in.finish(token.what());
}

