
#include "UmlEntryPointPseudoState.h"

#include "UmlCom.h"
#include "FileIn.h"
//Added by qt3to4:
#include <Q3CString>
void UmlEntryPointPseudoState::solve(Q3CString idref) {
  // note : parent state already resolved => is a state reference
  QMap<Q3CString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("state : unknown entry point reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anEntryPointPseudoState)
    set_Reference((UmlEntryPointPseudoState *) *it);


}

