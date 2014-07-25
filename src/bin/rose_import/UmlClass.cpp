
#include "UmlClass.h"
#include "File.h"
#include "UmlItem.h"
#include "UmlTypeSpec.h"

#include "UmlCom.h"
#include "UmlAttribute.h"
#include "UmlOperation.h"
#include "UmlRelation.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
#include "util.h"
#include "Artifact.h"
//Added by qt3to4:
#include <Q3CString>

UmlClass * UmlClass::import(File & f, UmlItem * parent, const Q3CString & knd)
{
  Q3CString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, "class's name");
    
  Q3CString id;
  Q3CString ste;
  Q3CString doc;
  Q3Dict<Q3CString> prop;
  Q3CString s2;
  int k;
  
  do {
    k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
  } while (id.isEmpty());

  if (ste == "CORBAConstant") {
    // not a class !
    if (!scanning) {
      if (parent->kind() == aClass)
	UmlAttribute::importIdlConstant((UmlClass *) parent, id, s, doc, prop);
      else
	importIdlConstant(parent, id, s, doc, prop);
    }
    
    if (k != ')')
      f.skipBlock();
    
    return 0;
  }

  UmlClass * cl;
  
  if (scanning) {
    if (((cl = UmlBaseClass::create(parent, s)) == 0) &&
	((cl = UmlBaseClass::create(parent, legalName(s))) == 0)) {
      UmlCom::trace("<br>cannot create class '" + s + "' in " +
		    parent->fullName());
      throw 0;
    }
    newItem(cl, id);
    
    if (!ste.isEmpty()) {
      if (ste.left(5) == "CORBA") {
	if (ste != "CORBAValue")
	  cl->set_Stereotype(ste.mid(5).lower());
      }
      else 
	cl->set_Stereotype(((ste == "Actor") || (ste == "Interface"))
			   ? ste.lower() : ste);
    }
    
    if (!doc.isEmpty())
      cl->set_Description(doc);
    
    cl->lang = None;  
  }
  else if ((cl = (UmlClass *) findItem(id, aClass)) == 0) {
    UmlCom::trace("<br>unknown class '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }
  
  Q3CString art_path;
  
  for (;;) {
    switch (k) {
    case ')':
      switch (cl->lang) {
      case Cplusplus:
      case AnsiCplusplus:
      case VCplusplus:
	cl->cplusplus(prop);
	break;
      case Oracle8:
	cl->oracle8(prop);
	break;
      case Corba:
	cl->corba(prop);
	break;
      case Java:
	cl->java(prop);
	break;
      default:
	break;
      }
      
      if (!scanning) {
	cl->setProperties(prop);
	cl->unload(TRUE);
      }
      return cl;
    case ATOM:
      if (s2 == "operations")
	cl->importOperations(f);
      else if (s2 == "class_attributes")
	cl->importAttributes(f);
      else if (!scanning &&
	       ((s2 == "superclasses") ||
		(s2 == "used_nodes") ||
		(s2 == "realized_interfaces")))
	cl->importRelations(f);
      else if (s2 == "nestedClasses")
	cl->importClasses(f);
      else if (s2 == "abstract") {
	if (f.readBool())
	  cl->set_isAbstract(TRUE);
      }
      else if (s2 == "language")
	cl->lang = f.readLanguage();
      else if (s2 == "instantiation_relationship") 
	cl->importInstantiate(f);
      else if (s2 == "parameters") {
	if (knd == "Parameterized_Class")
	  cl->importFormals(f);
        else
	  cl->importActuals(f);
      }
      else if (s2 == "module") {
	if (f.read(art_path) != STRING)
	  f.syntaxError(art_path, "module's name");
      }
      else if (!scanning && (s2 == "quidu")) {
	f.read(s2);
	cl->assocArtifact(Artifact::find(s2), art_path);
      }
      else
	f.skipNextForm();
      k = f.read(s2);
      break;
    default:
      f.syntaxError(s);
    }
  }
}

bool UmlClass::replaceType(UmlTypeSpec & t, Q3CString & target_id, const Q3CString & ts)
{
  UmlClass * cl = (UmlClass *) findItem(target_id, aClass);
  bool result = FALSE;
  
  if (cl != 0) {
    int index = 0;
    const Q3CString & s = cl->name();
    unsigned ln1 = s.length();
    unsigned ln2 = ts.length();
    
    while ((index = t.explicit_type.find(s, index)) != -1) {
      if (((index == 0) || isSep(((const char *) t.explicit_type)[index - 1])) &&
	  isSep(((const char *) t.explicit_type)[index + (int) ln1])) {
	t.explicit_type.replace((unsigned) index, ln1, ts);
	index += ln2;
	result = TRUE;
      }
      else
	index += 1;
    }
  }

  if (result) {
    target_id = t.explicit_type;
    t.explicit_type = 0;
    t.type = cl;
  }

  return result;
}

void UmlClass::importAttributes(File & f) {
  f.read("(");
  f.read("list");
  f.read("class_attribute_list");
  
  for (;;) {
    Q3CString s;
  
    switch (f.read(s)) {
    case -1:
      f.eof();
    case ')':
      return;
    case '(':
      f.read("object");
      f.read("ClassAttribute");
      UmlAttribute::import(f, this);
      break;
    default:
      f.syntaxError(s);
    }
  }

}

void UmlClass::importOperations(File & f) {
  f.read("(");
  f.read("list");
  f.read("Operations");
  
  for (;;) {
    Q3CString s;
  
    switch (f.read(s)) {
    case -1:
      f.eof();
    case ')':
      return;
    case '(':
      f.read("object");
      f.read("Operation");
      UmlOperation::import(f, this);
      break;
    default:
      f.syntaxError(s);
    }
  }

}

void UmlClass::importRelations(File & f) {
  Q3CString s;
  
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
    Q3CString sr;
    
    if (s == "Uses_Relationship") {
      rk = aDependency;
      sr = "dependency";
    }
    else if (s == "Inheritance_Relationship") {
      rk = aGeneralisation;
      sr = "generalisation";
    }
    else if (s == "Realize_Relationship") {
      rk = aRealization;
      sr = "realization";
    }
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
    
    Q3CString target_id;
    aVisibility visibility = PublicVisibility;
    bool virtual_inheritance = FALSE;
    bool a_friend = FALSE;
    
    for (;;) {
      if (k == ATOM) {
	if (s2 == "quidu") {
	  if (f.read(target_id) != STRING)
	    f.syntaxError(target_id, "quidu value");
	}
	else if (s2 == "exportControl")
	  visibility = f.readVisibility();
	else if (s2 == "virtual")
	  virtual_inheritance = f.readBool();
	else if (s2 == "friend")
	  a_friend = f.readBool();
	else
	  f.skipNextForm();
	k = f.read(s2);
      }
      else if (k == ')')
	break;
      else
	f.syntaxError(s2);
    }
    
    if (target_id.isEmpty())
      f.syntaxError("quidu missing");
    
    UmlClass * target = (UmlClass *) findItem(target_id, aClass);
    
    if (target != 0) {
      UmlRelation * r;
      
      if (a_friend) {
	if ((r = UmlRelation::create(rk, target, this)) == 0) {
	  UmlCom::trace("<br>cannot create " + sr + " from '" +
			target->fullName() + "' to '" + fullName() + "'");
	  f.skipBlock();
	  return;
	}
	r->set_Stereotype("friend");
      }
      else {
	if ((r = UmlRelation::create(rk, this, target)) == 0) {
	  UmlCom::trace("<br>cannot create " + sr + " from '" +
			fullName() + "' to '" + target->fullName() + "'");
	  f.skipBlock();
	  return;
	}

	if (!ste.isEmpty())
	  r->set_Stereotype(ste);
	if (visibility != PublicVisibility)
	  r->set_Visibility(visibility);
	if (virtual_inheritance)
	  r->set_CppVirtualInheritance(TRUE);
      }
      
      newItem(r, id);
      if (!doc.isEmpty())
	r->set_Description(doc);
      r->setProperties(prop);
    }
  }
}

void UmlClass::importInstantiate(File & f) {
  if (scanning) {
    f.skipNextForm();
    return;
  }

  f.read("(");
  f.read("object");
  f.read("Instantiation_Relationship");

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
  
  UmlClass * target = (UmlClass *) findItem(s2, aClass);
  
  if (target != 0) {
    UmlRelation * r = UmlRelation::create(aRealization, this, target);
    
    if (r == 0)
      UmlCom::trace("<br>cannot create aRealization from '" +
		    fullName() + "' to '" + target->fullName() + "'");
    else {
      newItem(r, id);
      if (!ste.isEmpty())
	r->set_Stereotype(ste);
      if (!doc.isEmpty())
	r->set_Description(doc);
      r->setProperties(prop);
    }
  }
  
  f.skipBlock();
}

void UmlClass::importActuals(File & f) {
  if (scanning) {
    f.skipNextForm();
    return;
  }
    
  f.read("(");
  f.read("list");
  f.read("Parameters");
  
  Q3CString s;
  unsigned rank = 0;
  
  for (;;) {
    switch (f.read(s)) {
    case ')':
      return;
    case '(':
      {
	UmlTypeSpec t;
	
	UmlActualParameter::import(f, t);
	replaceActual(rank++, t);
      }
      break;
    default:
      f.syntaxError(s, "(");
    }
  }
}

void UmlClass::importFormals(File & f) {
  f.read("(");
  f.read("list");
  f.read("Parameters");
  
  Q3CString s;
  unsigned rank = 0;
  
  for (;;) {
    switch (f.read(s)) {
    case ')':
      return;
    case '(':
      {
	UmlFormalParameter p;
	
	p.import(f, scanning);
	if (scanning)
	  addFormal(rank++, p);
	else
	  replaceFormal(rank++, p);
      }
      break;
    default:
      f.syntaxError(s, "(");
    }
  }
}

void UmlClass::importClasses(File & f) {
  f.read("(");
  f.read("list");
  f.read("nestedClasses");
  
  Q3CString s;
  
  for (;;) {
    switch (f.read(s)) {
    case ')':
      return;
    case '(':
      f.read("object");

      if (f.read(s) != ATOM)
	f.syntaxError(s, "an atom");
      
      if ((s == "Class") || (s == "Metaclass") ||
	  (s == "Instantiated_Class") || (s == "Parameterized_Class")) {
	UmlClass * cl = UmlClass::import(f, this, s);
	
	if (cl != 0)
	  // not a CORBAConstant
	  cl->lang = lang;
      }
      else
	f.syntaxError(s);
      break;
    default:
      f.syntaxError(s, "(");
    }
  }
}

void UmlClass::importIdlConstant(UmlItem * parent, const Q3CString & id, const Q3CString & s, const Q3CString & doc, Q3Dict<Q3CString> & prop)
{
  // use a class to define the constant !
  UmlClass * x;

  if ((x = UmlClass::create(parent, legalName(s))) == 0) {
    UmlCom::trace("<br>cannot create class '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }

  newItem(x, id);
  x->lang = Corba;
  x->set_Stereotype("constant");
  
  if (!doc.isEmpty())
    x->set_Description(doc);

  Q3CString type;
  Q3CString value;
  Q3CString * v;
  
  if ((v = prop.find("CORBA/ImplementationType")) != 0) {
    type = *v;
    prop.remove("CORBA/ImplementationType");
  }

  if ((v = prop.find("CORBA/ConstValue")) != 0) {
    if (!v->isEmpty())
      value = " = " + *v;
    prop.remove("CORBA/ConstValue");
  }

  Q3CString d = IdlSettings::constDecl();
  int index;
  
  if ((index = d.find("${type}")) != -1)
    d.replace(index, 7, type);
    
  if ((index = d.find("${value}")) != -1)
    d.replace(index, 8, value);
  
  x->setProperties(prop);
  x->set_IdlDecl(d);
}

void UmlClass::cplusplus(Q3Dict<Q3CString> & prop) {
  if (!scanning) {
    if (stereotype() == "typedef") {
      Q3CString * bt = prop.find("Cplusplus/ImplementationType");
      
      if (bt != 0) {
	UmlTypeSpec t;
	
	t.explicit_type = *bt;	// no quidu
	set_BaseType(t);
      }
      
      set_CppDecl(CppSettings::typedefDecl());
    }
    else if (stereotype() == "struct")
      set_CppDecl(CppSettings::structDecl());
    else if (stereotype() == "union")
      set_CppDecl(CppSettings::unionDecl());
    else if (stereotype() == "enum")
      set_CppDecl(CppSettings::enumDecl());
    else
      set_CppDecl(CppSettings::classDecl());
    
    prop.remove("Cplusplus/ImplementationType");
  }

  Q3CString * v;
  
  if ((v = prop.find("Cplusplus/BodySourceFile")) != 0) {
    _body_file = *v;
    prop.remove("Cplusplus/BodySourceFile");
  }
  else if ((v = prop.find("Traversal/BodyFile")) != 0) {
    _body_file = *v;
    prop.remove("Traversal/BodyFile");
  }
  
  if ((v = prop.find("Cplusplus/HeaderSourceFile")) != 0) {
    _file = *v;
    prop.remove("Cplusplus/HeaderSourceFile");
  }
  else if ((v = prop.find("Traversal/CodeFile")) != 0) {
    _file = *v;
    prop.remove("Traversal/CodeFile");
  }
}

void UmlClass::oracle8(Q3Dict<Q3CString> &) {
}

void UmlClass::corba(Q3Dict<Q3CString> & prop) {
  if (!scanning) {
    Q3CString * v;
    
    if (stereotype() == "union") {
      if ((v = prop.find("CORBA/ImplementationType")) != 0) {
	UmlTypeSpec t;
	
	t.explicit_type = *v;	// !!!!!!!!!!!!
	set_SwitchType(t);
	prop.remove("CORBA/ImplementationType");
      }
      
      set_IdlDecl(IdlSettings::unionDecl());
    }
    else if (stereotype() == "typedef") {
      if ((v = prop.find("CORBA/ImplementationType")) != 0) {
	UmlTypeSpec t;
	
	t.explicit_type = *v;	// no quidu
	set_BaseType(t);
	prop.remove("CORBA/ImplementationType");
      }
      
      QString d = IdlSettings::typedefDecl();
      
      if ((v = prop.find("CORBA/ArrayDimensions")) != 0) {
	if (!v->isEmpty()) {
	  int index;
	  
	  if ((index = d.find("${name}")) != -1)
	    d.insert(index + 7, "[" + *v + "]");
	}
	
	prop.remove("CORBA/ArrayDimensions");
      }
      
      set_IdlDecl(d);
    }
    else if (stereotype() == "struct")
      set_IdlDecl(IdlSettings::structDecl());
    else if (stereotype() == "enum")
      set_IdlDecl(IdlSettings::enumDecl());
    else if (stereotype() == "interface")
      set_IdlDecl(IdlSettings::interfaceDecl());
    else if (stereotype() == "exception")
      set_IdlDecl(IdlSettings::exceptionDecl());
    else if (stereotype() == "")
      set_IdlDecl(IdlSettings::valuetypeDecl());
  }
}

void UmlClass::java(Q3Dict<Q3CString> & prop) {
  if (!scanning) {
    Q3CString d = (stereotype() == "interface") 
      ? JavaSettings::interfaceDecl()
      : JavaSettings::classDecl();
    
    Q3CString * v;
    
    if ((v = prop.find("Java/Final")) != 0) {
      if (*v == "TRUE")
	set_isJavaFinal(TRUE);
      prop.remove("Java/Final");
    }
    
    if ((v = prop.find("Java/Strictfp")) != 0) {
      if (*v == "TRUE") {
	int index;
	
	if ((index = d.find("${public}")) != -1)
	  d.insert((unsigned) index + 9, "strictfp ");
	else if ((index = d.find("${visibility}")) != -1)
	  d.insert((unsigned) index + 13, "strictfp ");
      }
      prop.remove("Java/Strictfp");
    }
    
    set_JavaDecl(d);
  }
}

void UmlClass::assocArtifact(Artifact * c, Q3CString & art_path) {
  if ((c != 0) && (parent()->kind() == aClassView))
    c->add((UmlPackage *) parent()->parent(), this, art_path);
}

