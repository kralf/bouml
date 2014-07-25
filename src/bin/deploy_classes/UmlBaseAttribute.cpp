
#include "UmlBaseAttribute.h"
#include "UmlAttribute.h"
#include "UmlClass.h"
#include "UmlOperation.h"

#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>
UmlAttribute * UmlBaseAttribute::create(UmlClass * parent, const char * s)
{
  return (UmlAttribute *) parent->create_(anAttribute, s);
}

anItemKind UmlBaseAttribute::kind() {
  return anAttribute;
}

bool UmlBaseAttribute::isReadOnly() {
  read_if_needed_();
  
  return _read_only;
}

bool UmlBaseAttribute::set_isReadOnly(bool y) {
  UmlCom::send_cmd(_identifier, setIsReadOnlyCmd, (char) y);
  if (UmlCom::read_bool()) {
    _read_only = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseAttribute::isDerived() {
  read_if_needed_();
  return _derived;
}

bool UmlBaseAttribute::isDerivedUnion() {
  read_if_needed_();
  return _derived_union;
}

bool UmlBaseAttribute::set_isDerived(bool is_derived, bool is_union) {
  UmlCom::send_cmd(_identifier, setDerivedCmd,
                   (char) (((is_derived) ? 1 : 0) + ((is_union) ? 2 : 0)));
  if (UmlCom::read_bool()) {
    _derived = is_derived;
    _derived_union = is_union;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseAttribute::isOrdered() {
  read_if_needed_();
  return _ordered;
}

bool UmlBaseAttribute::set_isOrdered(bool v) {
  UmlCom::send_cmd(_identifier, setOrderingCmd, (char) v);
  if (UmlCom::read_bool()) {
    _ordered = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseAttribute::isUnique() {
  read_if_needed_();
  return _unique;
}

bool UmlBaseAttribute::set_isUnique(bool v) {
  UmlCom::send_cmd(_identifier, setUniqueCmd, (char) v);
  if (UmlCom::read_bool()) {
    _unique = v;
    return TRUE;
  }
  else
    return FALSE;
}

const Q3CString & UmlBaseAttribute::defaultValue() {
  read_if_needed_();
  
  return _default_value;
}

bool UmlBaseAttribute::set_DefaultValue(const char * s) {
  return set_it_(_default_value, s, setDefaultValueCmd);
}

const UmlTypeSpec & UmlBaseAttribute::type() {
  read_if_needed_();
  
  return _type;
}

bool UmlBaseAttribute::set_Type(const UmlTypeSpec & t) {
  return set_it_(_type, t, setTypeCmd);
}

const Q3CString & UmlBaseAttribute::multiplicity() {
  read_if_needed_();
  
  return _multiplicity;
}

bool UmlBaseAttribute::set_Multiplicity(const char * s) {
  return set_it_(_multiplicity, s, setMultiplicityCmd);
}

UmlOperation * UmlBaseAttribute::getOperation() {
  read_if_needed_();
  
  return _get_oper;
}

bool UmlBaseAttribute::addGetOperation() {
  UmlCom::send_cmd(_identifier, addGetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

UmlOperation * UmlBaseAttribute::setOperation() {
  read_if_needed_();
  
  return _set_oper;
}

bool UmlBaseAttribute::addSetOperation() {
  UmlCom::send_cmd(_identifier, addSetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

#ifdef WITHCPP
bool UmlBaseAttribute::isCppMutable() {
  read_if_needed_();
  
  return _cpp_mutable;
}

bool UmlBaseAttribute::set_isCppMutable(bool y) {
  UmlCom::send_cmd(_identifier, setIsCppMutableCmd, (char) y);
  if (UmlCom::read_bool()) {
    _cpp_mutable = y;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHJAVA
bool UmlBaseAttribute::isJavaTransient() {
  read_if_needed_();
  
  return _java_transient;
}

bool UmlBaseAttribute::set_isJavaTransient(bool y) {
  UmlCom::send_cmd(_identifier, setIsJavaTransientCmd, (char) y);
  if (UmlCom::read_bool()) {
    _java_transient = y;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHIDL
Q3CString UmlBaseAttribute::idlCase() {
  read_if_needed_();
  
  return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseAttribute::set_IdlCase(UmlAttribute * a) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");
  if (UmlCom::read_bool()) {
    _idl_case = a;
    _idl_explicit_case = 0;
    return TRUE;
  }
 else
   return FALSE;
}

bool UmlBaseAttribute::set_IdlCase(const char * s) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, (void *) 0, s);
  if (UmlCom::read_bool()) {
    _idl_case = 0;
    _idl_explicit_case = s;
    return TRUE;
  }
 else
   return FALSE;
}
#endif

void UmlBaseAttribute::unload(bool rec, bool del) {
  _type.explicit_type = 0;
  _default_value = 0;
#ifdef WITHIDL
  _idl_explicit_case = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
  _multiplicity = 0;
}

void UmlBaseAttribute::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _type.type = (UmlClass *) UmlBaseItem::read_();
  if (_type.type == 0)
    _type.explicit_type = UmlCom::read_string();
  _multiplicity = UmlCom::read_string();
  _default_value = UmlCom::read_string();
  _read_only = UmlCom::read_bool();
  _derived = UmlCom::read_bool();
  _derived_union = UmlCom::read_bool();
  _ordered = UmlCom::read_bool();
  _unique = UmlCom::read_bool();
  _get_oper = (UmlOperation *) UmlBaseItem::read_();
  _set_oper = (UmlOperation *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseAttribute::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_mutable = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseAttribute::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHPHP
void UmlBaseAttribute::read_php_() {
  UmlBaseClassMember::read_php_();
}
#endif

#ifdef WITHPYTHON
void UmlBaseAttribute::read_python_() {
  UmlBaseClassMember::read_python_();
}
#endif

#ifdef WITHIDL
void UmlBaseAttribute::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _idl_case = (UmlAttribute *) UmlBaseItem::read_();
  if (_idl_case == 0)
    _idl_explicit_case = UmlCom::read_string();
}
#endif

