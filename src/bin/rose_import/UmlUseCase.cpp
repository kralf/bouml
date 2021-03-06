
#include "UmlUseCase.h"
#include "UmlItem.h"
#include "File.h"

#include "UmlNcRelation.h"
#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>

UmlItem * UmlUseCase::item() {
  return this;
}

void UmlUseCase::readObject(File & f, Q3CString s) {
  if (!UmlItem::scanning &&
      ((s == "superclasses") || (s == "visible_modules"))) {
    f.read("(");
    f.read("list");
    if (f.read(s) != ATOM)
      f.syntaxError(s, "an atom");
    
    for (;;) {
      switch (f.read(s)) {
      case ')':
	return;
      case '(':
	break;
      default:
	f.syntaxError(s);
      }
      
      f.read("object");
      
      if (f.read(s) != ATOM)
	f.syntaxError(s, "an atom");
      
      aRelationKind rk;
      
      if (s == "Dependency_Relationship")
	rk = aDependency;
      else if (s == "Inheritance_Relationship")
	rk = aGeneralisation;
      else {
	f.skipBlock();
	continue;
      }
      
      // dependency or generalisation
      Q3CString id;
      Q3CString ste;
      Q3CString doc;
      Q3Dict<Q3CString> prop;
      Q3CString s2;
      int k;
  
      do {
	k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
      } while (id.isEmpty());
      
      for (;;) {
	if (k == ATOM) {
	  if (s2 == "quidu")
	    break;
	  f.skipNextForm();
	  k = f.read(s2);
	}
	else
	  f.syntaxError(s2);
      }
      
      if (f.read(s2) != STRING)
	f.syntaxError(s2, "quidu value");

      UmlUseCase * target = (UmlUseCase *) findItem(s2, anUseCase);
      
      if (target != 0) {
	UmlNcRelation * r = UmlNcRelation::create(rk, this, target);

	if (r == 0) {
	  UmlCom::trace(((rk == aDependency)
			 ? "<br>cannot create dependency from '"
			 : "<br>cannot create generalisation from '") +
			fullName() + "' to '" + target->fullName() + "'");
	  throw 0;
	}
	
	newItem(r, id);
	if (!doc.isEmpty())
	  r->set_Description(doc);
        r->setProperties(prop);
      }
      
      f.skipBlock();
    }
  }
  else
    f.skipNextForm();
}

void UmlUseCase::import(File & f, UmlItem * parent)
{
  Q3CString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, "use case's name");
    
  Q3CString id;
  Q3CString ste;
  Q3CString doc;
  Q3Dict<Q3CString> prop;
  Q3CString s2;
  int k;
  
  do {
    k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
  } while (id.isEmpty());

  UmlUseCase * x;

  if (UmlItem::scanning) {
    if ((x = UmlBaseUseCase::create(parent, s)) == 0) {
      UmlCom::trace("<br>cannot create use case '" + s + "' in " +
		    parent->fullName());
      throw 0;
    }
    newItem(x, id);

    if (!doc.isEmpty())
      x->set_Description(doc);
    
    x->setProperties(prop);
  }
  else if ((x = (UmlUseCase *) findItem(id, anUseCase)) == 0) {
    UmlCom::trace("<br>unknown use case '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }
  
  f.unread(k, s2);
  x->Uc::import(f);
}

UmlUseCase::~UmlUseCase() {
}

