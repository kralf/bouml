
#include "UmlUseCase.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlNcRelation.h"
#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>
UmlItem * UmlUseCase::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aClass:
  case aClassInstance:
  case anUseCase:
  case aState:
  case anActivity:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

void UmlUseCase::solve(int context, Q3CString idref) {
  QMap<Q3CString, UmlItem *>::Iterator it = All.find(idref);
      
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("extend/include : unknown use case reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() != anUseCase)
    UmlCom::trace("'" + idref + "' is not a use case<br>");
  else {
    UmlItem * rel = UmlNcRelation::create(aDependency, this, *it);
    
    if (rel == 0)
      UmlCom::trace("cannot create dependency from '" + name() + "' to '" + (*it)->name() + "'");
    else
      rel->set_Stereotype((context) ? "extend" : "include ");
  }
}

void UmlUseCase::init()
{
  declareFct("ownedmember", "uml:UseCase", &importIt);
  declareFct("ownedusecase", "uml:UseCase", &importIt);
  declareFct("packagedelement", "uml:UseCase", &importIt);

  declareFct("extend", "uml:Extend", &importExtendInclude);
  declareFct("include", "uml:Include", &importExtendInclude);

  declareFct("extensionpoint", "uml:ExtensionPoint", &importExtensionPoint);

}

void UmlUseCase::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anUseCase, token, in);
    
  if (where != 0) {
    Q3CString s = token.valueOf("name");
    
    if (s.isEmpty()) {
      static unsigned n = 0;
      
      s.sprintf("anonymous_usecase_%u", ++n);
    }
    
    UmlUseCase * uc = create(where, s);
    
    if (uc == 0)
      in.error("cannot create use case '" + s +
	       "' in '" + where->name() + "'");
    
    uc->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	uc->UmlItem::import(in, token);
    }

    uc->unload(TRUE, FALSE);
  }
}

void UmlUseCase::importExtendInclude(FileIn & in, Token & token, UmlItem * where)
{
  if (where->kind() == anUseCase) {
    bool extend = (token.what() == "extend");
    Q3CString other = token.valueOf((extend) ? "extendedcase" : "addition");
    
    if (other.isEmpty())
      in.warning((extend) ? "extendedCase is missing" : "addition is missing");
    else {
      QMap<Q3CString, UmlItem *>::Iterator it = All.find(other);
      
      if (it != All.end())
	where->solve(extend, other);
      else
	UnresolvedWithContext::add(where, other, extend);
    }
  }

  if (! token.closed())
    in.finish(token.what());
}

void UmlUseCase::importExtensionPoint(FileIn & in, Token & token, UmlItem * where)
{
  if (where->kind() == anUseCase) {
    Q3CString ep = token.valueOf("name");
    
    if (!ep.isEmpty()) {
      Q3CString eps = ((UmlUseCase *) where)->extensionPoints();
      
      if (! eps.isEmpty())
	eps += "\n" + ep;
      else
	eps = ep;
      
      ((UmlUseCase *) where)->set_ExtensionPoints(eps);
    }
  }

  if (! token.closed())
    in.finish(token.what());
}

int UmlUseCase::NumberOf;

