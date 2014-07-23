
#include "UmlCom.h"
#include "UmlBaseState.h"
#include "UmlState.h"
#include "UmlOperation.h"
#include "UmlStateDiagram.h"
//Added by qt3to4:
#include <Q3CString>

UmlState * UmlBaseState::create(UmlItem * parent, const char * s)
{
  return (UmlState *) parent->create_(aState, s);
}

anItemKind UmlBaseState::kind() {
  return aState;
}

const Q3CString & UmlBaseState::entryBehavior() {
  read_if_needed_();
  return _uml.on_entry;
}

bool UmlBaseState::set_EntryBehavior(const char * s) {
  return set_it_(_uml.on_entry, s, setUmlEntryBehaviorCmd);
}

const Q3CString & UmlBaseState::exitBehavior() {
  read_if_needed_();
  return _uml.on_exit;
}

bool UmlBaseState::set_ExitBehavior(const char * s) {
  return set_it_(_uml.on_exit, s, setUmlExitBehaviorCmd);
}

const Q3CString & UmlBaseState::doActivity() {
  read_if_needed_();
  return _uml.do_activity;
}

bool UmlBaseState::set_DoActivity(const char * s) {
  return set_it_(_uml.do_activity, s, setUmlActivityCmd);
}

#ifdef WITHCPP
const Q3CString & UmlBaseState::cppEntryBehavior() {
  read_if_needed_();
  return _cpp.on_entry;
}

bool UmlBaseState::set_CppEntryBehavior(const char * s) {
  return set_it_(_cpp.on_entry, s, setCppEntryBehaviorCmd);
}

const Q3CString & UmlBaseState::cppExitBehavior() {
  read_if_needed_();
  return _cpp.on_exit;
}

bool UmlBaseState::set_CppExitBehavior(const char * s) {
  return set_it_(_cpp.on_exit, s, setCppExitBehaviorCmd);
}

const Q3CString & UmlBaseState::cppDoActivity() {
  read_if_needed_();
  return _cpp.do_activity;
}

bool UmlBaseState::set_CppDoActivity(const char * s) {
  return set_it_(_cpp.do_activity, s, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const Q3CString & UmlBaseState::javaEntryBehavior() {
  read_if_needed_();
  return _java.on_entry;
}

bool UmlBaseState::set_JavaEntryBehavior(const char * s) {
  return set_it_(_java.on_entry, s, setJavaEntryBehaviorCmd);
}

const Q3CString & UmlBaseState::javaExitBehavior() {
  read_if_needed_();
  return _java.on_exit;
}

bool UmlBaseState::set_JavaExitBehavior(const char * s) {
  return set_it_(_java.on_exit, s, setJavaExitBehaviorCmd);
}

const Q3CString & UmlBaseState::javaDoActivity() {
  read_if_needed_();
  return _java.do_activity;
}

bool UmlBaseState::set_JavaDoActivity(const char * s) {
  return set_it_(_java.do_activity, s, setJavaActivityCmd);
}
#endif

UmlOperation * UmlBaseState::specification() {
  read_if_needed_();
  return _specification;
}

bool UmlBaseState::set_Specification(UmlOperation * v) {
  UmlCom::send_cmd(_identifier, setDefCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_bool()) {
    _specification = v;
    return TRUE;
  }
  else
    return FALSE;
}

UmlStateDiagram * UmlBaseState::associatedDiagram() {
  read_if_needed_();

  return _assoc_diagram;
}

bool UmlBaseState::set_AssociatedDiagram(UmlStateDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseState::unload(bool rec, bool del) {
  _uml.unload();
#ifdef WITHCPP
  _cpp.unload();
#endif
#ifdef WITHJAVA
  _java.unload();
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBaseState::read_uml_() {
  _assoc_diagram = (UmlStateDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  _uml.read();
  _specification = (UmlOperation *) UmlBaseItem::read_();
  (void) UmlCom::read_bool(); // active
  (void) UmlBaseItem::read_();	// reference
}

#ifdef WITHCPP
void UmlBaseState::read_cpp_() {
  _cpp.read();
}
#endif

#ifdef WITHJAVA
void UmlBaseState::read_java_() {
  _java.read();
}
#endif

