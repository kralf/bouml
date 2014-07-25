
#include "UmlStateAction.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlTransition.h"
//Added by qt3to4:
#include <Q3CString>

void UmlStateAction::generate(UmlClass * machine, UmlClass * anystate, UmlState * state, Q3CString & body, Q3CString indent) {
  body += indent + cppExpression() + '\n';
  
  const Q3PtrVector<UmlItem> ch = children();
  
  if (ch.count() != 1) {
    UmlCom::trace("Error : 'action' pseudo state must have one and only one transition<br>");
    throw 0;
  }

  UmlTransition * tr = (UmlTransition *) ch[0];
  
  if (tr->target()->parent() != parent()) {
    UmlCom::trace("Error : transition from 'action' must goes to a sub [pseudo] state of the state '"
		  + state->name() + "'<br>");
    throw 0;
  }

  if (!tr->cppGuard().isEmpty()) {
    UmlCom::trace("Error : transition from 'action' must have no guard<br>");
    throw 0;
  }

  tr->generate(machine, anystate, state, body, indent);
}

