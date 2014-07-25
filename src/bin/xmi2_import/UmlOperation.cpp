
#include "UmlOperation.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>
void UmlOperation::init()
{
  declareFct("ownedoperation", "", &importIt);
  declareFct("ownedoperation", "uml:Operation", &importIt);
}

void UmlOperation::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anOperation, token, in);
  
  if (where == 0)
    return;
    
  Q3CString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_operation_%u", ++n);
  }
    
  UmlOperation * op = create((UmlClass *) where, s);
  
  if (op == 0)
    in.error("cannot create operation '" + s +
	     "' in '" + where->name() + "'");
  
  op->addItem(token.xmiId(), in);
  
  op->setVisibility(token.valueOf("visibility"));
  
  if (token.valueOf("isstatic") == "true")
    op->set_isClassMember(TRUE);

  if (token.valueOf("isabstract") == "true")
    op->set_isAbstract(TRUE);
    
  if (token.valueOf("isquery") == "true")
    op->set_isCppConst(TRUE);

  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
	  
      if (s == "raisedexception")
	op->addException(token, in);
      else if ((s == "ownedparameter") ||
	       (s == "returnresult")) // andromda emf
	op->addParameter(token, in);
      else if (s == "ownedrule")
	op->set_Constraint(UmlItem::readConstraint(in, token));
      else
        op->UmlItem::import(in, token);
    }
  }
}

void UmlOperation::addException(Token & token, FileIn & in) {
  Q3CString idref = token.xmiIdref();
  
  if (! idref.isEmpty()) {
    int n = exceptions().count();
    UmlTypeSpec ts;
    
    (void) setType(idref, -n -1, ts);
    UmlBaseOperation::addException(n, ts);
  }

  if (! token.closed())
    in.finish(token.what());
}

void UmlOperation::addParameter(Token & token, FileIn & in) {
  UmlParameter p;
  Q3CString s;
  
  p.name = token.valueOf("name");
    
  s = token.valueOf("direction");
  if ((s == "in") || (s == "pk_in"))
    p.dir = InputDirection;
  else if ((s == "out") || (s == "pk_out"))
    p.dir = OutputDirection;
  else if ((s == "inout") || (s == "pk_inout"))
    p.dir = InputOutputDirection;
  else if ((s == "return") || (s == "pk_return"))
    p.dir = ReturnDirection;
  else {
    p.dir = InputDirection;
    if (! s.isEmpty())
      in.warning("wrong direction");
  }
    
  if (!(s = token.valueOf("defaultvalue")).isEmpty())
    p.default_value = s;
    
  Q3CString typeref = token.valueOf("type");
  
  if (! typeref.isEmpty()) {
    if (p.dir == ReturnDirection) {
      UmlTypeSpec ts;
    
      if (setType(typeref, 0, ts))
	set_ReturnType(ts);
    }
    else
      (void) setType(typeref, params().count() + 1, p.type);
  }
    
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "type") {
	if (p.dir == ReturnDirection) {
	  UmlTypeSpec ts;
	  
	  if (setType(token, 0, ts))
	    set_ReturnType(ts);
	}
	else
	  (void) setType(token, params().count() + 1, p.type);
      }
      else if (s == "defaultvalue")
        p.default_value = token.valueOf("value");
      else if (s == "ownedcomment")
	in.bypassedId(token);
      else {
	in.bypass(token);
	continue;
      }
      
      if (! token.closed())
        in.finish(s);
    }
  }

  if (p.dir != ReturnDirection)
    UmlBaseOperation::addParameter(params().count(), p);

}

void UmlOperation::solve(int context, Q3CString idref) {
  UmlTypeSpec ts;
  
  if (getType(idref, ts)) {
    if (context == 0)
      set_ReturnType(ts);
    else if (context > 0) {
      UmlParameter p = params()[context - 1];
      
      p.type = ts;
      replaceParameter(context - 1, p);
    }
    else
      replaceException(- context + 1, ts);
  }
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("operation : unknown type reference '" + idref + "'<br>");
}

