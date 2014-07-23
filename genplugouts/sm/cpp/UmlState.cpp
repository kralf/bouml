
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "CppSettings.h"
#include "UmlRegion.h"
#include "UmlAttribute.h"
//Added by qt3to4:
#include <Q3CString>

bool UmlState::isLeaf() {
  const Q3PtrVector<UmlItem> ch = children();
  unsigned index;
  
  for (index = 0; index != ch.count(); index += 1) {
    switch (ch[index]->kind()) {
    case aState:
    case aRegion:
    case anInitialPseudoState:
      return FALSE;
    default:
      break;
    }
  }

  return TRUE;
}

void UmlState::hasInitial() {
  if (_has_initial) {
    UmlCom::trace("Error : '" + name() + "' has several 'initial' pseudo state<br>");
    throw 0;
  }

  _has_initial = TRUE;
}

bool UmlState::inside(UmlState * other) {
  UmlItem * p = this;
  
  for (;;) {
    if (p == other)
      return TRUE;
    else if ((p = p->parent())->kind() == aClassView)
      return FALSE;
  }
}

void UmlState::init(UmlClass * mother, Q3CString path, Q3CString pretty_path, UmlState *) {
  // create if needed the class implementing the state

  Q3CString qn = quotedName() + "_State";
    
  if ((_class = (UmlClass*) mother->getChild(aClass, qn)) == 0) {
    if ((_class = UmlBaseClass::create(mother, qn)) == 0) {
      UmlCom::trace("Error : can't create nested class '" + qn
		    + "' under '" + mother->name() + "'<br>");
      throw 0;
    }
  }
  // place the class at the beginning
  const Q3PtrVector<UmlItem> v = mother->children();

  _class->moveAfter(((v.count() == 0) || (v[0]->kind() != aClass))
		    ? (UmlItem *) 0
		    : v[0]);
  
  _class->defaultDef();
  _class->setComment("implement the state " + name());
  _class->set_Visibility((mother->parent()->kind() == aClass)
			 ? PublicVisibility // for friend
			 : ProtectedVisibility);
  
  _class->addDestructor();

  // create if needed the attribute memorizing the class instance
  // implementing the state

  qn = "_" + qn.lower();		// _xyz_state
  
  UmlAttribute * var;
  
  if (((var = (UmlAttribute *) mother->getChild(anAttribute, qn)) == 0) &&
      ((var = UmlBaseAttribute::create(mother, qn)) == 0)) {
    UmlCom::trace("Error : can't create atttibute '" + qn
		  + "' under '" + mother->name() + "'<br>");
    throw 0;
  }
  var->setUsed();

  UmlTypeSpec t;
  
  t.type = _class;
  var->set_Type(t);
  var->set_CppDecl(CppSettings::attributeDecl(0));
  var->setComment("memorize the instance of the state " + name() + ", internal");
  var->set_Visibility((path.isEmpty()) ? ProtectedVisibility : PublicVisibility);
  
  _path = path + "." + qn;
  _pretty_path = pretty_path + "." + name();
	
  // goes down

  const Q3PtrVector<UmlItem> ch = children();
  unsigned index;
  
  for (index = 0; index != ch.count(); index += 1)
    ch[index]->init(_class, _path, _pretty_path, this);

}

bool UmlState::needCreate() {
  return (_has_completion ||
	  !cppEntryBehavior().isEmpty() ||
	  !isLeaf());

}

void UmlState::generate() {
  switch (parent()->kind()) {
  case anUseCase:
  case anUseCaseView:
    UmlCom::trace("Error : generation done only for state machines under class views<br>");
    break;
  default:
    UmlCom::trace("Error : must be applied on a state machine<br>");
    break;
  case aClassView:
    {
      // a class having the normalized name of the state machine 
      // implements it in the class view of the state machine
      Q3CString qn = quotedName();
      UmlClass * machine = (UmlClass *) parent()->getChild(aClass, qn);
      
      if ((machine == 0) &&
	  ((machine = UmlBaseClass::create(parent(), qn)) == 0)) {
	UmlCom::trace("Error : can't create class '" + qn
		      + "' in class view '"
		      + parent()->name() + "'<br>");
	return;
      }
      machine->setUseless();
      machine->defaultDef();
      machine->setComment("implement the state machine " + name());
      
      // a class named "AnyState" is a super class of all the classes
      // representing sub states
      // it is defined a the first position under machine
      UmlClass * anystate = (UmlClass *) machine->getChild(aClass, "AnyState");
      
      if (anystate == 0) {
	if ((anystate = UmlBaseClass::create(machine, "AnyState")) == 0) {
	  UmlCom::trace("Error : can't create nested class 'AnyState' in '"
			+ qn + "'<br>");
	  return;
	}
	
	anystate->moveAfter(0);
      }
      anystate->defaultDef();
      anystate->setComment("Mother class of all the classes representing states");
      anystate->set_Visibility(ProtectedVisibility);
      anystate->addDestructor();
      
      // add abstract operation _upper
      UmlOperation * upper;
      
      if (((upper = (UmlOperation *) anystate->getChild(anOperation, "_upper")) == 0) &&
	  ((upper = UmlBaseOperation::create(anystate, "_upper")) == 0)) {
	UmlCom::trace("Error : cannot create operation _upper in class '"
		      + anystate->name() + "'<br>");
	throw 0;
      }
      upper->set_isCppVirtual(TRUE);
      upper->set_isAbstract(TRUE);
      upper->defaultDef();
      upper->setComment("return the upper state");
      upper->setType(anystate, "${type} *");
      if (upper->params().isEmpty())
	upper->addParam(0, InputOutputDirection, "stm", machine);
      upper->setParams("${t0} &");
      
      // add virtual operation _do
      UmlOperation * da;
      
      if (((da = (UmlOperation *) anystate->getChild(anOperation, "_do")) == 0) &&
	  ((da = UmlBaseOperation::create(anystate, "_do")) == 0)) {
	UmlCom::trace("Error : cannot create operation _do in class '"
		      + anystate->name() + "'<br>");
	throw 0;
      }
      da->defaultDef();
      da->setComment("perform the 'do activity'");
      da->setType("void", "${type}");
      if (da->params().isEmpty())
	da->addParam(0, InputOutputDirection, "stm", machine);
      da->setParams("${t0} &");
      da->set_isCppVirtual(TRUE);
      
      // add if needed classes associated to each state, and set flags
      init(machine, "", "", this);
      
      // add constructor if needed
      UmlOperation * machine_constr = (UmlOperation *)
	machine->getChild(anOperation, machine->name());
      
      if (machine_constr == 0) {
	if ((machine_constr =
	     UmlBaseOperation::create(machine, machine->name()))
	    == 0) {
	  UmlCom::trace("Error : can't create constructor in class '"
			+ machine->name() + "'<br>");
	  return;
	}
      }
      machine_constr->defaultDef();
      machine_constr->setType("", "");
      if (machine_constr->cppBody().isEmpty())
	machine_constr->set_CppBody("  _current_state = 0;\n");
      
      machine->addDestructor();
      
      // generate the machine
      generate(machine, anystate, this);
      
      // set create's body
      UmlOperation * cr = (UmlOperation *)
	machine->getChild(anOperation, "create");
      
      if (cr == 0) {
	UmlCom::trace("Error : the state machine must have a transition from 'initial'<br>");
	return;
      }
      if (cr->cppBody().isEmpty())
	cr->set_CppBody("\
if (_current_state == 0)\n\
(_current_state = &(*this)" + path() + ")->create(*this);\n\
return (_current_state != 0);\n");
      
      // add operation doActivity
      UmlOperation * doAct = (UmlOperation *)
	machine->getChild(anOperation, "doActivity");
      
      if (doAct == 0) {
	if ((doAct =
	     UmlBaseOperation::create(machine, "doActivity"))
	    == 0) {
	  UmlCom::trace("Error : can't create operation 'doActivity' in class '"
			+ machine->name() + "'<br>");
	  return;
	}
      }
      doAct->defaultDef();
      doAct->setComment("to execute the current state 'do activity'");
      doAct->setType("void", "${type}");
      doAct->set_CppBody("  _current_state->_do(*this);\n");
      doAct->set_Visibility(PublicVisibility);
      
      // add if needed operation _set_currentState
      UmlOperation * set_currentState = (UmlOperation *)
	machine->getChild(anOperation, "_set_currentState");
      
      if (set_currentState == 0) {
	if ((set_currentState =
	     UmlBaseOperation::create(machine, "_set_currentState"))
	    == 0) {
	  UmlCom::trace("Error : can't create operation '_set_currentState' in class '"
			+ machine->name() + "'<br>");
	  return;
	}
      }
      set_currentState->defaultDef();
      set_currentState->setComment("change the current state, internal");
      set_currentState->setType("void", "${type}");
      if (set_currentState->params().isEmpty())
	set_currentState->addParam(0, InputOutputDirection, "st", anystate);
      set_currentState->setParams("${t0} & ${p0}");
      set_currentState->set_CppBody("  _current_state = &st;\n");
      set_currentState->set_isCppInline(TRUE);
      set_currentState->set_Visibility(ProtectedVisibility);
      
      // add if needed operation _final
      UmlOperation * final = (UmlOperation *)
	machine->getChild(anOperation, "_final");
      
      if (final == 0) {
	if ((final =
	     UmlBaseOperation::create(machine, "_final"))
	    == 0) {
	  UmlCom::trace("Error : can't create operation '_final' in class '"
			+ machine->name() + "'<br>");
	  return;
	}
      }
      final->defaultDef();
      final->setComment("execution done, internal");
      final->setType("void", "${type}");
      final->set_CppBody("  _current_state = 0;\n"
			 "#ifdef VERBOSE_STATE_MACHINE\n"
			 "  puts(\"DEBUG : final state reached\");\n"
			 "#endif\n");
      if (final->isCppInline())
	// defined by old release
	final->set_isCppInline(FALSE);
      final->set_Visibility(ProtectedVisibility);
      
      // add if needed a relation to memorize the current state
      UmlRelation * current_state = (UmlRelation *)
	machine->getChild(aRelation, "_current_state");
      
      if (current_state == 0) {
	if ((current_state =
	     UmlBaseRelation::create(aDirectionalAggregation, machine, anystate))
	    == 0) {
	  UmlCom::trace("Error : can't create relation '_current_state' from '"
			+ machine->name() + "' to '" + anystate->name() + "'<br>");
	  return;
	}
      } 
      current_state->set_RoleName("_current_state");
      current_state->set_CppDecl(CppSettings::relationDecl(FALSE, ""));
      current_state->setComment("contains the current state, internal");
      current_state->set_Visibility(ProtectedVisibility);
      
      // all done
      machine->deleteUseless();
    }
  }
}

void UmlState::generate(UmlClass * machine, UmlClass * anystate, UmlState *) {
  // inherits anystate
  const Q3PtrVector<UmlItem> clch = _class->children();
  unsigned index;
  
  for (index = 0; index != clch.count(); index += 1)
    if ((clch[index]->kind() == aRelation) &&
	(((UmlRelation *) clch[index])->relationKind() == aGeneralisation) &&
	(((UmlRelation *) clch[index])->roleType() == anystate))
      break;
    
  if (index == clch.count()) {
    UmlRelation * inh = UmlRelation::create(aGeneralisation, _class, anystate);
  
    if (inh == 0) {
      UmlCom::trace("Error : '" + _class->name() + "'cannot inherits '"
		    + anystate->name() + "'<br>");
      throw 0;
    }
    inh->set_CppDecl("${type}");
  }

  // generate children

  const Q3PtrVector<UmlItem> ch = children();
  
  for (index = 0; index != ch.count(); index += 1)
    ch[index]->generate(machine, anystate, this);
    
  // additional operations
    
  Q3CString s;
    
  s = cppEntryBehavior();
    
  if (isLeaf() && // done by the transition from initial pseudo state for the non leaf
      (_has_completion || !s.isEmpty())) {
    // add a 'create' to do the entry behavior / completion
    UmlOperation * create = _class->trigger("create", machine, anystate);
    Q3CString body;
    
    if (!s.isEmpty())
      body = "\t_doentry(stm);\n";
    if (_has_completion)
      body += "\t_completion(stm);\n";
    
    create->set_CppBody(body);
  }

  if (!s.isEmpty()) {
    // add operation _doentry
    UmlOperation * doen;
    
    if (((doen = (UmlOperation *) _class->getChild(anOperation, "_doentry")) == 0) &&
	((doen = UmlBaseOperation::create(_class, "_doentry")) == 0)) {
      UmlCom::trace("Error : cannot create operation _doentry in class '"
		    + _class->name() + "'<br>");
      throw 0;
    }
   
    doen->defaultDef();
    if (doen->params().isEmpty())
      doen->addParam(0, InputOutputDirection, "stm", machine);
    doen->setParams("${t0} & ${p0}");
    doen->setComment("perform the 'entry behavior'");
    doen->setType("void", "${type}");
    s.insert(0,
	     (const char *)("#ifdef VERBOSE_STATE_MACHINE\n" //[rageek] ambiguous
	     "\tputs(\"DEBUG : execute entry behavior of " + _pretty_path + "\");\n"
	     "#endif\n"));
    doen->set_CppBody(s);
  }

  s = cppExitBehavior();
  
  if (!s.isEmpty()) {
    // add operation _doexit
    UmlOperation * doex;
    
    if (((doex = (UmlOperation *) _class->getChild(anOperation, "_doexit")) == 0) &&
	((doex = UmlBaseOperation::create(_class, "_doexit")) == 0)) {
      UmlCom::trace("Error : cannot create operation _doexit in class '"
		    + _class->name() + "'<br>");
      throw 0;
    }
   
    doex->defaultDef();
    if (doex->params().isEmpty())
      doex->addParam(0, InputOutputDirection, "stm", machine);
    doex->setParams("${t0} & ${p0}");
    doex->setComment("perform the 'exit behavior'");
    doex->setType("void", "${type}");
    s.insert(0, 
	     (const char *)("#ifdef VERBOSE_STATE_MACHINE\n" //[rageek] ambiguous
	     "\tputs(\"DEBUG : execute exit behavior of " + _pretty_path + "\");\n"
	     "#endif\n"));
    doex->set_CppBody(s);
  }

  s = cppDoActivity();
  
  if (!s.isEmpty()) {
    // add operation _do
    UmlOperation * da;
    
    if (((da = (UmlOperation *) _class->getChild(anOperation, "_do")) == 0) &&
	((da = UmlBaseOperation::create(_class, "_do")) == 0)) {
      UmlCom::trace("Error : cannot create operation _do in class '"
		    + _class->name() + "'<br>");
      throw 0;
    }
    
    da->defaultDef();
    if (da->params().isEmpty())
      da->addParam(0, InputOutputDirection, "stm", machine);
    da->setParams("${t0} & ${p0}");
    da->setComment("perform the 'do activity'");
    da->setType("void", "${type}");
    s.insert(0, 
	     (const char *)("#ifdef VERBOSE_STATE_MACHINE\n" //[rageek] ambiguous
	     "\tputs(\"DEBUG : execute do behavior of " + _pretty_path + "\");\n"
	     "#endif\n"));
    da->set_CppBody(s);
    da->set_isCppVirtual(TRUE);
  }

  // adds operation _upper()
  UmlOperation * upper;
  
  if (((upper = (UmlOperation *) _class->getChild(anOperation, "_upper")) == 0) &&
      ((upper = UmlBaseOperation::create(_class, "_upper")) == 0)) {
    UmlCom::trace("Error : cannot create operation _upper in class '"
		  + _class->name() + "'<br>");
    throw 0;
  }
  upper->defaultDef();
  upper->setComment("returns the state containing the current");
  upper->set_isCppVirtual(TRUE);
  upper->setType(anystate, "${type} *");
  upper->addParam(0, InputOutputDirection, "stm", machine);
  
  int dot = _path.findRev('.');
  
  if (dot == 0) {
    upper->setParams("${t0} &");
    upper->set_CppBody("  return 0;\n");
  }
  else {
    upper->setParams("${t0} & ${p0}");
    upper->set_CppBody("  return &stm" + _path.left(dot) + ";\n");
  }

  // adds friend declaration
  const Q3PtrVector<UmlItem> stmch = machine->children();
  
  for (index = 0; index != stmch.count(); index += 1) {
    if ((stmch[index]->kind() == aRelation) &&
	(((UmlRelation *) stmch[index])->relationKind() == aDependency) &&
	(((UmlRelation *) stmch[index])->roleType() == _class))
      // already exist
      return;
  }

  UmlRelation * fr = UmlRelation::create(aDependency, machine, _class);
  
  if (fr == 0) {
    UmlCom::trace("Error : cannot add friend dependency to '" + _class->name() + "'<br>");
    throw 0;
  }
  fr->set_Stereotype("friend");
}

