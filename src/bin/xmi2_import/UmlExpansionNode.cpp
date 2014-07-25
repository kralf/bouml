
#include "UmlExpansionNode.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"
//Added by qt3to4:
#include <Q3CString>

void UmlExpansionNode::init()
{
  declareFct("containednode", "uml:ExpansionNode", &importIt);
  declareFct("node", "uml:ExpansionNode", &importIt);

  UmlActivityObject::init();
}

void UmlExpansionNode::importIt(FileIn & in, Token & token, UmlItem * where)
{
  if (where->kind() != anExpansionRegion) 
    in.bypass(token);
  else {
    Q3CString s = token.valueOf("name");
    UmlExpansionNode * e =
      UmlExpansionNode::create((UmlExpansionRegion *) where, s);
    
    if (e == 0)
      in.error("cannot create expansion node '"
	       + s + "' in '" + where->name() + "'");
    
    e->addItem(token.xmiId(), in);
    
    if (token.valueOf("iscontroltype") == "true")
      e->set_IsControlType(TRUE);
    
    if (!(s = token.valueOf("ordering")).isEmpty())
      e->setOrdering(s, in);
    if (!(s = token.valueOf("selection")).isEmpty())
      e->setSelection(s);
    if (!(s = token.valueOf("instate")).isEmpty())
      e->setInState(s);
    if (!(s = token.valueOf("type")).isEmpty())
      e->setType(s);
    
    if (! token.closed()) {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "selection") {
	  e->setSelection(token.valueOf("idref"));
	  if (! token.closed())
	    in.finish(s);
	}
	else if (s == "instate") {
	  e->setInState(token.valueOf("idref"));
	  if (! token.closed())
	    in.finish(s);
	}
	else if (s == "type") {
	  e->setType(token);
	  if (! token.closed())
	    in.finish(s);
	}
	else if (s == "lowervalue")
	  e->importMultiplicity(in, token, FALSE);
	else if (s == "uppervalue")
	  e->importMultiplicity(in, token, TRUE);
	else if (s == "upperbound") {
	  if (! token.closed())
	    in.finish(s);
	}
	else
	  e->UmlItem::import(in, token);
      }
    }
  }
}

