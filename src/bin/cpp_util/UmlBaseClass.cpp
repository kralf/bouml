
#include "UmlBaseClass.h"
#include "UmlClass.h"
#include "UmlItem.h"
#include "UmlClassDiagram.h"
#include "UmlArtifact.h"
#include "UmlComponent.h"
#include "UmlPackage.h"

#include "UmlCom.h"
#include "ClassGlobalCmd.h"
#include "PackageGlobalCmd.h"
//Added by qt3to4:
#include <Q3CString>
#include <Q3ValueList>
UmlClass * UmlBaseClass::create(UmlItem * parent, const char * s)
{
  return (UmlClass *) parent->create_(aClass, s);
}

anItemKind UmlBaseClass::kind() {
  return aClass;
}

bool UmlBaseClass::isAbstract() {
  read_if_needed_();
  
  return _abstract;
}

bool UmlBaseClass::set_isAbstract(bool y) {
  return set_it_(_abstract, y, setIsAbstractCmd);
}

const UmlTypeSpec & UmlBaseClass::baseType() {
  read_if_needed_();
  
  return _base_type;
}

bool UmlBaseClass::set_BaseType(const UmlTypeSpec & t) {
  return set_it_(_base_type, t, setBaseTypeCmd);
}

Q3ValueList<UmlFormalParameter> UmlBaseClass::formals() {
  UmlCom::send_cmd(_identifier, formalsCmd);
  
  Q3ValueList<UmlFormalParameter> formals;
  
  for (unsigned n = UmlCom::read_unsigned(); n; n -= 1) {
    UmlFormalParameter f;
    
    f.read_();
    formals.append(f);
  }
  
  return formals;
}

bool UmlBaseClass::removeFormal(unsigned int rank) {
  UmlCom::send_cmd(_identifier, removeFormalCmd, rank);
  return UmlCom::read_bool();
}

bool UmlBaseClass::addFormal(unsigned int rank, const UmlFormalParameter & formal) {
  UmlCom::send_cmd(_identifier, addFormalCmd, rank, formal._name,
		   formal._type, formal._default_value, formal._extends);
  return UmlCom::read_bool();
}

bool UmlBaseClass::replaceFormal(unsigned int rank, const UmlFormalParameter & formal) {
  UmlCom::send_cmd(_identifier, replaceFormalCmd, rank, formal._name,
		   formal._type, formal._default_value, formal._extends);
  return UmlCom::read_bool();
}

Q3ValueList<UmlActualParameter> UmlBaseClass::actuals() {
  UmlCom::send_cmd(_identifier, actualsCmd);
  
  Q3ValueList<UmlActualParameter> actuals;
  
  for (unsigned n = UmlCom::read_unsigned(); n; n -= 1) {
    UmlActualParameter a;
    
    a.read_();
    actuals.append(a);
  }
  
  return actuals;
}

bool UmlBaseClass::replaceActual(unsigned int rank, const UmlTypeSpec & type) {
  UmlCom::send_cmd(_identifier, replaceActualCmd, rank, type);
  return UmlCom::read_bool();
}

UmlClassDiagram * UmlBaseClass::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseClass::set_AssociatedDiagram(UmlClassDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;

}

UmlArtifact * UmlBaseClass::associatedArtifact() {
  UmlCom::send_cmd(_identifier, assocArtifactCmd);
  
  return (UmlArtifact *) UmlBaseItem::read_();
}

const Q3PtrVector<UmlComponent> UmlBaseClass::associatedComponents() {
  UmlCom::send_cmd(_identifier, assocComponentCmd);

  Q3PtrVector<UmlComponent> result;
  unsigned n = UmlCom::read_unsigned();

  result.resize(n);

  for (unsigned index = 0; index != n; index += 1)
    result.insert(index, (UmlComponent *) UmlBaseItem::read_());

  return result;
}

#ifdef WITHCPP
bool UmlBaseClass::isCppExternal() {
  read_if_needed_();
  
  return _cpp_external;
}

bool UmlBaseClass::set_isCppExternal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsCppExternalCmd)) {
    _cpp_external = y;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHJAVA
bool UmlBaseClass::isJavaExternal() {
  read_if_needed_();
  
  return _java_external;
}

bool UmlBaseClass::set_isJavaExternal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsJavaExternalCmd)) {
    _java_external = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseClass::isJavaPublic() {
  return visibility() == PublicVisibility;
}

bool UmlBaseClass::set_isJavaPublic(bool y) {
  return set_Visibility((y) ? PublicVisibility : PackageVisibility);
}

bool UmlBaseClass::isJavaFinal() {
  read_if_needed_();
  
  return _java_final;
}

bool UmlBaseClass::set_isJavaFinal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsJavaFinalCmd)) {
    _java_final = y;
    return TRUE;
  }
  else
    return FALSE;

}
#endif

#ifdef WITHPHP
bool UmlBaseClass::isPhpExternal() {
  read_if_needed_();
  
  return _php_external;
}

bool UmlBaseClass::set_isPhpExternal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsPhpExternalCmd)) {
    _php_external = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseClass::isPhpFinal() {
  read_if_needed_();
  
  return _php_final;
}

bool UmlBaseClass::set_isPhpFinal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsPhpFinalCmd)) {
    _php_final = y;
    return TRUE;
  }
  else
    return FALSE;

}
#endif

#ifdef WITHPYTHON
bool UmlBaseClass::isPythonExternal() {
  read_if_needed_();
  
  return _python_external;
}

bool UmlBaseClass::set_isPythonExternal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsPythonExternalCmd)) {
    _python_external = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseClass::isPython_2_2() {
  read_if_needed_();
  return _python_2_2;
}

bool UmlBaseClass::set_isPython_2_2(bool v) {
  UmlCom::send_cmd(_identifier, setIsPython2_2Cmd, (char) v);
  if (UmlCom::read_bool()) {
    _python_2_2 = v;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHIDL
const UmlTypeSpec & UmlBaseClass::switchType() {
  read_if_needed_();
  
  return _switch_type;
}

bool UmlBaseClass::set_SwitchType(const UmlTypeSpec & t) {
  return set_it_(_switch_type, t, setSwitchTypeCmd);
}

bool UmlBaseClass::isIdlExternal() {
  read_if_needed_();
  
  return _idl_external;
}

bool UmlBaseClass::set_isIdlExternal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsIdlExternalCmd)) {
    _idl_external = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseClass::isIdlLocal() {
  read_if_needed_();
  
  return _idl_local;
}

bool UmlBaseClass::set_isIdlLocal(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsIdlLocalCmd)) {
    _idl_local = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseClass::isIdlCustom() {
  read_if_needed_();
  
  return _idl_custom;
}

bool UmlBaseClass::set_isIdlCustom(bool y) {
  bool r;
  
  if (set_it_(r, y, setIsIdlCustomCmd)) {
    _idl_custom = y;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

UmlClass * UmlBaseClass::get(const Q3CString & n, const UmlPackage * p)
{
  if (p == 0) {
    UmlClass * x = _classes[n];
    
    if (x != 0)
      return x;
  }
  
  UmlCom::send_cmd(classGlobalCmd, findClassCmd,
		   (p) ? p->_identifier : 0, n);
  
  return (UmlClass *) UmlBaseItem::read_();
}

UmlClass * UmlBaseClass::findStereotype(Q3CString s, bool caseSensitive)
{
  UmlCom::send_cmd(packageGlobalCmd, findStereotypeCmd,
                   (caseSensitive) ? "y" : "n", (const char *) s);
  return (UmlClass *) UmlBaseItem::read_();
}

void UmlBaseClass::unload(bool rec, bool del) {
  _base_type.explicit_type = 0;
  
#ifdef WITHIDL
  _switch_type.explicit_type = 0;
#endif
  
  UmlBaseClassItem::unload(rec, del);
}

bool UmlBaseClass::set_Name(const Q3CString & s) {
  if (!UmlBaseItem::set_Name(s))
    return FALSE;

  const Q3PtrVector<UmlItem> ch = children();
  Q3CString destr = "~" + name();

  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == anOperation) {
      if (ch[i]->name() == name())
	ch[i]->set_Name(s);
      else if (ch[i]->name() == destr)
	ch[i]->set_Name("~" + s);
    }
  }

  return TRUE;
}

Q3Dict<UmlClass> UmlBaseClass::_classes(1001);

 UmlBaseClass::UmlBaseClass(void * id, const Q3CString & n) 
    : UmlClassMember(id, n) {
  _assoc_diagram = 0;
  
  _classes.insert(n, (UmlClass *) this);
  
  if ((_classes.count() / 2) >= _classes.size())
    _classes.resize(_classes.size() * 2 - 1);
}

void UmlBaseClass::read_uml_() {
  _assoc_diagram = (UmlClassDiagram *) UmlBaseItem::read_();
  UmlBaseClassMember::read_uml_();
  _abstract = UmlCom::read_bool();
  
  if (_stereotype == "typedef") {
    _base_type.type = (UmlClass *) UmlBaseItem::read_();
    if (_base_type.type == 0)
      _base_type.explicit_type = UmlCom::read_string();
  }
}

#ifdef WITHCPP
void UmlBaseClass::read_cpp_() {
  UmlBaseClassMember::read_cpp_();  
  _cpp_external = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseClass::read_java_() {
  UmlBaseClassMember::read_java_();
  ;
  _java_final = UmlCom::read_bool();
  _java_external = UmlCom::read_bool();
}
#endif

#ifdef WITHPHP
void UmlBaseClass::read_php_() {
  UmlBaseClassMember::read_php_();
  ;
  _php_final = UmlCom::read_bool();
  _php_external = UmlCom::read_bool();
}
#endif

#ifdef WITHPYTHON
void UmlBaseClass::read_python_() {
  UmlBaseClassMember::read_python_();
  _python_2_2 = UmlCom::read_bool();
  _python_external = UmlCom::read_bool();
}
#endif

#ifdef WITHIDL
void UmlBaseClass::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _switch_type.type = (UmlClass *) UmlBaseItem::read_();
  if (_switch_type.type == 0)
    _switch_type.explicit_type = UmlCom::read_string();
  _idl_local = UmlCom::read_bool();
  _idl_custom = UmlCom::read_bool();
  _idl_external = UmlCom::read_bool();
}
#endif

void UmlBaseClass::reread_if_needed_() {
  if (_defined) {
    UmlCom::send_cmd(_identifier, getUmlDefCmd);
    read_uml_();
  }
}

