
#include "UmlOperation.h"
#include "UmlClass.h"

#include "CppSettings.h"
//Added by qt3to4:
#include <Q3CString>

void UmlOperation::defaultDef() {
  if (cppDecl().isEmpty())
    set_CppDecl(CppSettings::operationDecl());
    
  if (isAbstract())
    set_CppDef("");
  else if (cppDef().isEmpty())
    set_CppDef(CppSettings::operationDef());

  setUsed();
}

void UmlOperation::setType(const char * type, const char * s) {
  UmlTypeSpec t;
  
  t.explicit_type = type;
  set_ReturnType(t);
 
  static const Q3CString sep = " \t\n\r";
  Q3CString d;
  int typeindex;
  int nameindex;
  
  d = cppDecl();
  if (((typeindex = d.find("${type}")) != -1) &&
      ((nameindex = d.find(" ${name}", typeindex + 7)) != -1)) {
    while (sep.find(d[nameindex - 1]) != -1)
      nameindex -= 1;
    d.replace(typeindex, nameindex - typeindex, s);
    set_CppDecl(d);
  }

  if (! isAbstract()) {
    d = cppDef();
    if (((typeindex = d.find("${type}")) != -1) &&
	((nameindex = d.find(" ${class}", typeindex + 7)) != -1)) {
      while (sep.find(d[nameindex - 1]) != -1)
	nameindex -= 1;
      d.replace(typeindex, nameindex - typeindex, s);
      set_CppDef(d);
    }
  }
}

void UmlOperation::setType(UmlClass * type, const char * s) {
  UmlTypeSpec t;
  
  t.type = type;
  set_ReturnType(t);
 
  static const Q3CString sep = " \t\n\r";
  Q3CString d;
  int typeindex;
  int nameindex;
  
  d = cppDecl();
  if (((typeindex = d.find("${type}")) != -1) &&
      ((nameindex = d.find("${name}", typeindex + 7)) != -1)) {
    while (sep.find(d[nameindex - 1]) != -1)
      nameindex -= 1;
    d.replace(typeindex, nameindex - typeindex, s);
    set_CppDecl(d);
  }

  if (! isAbstract()) {
    d = cppDef();
    if (((typeindex = d.find("${type}")) != -1) &&
	((nameindex = d.find("${class}", typeindex + 7)) != -1)) {
      while (sep.find(d[nameindex - 1]) != -1)
	nameindex -= 1;
      d.replace(typeindex, nameindex - typeindex, s);
      set_CppDef(d);
    }
  }
}

void UmlOperation::addParam(int rank, aDirection dir, const char * name, UmlClass * type) {
  UmlParameter p;
    
  p.dir = dir;
  p.type.type = type;
  p.name = name;
      
  if (params().count() <= (unsigned) rank)
    addParameter(rank, p);
  else
    replaceParameter(rank, p);
}

void UmlOperation::setParams(const char * s) {
  Q3CString d;
  int index;
  
  d = cppDecl();
  if (((index = d.find("${(}")) != -1) &&
      (d.mid(index + 4, 4) == "${)}")) {
    d.insert(d.find("${(}") + 4, s);
    set_CppDecl(d);
  }

  d = cppDef();
  if (((index = d.find("${(}")) != -1) &&
      (d.mid(index + 4, 4) == "${)}")) {
    d.insert(d.find("${(}") + 4, s);
    set_CppDef(d);
  }
}

void UmlOperation::setUsed() {
  UmlItem::setUsed();
  set_isBodyGenerationForced(TRUE);
}

