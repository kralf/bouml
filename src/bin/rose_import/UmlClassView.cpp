
#include "UmlClassView.h"
#include "File.h"
#include "UmlUseCaseView.h"

#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlPackage.h"
#include "UmlUseCase.h"
#include "UmlRelation.h"
#include "UmlClassDiagram.h"
#include "UmlUseCaseDiagram.h"
#include "UmlSequenceDiagram.h"
#include "UmlCollaborationDiagram.h"
//Added by qt3to4:
#include <Q3CString>
UmlClassView::UmlClassView(void * id, const Q3CString & n)  : UmlBaseClassView(id, n){
 ucv = 0;
 cpt[kind()] += 1;

}

void UmlClassView::import(UmlPackage * parent, File & f)
{
  Q3CString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, " class view's name expected");

  Q3CString a;
  Q3CString id;
  Q3CString ste;
  Q3CString doc;
  Q3Dict<Q3CString> prop;
  
  for (;;) {
    int k = f.readDefinitionBeginning(a, id, ste, doc, prop);

    if (k != ATOM)
      f.syntaxError(a);
    
    if (a == "file_name") {
      if (f.read(a) != STRING)
	f.syntaxError(a, "a filename");
      
      File f2(a, f.name());
      
      if (! f2.open(QIODevice::ReadOnly))
	UmlCom::trace("<br>cannot open '" + a + "' referenced in "
		      + Q3CString(f.name().toAscii()));//[jasa] QString to Q3CString conversion
      else {     
        f2.read("(");
        f2.read("object");
        f2.read("Petal");
        f2.skipBlock();
      
        f2.read("(");
        f2.read("object");
        f2.read("Class_Category");
        import(parent, f2);
      }

      f.skipBlock();
      return;
    }
    else if (!id.isEmpty()) {
      f.unread(k, a);
      
      UmlClassView * clv;
      
      if (scanning) {
	UmlPackage * pack = UmlBasePackage::create(parent, s);
	clv = UmlBaseClassView::create(pack, s);
	
	if (clv != 0)
	  newItem(clv, id);
	
	if (!ste.isEmpty()) {
	  if (ste == "CORBAModule")
	    pack->set_IdlModule(s);
	  else
	    clv->set_Stereotype(ste);
	}
	
	if (!doc.isEmpty())
	  clv->set_Description(doc);
	
	Q3CString * nmsp;
	
	if ((nmsp = prop.find("Cplusplus/IsNamespace")) != 0) {
	  if (*nmsp == "TRUE")
	    pack->set_CppNamespace(s);
	  prop.remove("Cplusplus/IsNamespace");
	}
	
	clv->setProperties(prop);
      }
      else
	clv = (UmlClassView *) findItem(id, aClassView);
      
      if (clv == 0) {
	UmlCom::trace("<br>cannot create class view named '" +
		      s + "' in '" + parent->fullName() + "'");
	throw 0;
      }
      
      clv->import(f);
      return;
    }
    else
      f.skipNextForm();
  }
}

void UmlClassView::import(File & f) {
  for (;;) {
    Q3CString s;

    switch (f.read(s)) {
    case -1:
      f.eof();
      throw 0;
    case ')':
      return;
    case ATOM:
      if ((s == "logical_models") || 
          (s == "logical_presentations")) {
        f.read("(");
        f.read("list");
        f.read("unit_reference_list");
        readObjects(f);
      }
      else
        f.skipNextForm();
      break;
    default:
      f.syntaxError(s);
    }
  }
}

void UmlClassView::readObjects(File & f) {
  for (;;) {
    Q3CString s;

    switch (f.read(s)) {
    case ')':
      return;
    case '(':
      break;
    default:
      f.syntaxError(s);
    }

    f.read("object");

    if (f.read(s) != ATOM)
      f.syntaxError(s, "an atom");

    if ((s == "Class") || (s == "Metaclass") ||
	(s == "Instantiated_Class") || (s == "Parameterized_Class"))
      UmlClass::import(f, this, s);
    else if (s == "Association")
      UmlRelation::import(f);
    else if (s == "UseCase")
      UmlUseCase::import(f, useCaseView());
    else if (s == "UseCaseDiagram")
      UmlUseCaseDiagram::import(f, useCaseView());
    else if (s == "ObjectDiagram")
      UmlCollaborationDiagram::import(f, this);
    else if (s == "InteractionDiagram")
      UmlSequenceDiagram::import(f, this);
    else if (s == "ClassDiagram")
      UmlClassDiagram::import(f, this);
    else if (s == "Class_Category") 
      UmlClassView::import((UmlPackage *) parent(), f);
    else if (s == "logical_presentations") {
      f.read("(");
      f.read("list");
      f.read("unit_reference_list");
      readObjects(f);
    }
    else {
      if (s != "Mechanism")
	UmlCom::trace("<br>" + s + " in " + Q3CString(f.name().toAscii()) + " NOT MANAGED by ClassView::readObject()");//[jasa] QString to Q3CString conversion
      f.skipBlock();
    }
  }

}

UmlUseCaseView * UmlClassView::useCaseView() {
  if ((ucv == 0) &&
      ((ucv = UmlBaseUseCaseView::create(parent(), name())) == 0)) {
    UmlCom::trace("<br>cannot create use case view named '" +
		  name() + "' in '" + parent()->fullName() + "'");
    throw 0;
  }

  return ucv;
}

