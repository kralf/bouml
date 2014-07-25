
#include "UmlOperation.h"
#include "File.h"
#include "UmlClass.h"

#include <qstringlist.h> 
//Added by qt3to4:
#include <Q3CString>

#include "UmlCom.h"
#include "util.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
void UmlOperation::import(File & f, UmlClass * parent)
{
  Q3CString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, "operations's name");
    
  Q3CString id;
  Q3CString ste;
  Q3CString doc;
  Q3Dict<Q3CString> prop;
  Q3CString s2;
  int k;
  
  do {
    k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
  } while (id.isEmpty());

  UmlOperation * x;

  if (scanning) {
    Q3CString name;
    
    if (s.left(8) != "operator")
      name = (s.at(0) == '~')
	? ("~" + legalName(s.mid(1)))
	: legalName(s);
    else
      name = s;
    
    if ((x = UmlBaseOperation::create(parent, name)) == 0) {
      UmlCom::trace("<br>cannot create operation '" + s + "' in " +
		    parent->fullName());
      throw 0;
    }
    newItem(x, id);
    
    if (!ste.isEmpty()) {
      bool managed = FALSE;
      QStringList l = QStringList::split(",", QString(ste));
      
      for (QStringList::Iterator it = l.begin();
	   it != l.end();
	   ++it) {
	if ((*it) == "const") {
	  managed = TRUE;
	  x->set_isCppConst(TRUE);
	}
	else if ((*it) == "abstract") {
	  managed = TRUE;
	  x->set_isAbstract(TRUE);
	  x->set_isCppVirtual(TRUE);
	}
	else if ((*it) == "virtual") {
	  managed = TRUE;
	  x->set_isCppVirtual(TRUE);
	}
	else if ((*it) == "static") {
	  managed = TRUE;
	  x->set_isClassMember(TRUE);
	}
      }
      
      if (!managed)
	x->set_Stereotype(ste);
    }
    
    if (!doc.isEmpty())
      x->set_Description(doc);
  }
  else if ((x = (UmlOperation *) findItem(id, anOperation)) == 0) {
    UmlCom::trace("<br>unknown operation '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }
  else {
    switch (((UmlClass *) x->parent())->language()) {
    case Cplusplus:
    case AnsiCplusplus:
    case VCplusplus:
      x->cplusplus(prop);
      break;
    case Oracle8:
      x->oracle8(prop);
      break;
    case Corba:
      x->corba(prop);
      break;
    case Java:
      x->java(prop);
      break;
    default:
      break;
    }
    
    x->setProperties(prop);
  }

  f.unread(k, s2);
  x->import(f);
}

void UmlOperation::import(File & f) {
  if (scanning) {
    f.skipBlock();
    return;
  }

  Q3CString s;
  UmlTypeSpec t;

  for (;;) {    
    switch (f.read(s)) {
    case -1:
      f.eof();
      throw 0;
    case ')':
      set_ReturnType(t);
      return;
    case ATOM:
      break;
    default:
      f.syntaxError(s);
    }
    
    // atom
    if (s == "parameters") {
      f.read("(");
      f.read("list");
      f.read("Parameters");
      importParameters(f);
    }
    else if (s == "result") {
      if (f.read(s) != STRING)
        f.syntaxError(s, "return type");
      t.explicit_type = s;
    }
    else if (s == "quidu") {
      if (f.read(s) != STRING)
	f.syntaxError(s, "wrong quidu");
      if (UmlClass::replaceType(t, s, "${type}")) {
	switch (((UmlClass *) parent())->language()) {
	case Cplusplus:
	case AnsiCplusplus:
	case VCplusplus:
	  set_CppDecl(replace(cppDecl(), "${type}", s));
	  set_CppDef(replace(cppDef(), "${type}", s));
	  break;
	case Oracle8:
	  break;
	case Corba:
	  set_IdlDecl(replace(idlDecl(), "${type}", s));
	  break;
	case Java:
	  set_JavaDecl(replace(javaDecl(), "${type}", s));
	  break;
	default:
	  break;
	}
      }
    }
    else if (s == "exceptions")
      importExceptions(f);
    else if (s == "opExportControl")
      set_Visibility(f.readVisibility());
    else
      f.skipNextForm();
  }

}

void UmlOperation::importParameters(File & f) {
  Q3CString s;
  unsigned rank = 0;
  const char * sep = "";
  Q3CString doc = description();

  for (;;) {
    switch (f.read(s)) {
    case ')':
      if (doc != description())
        set_Description(doc);
      return;
    case '(':
      // a parameter
      break;
    default:
      f.syntaxError(s);
    }

    f.read("object");
    f.read("Parameter");

    UmlParameter p;
    Q3CString ti;
	
    ti.sprintf("${t%u}", rank);

    if (f.read(p.name) != STRING)
      f.syntaxError(s, "parameter's name");

    Q3CString id;
    Q3CString ste;
    Q3CString p_doc;
    Q3Dict<Q3CString> prop;
    int k;

    for (;;) {
      k = f.readDefinitionBeginning(s, id, ste, p_doc, prop);
      
      if (k == ')')
	break;
      
      if (s == "type") {
        if (f.read(s) != STRING)
          f.syntaxError(s, "parameter type");
	
	switch (((UmlClass *) parent())->language()) {
	case Corba:
	  if (strncmp(s,  "in ", 3) == 0) {
	    p.dir = InputDirection;
	    s = s.mid(3);
	  }
	  else if (strncmp(s,  "out ", 4) == 0) {
	    p.dir = OutputDirection;
	    s = s.mid(4);
	  }
	  else if (strncmp(s,  "inout ", 6) == 0) {
	    p.dir = InputOutputDirection;
	    s = s.mid(6);
	  }
	  else {
	    Q3CString err =
	      "<br>'" + s + "' : wrong parameter direction, in " + f.context();
	  
	    UmlCom::trace(err);
	    p.dir = InputOutputDirection;
	  }
	  break;
	default:
	  // !!!!!!!!!!!!!
	  p.dir = InputOutputDirection;
	}
	
        p.type.explicit_type = s;
      }
      else if (s == "quidu") {
	if (f.read(s) != STRING)
	  f.syntaxError(s, "wrong quidu");
	
	if (UmlClass::replaceType(p.type, s, ti))
	  ti = s;
      }
      else
	f.skipNextForm();
    }

    Q3CString d;
    int index;
    
    switch (((UmlClass *) parent())->language()) {
    case Cplusplus:
    case AnsiCplusplus:
    case VCplusplus:
      s.sprintf("%s%s ${p%u}", sep, (const char *) ti, rank);
      if ((index = (d = cppDecl()).find("${)}")) != -1)
	//set_CppDecl(d.insert(index, s));//[jasa] original line
	set_CppDecl(d.insert(index, (const char*)s));//[jasa] fix ambiguous call
      if ((index = (d = cppDef()).find("${)}")) != -1)
	set_CppDef(d.insert(index, (const char*)s));//[jasa] fix ambiguous call
      break;
    case Oracle8:
      break;
    case Corba:
      if ((index = (d = idlDecl()).find("${)}")) != -1) {
	s.sprintf("%s${d%u} %s ${p%u}", 
		  sep, rank, (const char *) ti, rank);
	set_IdlDecl(d.insert(index, (const char*)s));//[jasa] fix ambiguous call
      }
      break;
    case Java:
      if ((index = (d = javaDecl()).find("${)}")) != -1) {
	s.sprintf("%s%s ${p%u}", sep, (const char *) ti, rank);
	set_JavaDecl(d.insert(index, (const char*)s));//[jasa]
      }
      break;
    default:
      break;
    }
    
    addParameter(rank++, p);
    sep = ", ";
  }
}

void UmlOperation::importExceptions(File & f) {
  Q3CString s;
  unsigned rank = 0;
  
  if (f.read(s) != STRING)
    f.syntaxError(s, "the exceptions");
    
  int index = 0;
  
  while (!(s = s.mid(index).stripWhiteSpace()).isEmpty()) {
    if ((index = s.find(' ')) == -1)
      index = s.length();
      
    UmlTypeSpec t;
    int index2;
    
    t.explicit_type = s.left(index);
    
    if (((index2 = t.explicit_type.find("[")) != -1) &&
	(((const char *) t.explicit_type)[t.explicit_type.length() - 1]
	 == ']')) {
      Q3CString target_id =
	t.explicit_type.mid(index2+1, 
			    t.explicit_type.length() - index2 - 2);
      UmlClass * cl = (UmlClass *) UmlItem::findItem(target_id, aClass);
      
      if (cl != 0) {
	t.explicit_type = 0;
	t.type = cl;
      }
      else
	t.explicit_type.truncate(index2);
    }
    
    addException(rank++, t);
  }
}

void UmlOperation::cplusplus(Q3Dict<Q3CString> &) {
  set_CppDecl(CppSettings::operationDecl());
  set_CppDef(CppSettings::operationDef());
}

void UmlOperation::oracle8(Q3Dict<Q3CString> &) {
}

void UmlOperation::corba(Q3Dict<Q3CString> & prop) {
  Q3CString * v;
    
  if ((v = prop.find("CORBA/OperationIsOneWay")) != 0) {
    if (*v == "TRUE")
      set_isIdlOneway(TRUE);
    prop.remove("CORBA/OperationIsOneWay");
  }

  set_IdlDecl(IdlSettings::operationDecl());
}

void UmlOperation::java(Q3Dict<Q3CString> & prop) {
  Q3CString d = JavaSettings::operationDef();
  Q3CString * v;
    
  if ((v = prop.find("Java/Final")) != 0) {
    if (*v == "TRUE")
      set_isJavaFinal(TRUE);
    prop.remove("Java/Final");
  }
  if ((v = prop.find("Java/Synchronized")) != 0) {
    if (*v == "TRUE")
      set_isJavaSynchronized(TRUE);
    prop.remove("Java/Synchronized");
  }
  if ((v = prop.find("Java/Static")) != 0) {
    if (*v == "TRUE")
      set_isClassMember(TRUE);
    prop.remove("Java/Static");
  }
  if ((v = prop.find("Java/Abstract")) != 0) {
    if (*v == "TRUE")
      set_isAbstract(TRUE);
    prop.remove("Java/Abstract");
  }
  if ((v = prop.find("Java/Strictfp")) != 0) {
    if (*v == "TRUE") {
      int index = d.find("${final}");
      
      if (index != -1)
	d.insert(index, "strictfp ");
    }
      
    prop.remove("Java/Strictfp");
  }

  set_JavaDef(d);
}

