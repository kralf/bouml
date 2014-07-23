
#include "ClassInstance.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlClassInstance.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
//Added by qt3to4:
#include <Q3CString>
#include <Q3ValueList>
void ClassInstance::init()
{
  UmlItem::declareFct("ownedmember", "uml:InstanceSpecification", &importIt);
  UmlItem::declareFct("packagedelement", "uml:InstanceSpecification", &importIt);
}

void ClassInstance::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aClassInstance, token, in);	// can't be null
  
  ClassInstance * cli = new ClassInstance;

  cli->id = token.xmiId();
  cli->name = token.valueOf("name");
  cli->classifierId = token.valueOf("classifier");
  cli->where = where;
    
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if (s == "classifier")
	cli->classifierId = token.xmiIdref();
      else if (s == "slot") {
	cli->bindings.append(Slot());
	
	cli->bindings.last().importIt(in, token);
      }
      else if (! token.closed())
	in.finish(s);
    }
  }

  if (cli->classifierId.isEmpty()) {
    in.warning("classifier missing for class instance '" + cli->id + "', bypass it");
    delete cli;
  }
  else
    All.append(cli);

}

void ClassInstance::solveThem()
{
  // first create instances because an instance may be referenced by
  // an other to set the value of a relation

  ClassInstance * cli;
  Q3PtrList<UmlClassInstance> insts;
  
  for (cli = All.first(); cli != 0; cli = All.next()) {
    QMap<Q3CString, UmlItem *>::Iterator it;
    
    it = UmlItem::All.find(cli->classifierId);
    if (it == UmlItem::All.end()) {
      if (!FileIn::isBypassedId(cli->classifierId))
	UmlCom::trace("class instance : unknown classifier reference '" +
		      cli->classifierId + "'<br>");
    }
    else if ((*it)->kind() != aClass)
      UmlCom::trace("class instance : classifier '" + cli->classifierId +
		    "' is not a class<br>");
    else {
      UmlClassInstance * inst = 
	UmlBaseClassInstance::create(cli->where, cli->name, (UmlClass *) *it);

      if (inst == 0)
	UmlCom::trace("cannot create class instance '" + cli->id + "'<br>");
      else {
	UmlItem::All.insert(cli->id, inst);
      }
    }
  }

  // set attribute ans relation values

  while (! All.isEmpty()) {
    QMap<Q3CString, UmlItem *>::Iterator it;
    
    cli = All.take(0);
    
    it = UmlItem::All.find(cli->id);
    if (it != UmlItem::All.end()) {
      // class instance ok
      UmlClassInstance * inst = (UmlClassInstance *) *it;
      Q3ValueList<Slot> & l = cli->bindings;
      Q3ValueList<Slot>::Iterator itsl;
      
      for (itsl = l.begin(); itsl != l.end(); ++itsl) {
	Slot & slot = *itsl;
	
	it = UmlItem::All.find(slot.featureId);
	if (it == UmlItem::All.end()) {
	  if (!FileIn::isBypassedId(slot.featureId))
	    UmlCom::trace("class instance reference '" + cli->id +
			  "' : unknown slot feature reference '" +
			  slot.featureId + "'<br>");
	}
	else if ((*it)->kind() == anAttribute) {
	  if (! inst->set_AttributeValue((UmlAttribute *) *it, slot.value))
	    UmlCom::trace("class instance reference '" + cli->id +
			  "' doesn't have attribute '" + (*it)->name() + "'<br>");
	}
	else if ((*it)->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) *it;
	  
	  it = UmlItem::All.find(slot.valueId);
	  if (it == UmlItem::All.end()) {
	    if (!FileIn::isBypassedId(slot.valueId))
	      UmlCom::trace("class instance reference '" + cli->id +
			    "': unknown class instance reference '" +
			    slot.valueId + "'<br>");
	  }
	  else if ((*it)->kind() != aClassInstance)
	    UmlCom::trace("class instance reference '" + cli->id +
			  "': value reference '" + slot.valueId + 
			  "' is not a class instance<br>");
	  else if (! inst->add_Relation(rel, (UmlClassInstance *) *it))
	    UmlCom::trace("class instance reference '" + cli->id + 
			  "' : cannot set value for relation reference '" +
			  slot.featureId + "'<br>");
	}
	else 
	  UmlCom::trace("class instance : slot feature reference '" +
			slot.featureId +
			"' is not an attribute nor a relation<br>");
      }
    }
    
    delete cli;
  }
}

Q3PtrList<ClassInstance> ClassInstance::All;

void ClassInstance::Slot::importIt(FileIn & in, Token & token) {
  featureId = token.valueOf("definingfeature");
  valueId = token.valueOf("value");
    
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if (s == "featureid")
	featureId = token.xmiIdref();
      else if (s == "value") {
	value = token.valueOf("value");
	if (value.isEmpty())
	  value = token.valueOf("body");
      }
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (featureId.isEmpty())
    in.warning("'definingFeature' is missing");

}

