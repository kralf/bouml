
#include "UmlClass.h"

#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlOperation.h"
#include "UmlRelation.h"
//Added by qt3to4:
#include <Q3CString>
void UmlClass::singleton() {
  // checking
  Q3PtrVector<UmlItem> ch = children();
  int index;
  bool ok = TRUE;
  
  for (index = 0; index != (int) ch.size(); index += 1)
    if (ch[index]->kind() != aNcRelation)
      ok &= ((UmlClassItem *) ch[index])->check();

  if (! ok)
    return;
  
  // the private constructor
  UmlOperation * op = UmlBaseOperation::create(this, name());
  
  if (op == 0)
    UmlCom::trace(Q3CString("<font face=helvetica><b>cannot create constructor for <i>")
		  + name() + "</i></b></font><br><hr><br>");
  else {
    op->set_Visibility(PrivateVisibility);
    
    // the 'instance()' operation
    
    op = UmlBaseOperation::create(this, "instance");
    
    if (op == 0)
      UmlCom::trace(Q3CString("<font face=helvetica><b>cannot create operation <i>instance</i> for <i>")
		    + name() + "</i></b></font><br><hr><br>");
    else {
      op->set_Visibility(PublicVisibility);
      op->set_isClassMember(TRUE);
      
      UmlTypeSpec t;
      Q3CString s;
      
      t.type = this;
      op->set_ReturnType(t);
      
      // Cpp, try to reuse default declaration & definition
      
      s = CppSettings::operationDecl();
      if ((index = s.find("${type}")) != -1) {
	s.insert(index + 7, " *");
	op->set_CppDecl(s);
      }
      else {
	UmlCom::trace("<font face=helvetica><b>strange default cpp operation declaration : no <i>${type}</i></b></font><br><hr><br>");
	op->set_CppDecl("${comment}static ${type} * ${name}${(}${)};");
      }
      
      s = CppSettings::operationDef();
      if (((s.find("${body}")) != -1) && ((index = s.find("${type}")) != -1))
	s.insert(index + 7, " *");
      else {
	UmlCom::trace("<font face=helvetica><b>strange default cpp operation definition: no <i>${type}</i> or <i>${body}</i></b></font><br><hr><br>");
	s = "${inline}${type} * ${class}::${name}${(}${)}${const}${volatile}${staticnl}{\n${body}}\n";
      }
      s.replace(s.findRev("${body}"),	// cannot be -1 !
		7,
		"  return (the == 0) ? the = new ${type}() : the;\n");
      op->set_CppDef(s);
      
      // Java, try to reuse default definition
      
      s = JavaSettings::operationDef();
      if ((index = s.findRev("${body}")) != -1)
	op->set_JavaDecl(s.replace(index, 7,
				   "  return (the == null) ? the = new ${type}() : the;\n"));
      else {
	UmlCom::trace("<font face=helvetica><b>strange default java operation definition: no <i>${body}</i></b></font><br><hr><br>");
	op->set_JavaDecl("  ${comment}${final}static ${type} ${name}${(}${)}${throws}\n\
{\n\
return (the == null) ? the = new ${type}() : the;\n\
}\n");
      }
      
      op->set_isJavaFinal(TRUE);
      
      // change the stereotype before the relation is add
      // to set the canvas appropriate width, else the relation
      // may be hidden
      bool stereotype_changed;
      
      if ((stereotype_changed = stereotype().isEmpty()) != 0)
	set_Stereotype("singleton");
      
      // the relation to the unique instance (cannot have a static variable
      // to memorise it in the instance() operation in Java). class relation, private
      
      UmlRelation * rel =
	UmlBaseRelation::create(aDirectionalAssociation, this, this);
      
      if (rel == 0) {
	UmlCom::trace(Q3CString("<font face=helvetica><b>cannot create relation <i>the</i> for <i>")
		      + name() + "</i></b></font><br><hr><br>");
	if (stereotype_changed)
	  set_Stereotype("");
      }
      else {
	rel->set_Visibility(PrivateVisibility);
	rel->set_isClassMember(TRUE);
	rel->set_RoleName("the");
	
	rel->set_CppDecl(CppSettings::relationDecl(FALSE, "1"));
	rel->set_JavaDecl(JavaSettings::relationDecl("1"));
      }
    }
  }
}

