
#include "UmlState.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlCom.h"
#include "UmlRegion.h"
#include "UmlPseudoState.h"
#include "UmlFinalState.h"
#include "UmlTransition.h"
//Added by qt3to4:
#include <Q3CString>

UmlItem * UmlState::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aState:
  case aRegion:
  case anInitialPseudoState:
  case anEntryPointPseudoState:
  case aFinalState:
  case aTerminatePseudoState:
  case anExitPointPseudoState:
  case aDeepHistoryPseudoState:
  case aShallowHistoryPseudoState:
  case aJunctionPseudoState:
  case aChoicePseudoState:
  case aForkPseudoState:
  case aJoinPseudoState:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

void UmlState::importActivity(FileIn & in, Token & token) {
  Q3CString k = token.what();
  const char * kstr = k;
  Q3CString b = token.valueOf("body");
  
  if (b.isEmpty()) {
    if (! token.closed()) {
      while (in.read(), !token.close(kstr)) {
	Q3CString s = token.what();
    
	if (s == "body") {
	  b = in.body("body");
	  in.finish(k);
	  break;
	}
	else if ((s == "node") &&
		 (token.xmiType() == "uml:CallOperationAction")) {
	  s = token.valueOf("operation");
	  if (! s.isEmpty()) {
	    if (k == "entry")
	      UnresolvedWithContext::add(this, s, 0);
	    else if (k == "exit")
	      UnresolvedWithContext::add(this, s, 1);
	    else
	      UnresolvedWithContext::add(this, s, 2);
	  }
	  if (! token.closed())
	    in.finish("node");
	  in.finish(k);
	  return;
	}
	else if (! token.closed())
	  in.finish(s);
      }
    }
  }
  else if (! token.closed())
    in.finish(k);
  
  if (! b.isEmpty()) {
    if (k == "entry")
      set_EntryBehavior(b);
    else if (k == "exit")
      set_ExitBehavior(b);
    else
      set_DoActivity(b);
  }

}

void UmlState::solve(int context, Q3CString idref) {
  QMap<Q3CString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("state : unknown reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anOperation) {
    if (context == 3)
      set_Specification((UmlOperation *) *it);
    else {
      Q3CString b = (*it)->name() + "()";
      
      switch(context) {
      case 0:
	set_EntryBehavior(b);
	break;
      case 1:
	set_ExitBehavior(b);
	break;
      default:
	set_DoActivity(b);
	break;
      }
    }
  }
  else if (((*it)->kind() == aState) && (context == 4))
    set_Reference((UmlState *) *it);
}

void UmlState::init()
{
  declareFct("ownedmember", "uml:StateMachine", &importIt);
  declareFct("packagedelement", "uml:StateMachine", &importIt);
  declareFct("subvertex", "uml:State", &importIt);
  declareFct("ownedstatemachine", "", &importIt); // andromda emf
  declareFct("ownedbehavior", "uml:StateMachine", &importIt); // magic draw

  UmlRegion::init();
  UmlPseudoState::init();
  UmlFinalState::init();
  UmlTransition::init();
}

void UmlState::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aState, token, in);
    
  if (where != 0) {
    bool machine = ((token.xmiType() == "uml:StateMachine") ||
		    (token.valueOf("issubmachinestate") == "true") ||
		    (token.what() == "ownedstatemachine")); // andromda emf
    Q3CString s = token.valueOf("name");
    
    if (s.isEmpty()) {
      static unsigned n = 0;
      
      s.sprintf((machine) ? "anonymous_state_machine_%u" 
			  : "anonymous_state_%u",
		++n);
    }
    
    UmlState * st = create(where, s);
    
    if (st == 0)
      in.error((machine) ? "cannot create state machine '"
			 : "cannot create state '"
	       + s + "' in '" + where->name() + "'");
    
    st->addItem(token.xmiId(), in);

    if (token.valueOf("isactive") == "true")
      st->set_isActive(TRUE);
    
    Q3CString ref = token.valueOf("submachine");
    Q3CString spec = token.valueOf("specification");
    
    if (! token.closed()) {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if ((s == "entry") || (s == "doactivity") || (s == "exit"))
	  st->importActivity(in, token);
	else if (s == "specification") {
	  spec = token.xmiIdref();
	  if (! token.closed())
	    in.finish(s);
	}
	else
	  st->UmlItem::import(in, token);
      }
    }
    
    if (machine)
      st->set_Stereotype("machine");
    
    if (! ref.isEmpty()) {
      QMap<Q3CString, UmlItem *>::Iterator it = All.find(ref);
      
      if (it == All.end())
	UnresolvedWithContext::add(st, ref, 4);
      else if ((*it)->kind() == aState)
	st->set_Reference((UmlState *) *it);
    }
    
    if (! spec.isEmpty()) {
      QMap<Q3CString, UmlItem *>::Iterator it = All.find(spec);
      
      if (it == All.end())
	UnresolvedWithContext::add(st, spec, 3);
      else if ((*it)->kind() == anOperation)
	st->set_Specification((UmlOperation *) *it);
    }

    st->unload(TRUE, FALSE);
  }
}

int UmlState::NumberOf;

