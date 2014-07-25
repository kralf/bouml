
#include "UmlItem.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlNcRelation.h"
//Added by qt3to4:
#include <Q3CString>
#include <Q3ValueList>

void Unresolved::addGeneralization(UmlItem * e, Q3CString & id, Q3CString cstr)
{
  Generalizations.append(Unresolved(e, id, cstr));
}

void Unresolved::addRef(UmlItem * e, Q3CString & id)
{
  Refs.append(Unresolved(e, id, ""));
}

Q3ValueList<Unresolved> Unresolved::Generalizations;

Q3ValueList<Unresolved> Unresolved::Refs;

void Unresolved::solveThem()
{
  Q3ValueList<Unresolved>::Iterator it;
      
  for (it = Refs.begin(); it != Refs.end(); ++it)
    (*it).element->solve((*it).idref);
  Refs.clear();
  
  for (it = Generalizations.begin(); it != Generalizations.end(); ++it)
    (*it).element->solveGeneralizationDependencyRealization(0, (*it).idref, "", (*it).constraint);
  Generalizations.clear();
}

void UnresolvedWithContext::add(UmlItem * e, Q3CString id, int c)
{
  All.append(UnresolvedWithContext(e, id, c));
}

void UnresolvedWithContext::solveThem()
{
  Q3ValueList<UnresolvedWithContext>::Iterator it;
      
  for (it = All.begin(); it != All.end(); ++it)
    (*it).element->solve((*it).context, (*it).idref);
    
  All.clear();
}

Q3ValueList<UnresolvedWithContext> UnresolvedWithContext::All;

void UnresolvedRelation::add(int ctx, Q3CString idFrom, Q3CString idTo, Q3CString label, Q3CString constraint)
{
  All.append(UnresolvedRelation(ctx, idFrom, idTo, label, constraint));
}

UnresolvedRelation::UnresolvedRelation() {
}

Q3ValueList<UnresolvedRelation> UnresolvedRelation::All;

void UnresolvedRelation::solveThem()
{
  Q3ValueList<UnresolvedRelation>::Iterator it;
      
  for (it = All.begin(); it != All.end(); ++it) {
    QMap<Q3CString, UmlItem *>::Iterator from = UmlItem::All.find((*it).from);
    
    if (from != UmlItem::All.end())
      (*from)->solveGeneralizationDependencyRealization((*it).context, (*it).to, (*it).name, (*it).constraint);
    else if (!FileIn::isBypassedId((*it).from))
      UmlCom::trace("relation : unknown source reference '" + (*it).from + "'<br>");
  }
  All.clear();
}

 UmlItem::~UmlItem() {
}

void UmlItem::import(QString) {
  UmlCom::trace("Error : must be applied on a package<br>");
}

void UmlItem::addItem(Q3CString id, FileIn & in) {
  QMap<Q3CString, UmlItem *>::ConstIterator iter = All.find(id);
  
  if (iter != All.end())
    in.error("xmi:id '" + id + "' used twice");
    
  All.insert(id, this);
  _xmi_id = id;
}

void UmlItem::import(FileIn & in, Token & tk) {
  Q3CString key = tk.what() + " " + tk.xmiType();
    
  PFunc pf = Functions[key];
  
  if (pf != 0)
    pf(in, tk, this);
  else
    in.bypass(tk);

}

UmlItem * UmlItem::container(anItemKind kind, Token & token, FileIn & in) {
  return parent()->container(kind, token, in);
}

void UmlItem::solve(Q3CString) {
}

void UmlItem::solve(int, Q3CString) {
}

void UmlItem::generalizeDependRealize(UmlItem * target, FileIn & in, int context, Q3CString label, Q3CString) {
  static const struct {
    aRelationKind rk;
    const char * err;
  } r[] = {
    { aGeneralisation, "cannot create generalization from '" },
    { aDependency, "cannot create dependency from '" },
    { aRealization, "cannot create realization from '" },
    { aDependency, "cannot create usage from '" },
    { aDependency, "cannot create import from '" }
  };
  UmlItem * rel = UmlNcRelation::create(r[context].rk, this, target);
  
  if (rel == 0)
    in.warning(r[context].err + name() + "' to '" + target->name() + "'");
  else if (!label.isEmpty())
    rel->set_Name(label);

}

void UmlItem::solveGeneralizationDependencyRealization(int context, Q3CString idref, Q3CString label, Q3CString) {
  QMap<Q3CString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it != All.end()) {
    static const struct {
      aRelationKind rk;
      const char * err;
    } r[] = {
      { aGeneralisation, "cannot create generalization from '" },
      { aDependency, "cannot create dependency from '" },
      { aRealization, "cannot create realization from '" },
      { aDependency, "cannot create usage from '" },
      { aDependency, "cannot create import from '" }
    };
    UmlItem * target = *it;
    UmlItem * rel = UmlNcRelation::create(r[context].rk, this, target);
    
    if (rel == 0)
      UmlCom::trace(r[context].err + name() + "' to '" + target->name() + "'<br>");
    else {
      if (! label.isEmpty())
	rel->set_Name(label);
      switch (context) {
      case 3:
	rel->set_Stereotype("use");
	break;
      case 4:
	rel->set_Stereotype("import");
	break;
      default:
	break;
      }
    }
  }
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("relation : unknown target reference '" + idref + "'<br>");

}

bool UmlItem::setType(Q3CString idref, UmlTypeSpec & type) {
  if (idref.isEmpty())
    return FALSE;
  else if (getType(idref, type))
    return TRUE;
  else {
    Unresolved::addRef(this, idref);
    return FALSE;
  }
}

bool UmlItem::setType(Q3CString idref, int context, UmlTypeSpec & type) {
  if (idref.isEmpty())
    return FALSE;
  else if (getType(idref, type))
    return TRUE;
  else {
    UnresolvedWithContext::add(this, idref, context);
    return FALSE;
  }
}

bool UmlItem::setType(Token & token, UmlTypeSpec & type) {
  Q3CString idref = token.xmiIdref();
  
  if (idref.isEmpty())
    return UmlClass::isPrimitiveType(token, type);
  else if (getType(idref, type))
    return TRUE;
  else {
    Unresolved::addRef(this, idref);
    return FALSE;
  }
}

bool UmlItem::setType(Token & token, int context, UmlTypeSpec & type) {
  Q3CString idref = token.xmiIdref();
  
  if (idref.isEmpty())
    return UmlClass::isPrimitiveType(token, type);
  else if (getType(idref, type))
    return TRUE;
  else {
    UnresolvedWithContext::add(this, idref, context);
    return FALSE;
  }
}

void UmlItem::loadFromProfile() {
  Q3CString id;
    
  if (propertyValue("xmiId", id) && (All.find(id) == All.end()))
    All.insert(id, this);
  
  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned u = 0; u != n; u += 1)
    ch[u]->loadFromProfile();
}

bool UmlItem::getType(Q3CString idref, UmlTypeSpec & type)
{
  QMap<Q3CString, UmlItem *>::Iterator it = All.find(idref);
  
  type.type = 0;
  type.explicit_type = "";
  
  if (it != All.end()) {
    if ((*it)->kind() == aClass) {
      type.type = (UmlClass *) *it;
      return TRUE;
    }
    else
      return FALSE;
  }
  else {
    QMap<Q3CString, UmlTypeSpec>::Iterator itp = PrimitiveTypes.find(idref);
    
    if (itp != PrimitiveTypes.end()) {
      type = *itp;
      return TRUE;
    }
    else
      return FALSE;
  }
}

void UmlItem::declareFct(Q3CString what, Q3CString type, PFunc fct)
{
  Functions[what + " " + type] = fct;
}

PFunc UmlItem::getFct(const Token & tk)
{
  Q3CString key = tk.what() + " " + tk.xmiType();
    
  return Functions[key];
}

Q3CString UmlItem::readComment(FileIn & in, Token & token)
{
  in.bypassedId(token);

  Q3CString doc = token.valueOf("body");

  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      if (token.what() == "body") {
	if (! doc.isEmpty())
	  doc += "\n";
	doc += in.body("body");
      }
      else if (! token.closed())
	in.finish(token.what());
    }
  }

  return doc;

}

void UmlItem::init()
{
  declareFct("xmi:documentation", "", &importDocumentation);

  declareFct("ownedcomment", "uml:Comment", &importComment);
  declareFct("ownedcomment", "", &importComment); // andromda emf
  
  declareFct("xmi:extension", "", &importExtension);

  declareFct("incoming", "", &incoming);
  declareFct("outgoing", "", &outgoing);
  
  declareFct("ownedbehavior", "uml:OpaqueBehavior", &importOpaqueDef);
  declareFct("ownedmember", "uml:OpaqueExpression", &importOpaqueDef);
  declareFct("packagedelement", "uml:OpaqueExpression", &importOpaqueDef);

  declareFct("generalization", "uml:Generalization", &importGeneralization);
  declareFct("generalization", "", &importGeneralization);	// eclipse
  
  declareFct("ownedelement", "uml:Dependency", &importDependency);
  declareFct("ownedmember", "uml:Dependency", &importDependency);
  declareFct("packagedelement", "uml:Dependency", &importDependency);
  
  declareFct("ownedelement", "uml:Usage", &importDependency);
  declareFct("ownedmember", "uml:Usage", &importDependency);
  declareFct("packagedelement", "uml:Usage", &importDependency);
  
  declareFct("ownedelement", "uml:Realization", &importRealization);
  declareFct("ownedmember", "uml:Realization", &importRealization);
  declareFct("packagedelement", "uml:Realization", &importRealization);
  declareFct("interfacerealization", "uml:InterfaceRealization", &importRealization);

}

void UmlItem::importDocumentation(FileIn & in, Token & token, UmlItem *)
{
  Q3CString who = token.valueOf("exporter");

  if (who.isNull())
    who = token.valueOf("xmi:exporter");
    
    if (! who.isNull()) {
      UmlCom::trace("xmi file produced by <b>" + who + "</b><br><br>");
      FromBouml = (who == "Bouml");
    }

  if (! token.closed())
    in.finish(token.what());
}

void UmlItem::importComment(FileIn & in, Token & token, UmlItem * where)
{
  Q3CString doc = readComment(in, token);

  if (! doc.isEmpty())
    where->set_Description(doc);
}

void UmlItem::importExtension(FileIn & in, Token & token, UmlItem * where)
{
  if (! token.closed()) {
    Q3CString s = token.valueOf("extender");
    
    if (s.isNull())
      s = token.valueOf("xmi:extender");
    
    if (s == "Bouml") {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "taggedvalue")
	  // is closed
	  where->set_PropertyValue(token.valueOf("tag"), token.valueOf("value"));
	else if (s == "stereotype")
	  where->set_Stereotype(token.valueOf("name"));

	if (! token.closed())
	  in.finish(token.what());
      }
    }
    else if (s == "Visual Paradigm for UML") {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "appliedstereotype") {
	  s = token.valueOf("xmi:value");
	  
	  if (s.right(3) == "_id") {
	    s = s.mid(s.find("_") + 1);
	    s = s.left(s.length() - 3).lower();
	  }
	  where->set_Stereotype(s);
	}

	if (! token.closed())
	  in.finish(token.what());
      }
    }
    else
      in.finish(token.what());
  }
}

void UmlItem::importOpaqueDef(FileIn & in, Token & token, UmlItem *)
{
  Q3CString id = token.xmiId();
  QMap<Q3CString, Q3CString>::ConstIterator iter = OpaqueDefs.find(id);
  
  if (iter != OpaqueDefs.end())
    in.error("xmi:id '" + id + "' used twice");
    
  Q3CString s = token.valueOf("body");
  
  if (!s.isNull()) {
    OpaqueDefs.insert(id, s);
    
    if (! token.closed())
      in.finish(token.what());
  }
  else if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "body")
	OpaqueDefs.insert(id, in.body("body"));
      else if (! token.closed())
	in.finish(s);
    }
  }
}

void UmlItem::importGeneralization(FileIn & in, Token & token, UmlItem * where)
{
  Q3CString id = token.valueOf("general");
  Q3CString constraint;
  
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if (s == "general") {
	id = token.xmiIdref();
	
	if (id.isEmpty() && !(id = token.valueOf("href")).isEmpty()) {
	  int index = id.find('#');
	  
	  if (index != -1)
	    id = id.mid(index + 1);
	}
      }
      else if (s == "ownedrule") {
	constraint = UmlItem::readConstraint(in, token);
	continue;
      }
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (!id.isEmpty()) {
    QMap<Q3CString, UmlItem *>::ConstIterator iter = All.find(id);
  
    if (iter != All.end())
      where->generalizeDependRealize(*iter, in, 0, "", constraint);
    else
      Unresolved::addGeneralization(where, id, constraint);
  }
  else
    in.warning("'general' is missing");
}

void UmlItem::importDependency(FileIn & in, Token & token, UmlItem * where)
{
  Q3CString client = token.valueOf("client");
  Q3CString supplier = token.valueOf("supplier");
  Q3CString label = token.valueOf("name");
  Q3CString constraint;
  int kind = (token.xmiType() == "uml:Usage") ? 3 : 1;
  
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if (s == "client")
	client = token.xmiIdref();
      else if (s == "supplier")
	supplier = token.xmiIdref();
      else if (s == "ownedrule") {
	constraint = UmlItem::readConstraint(in, token);
	continue;
      }
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (client.isEmpty())
    in.warning("'client' is missing");
  else {
    if (supplier.isEmpty())
      // Borland Together 2006 for Eclipse
      supplier = where->id();

    QMap<Q3CString, UmlItem *>::ConstIterator from = All.find(client);
    QMap<Q3CString, UmlItem *>::ConstIterator to = All.find(supplier);
  
    if ((from != All.end()) && (to != All.end()))
      (*from)->generalizeDependRealize(*to, in, kind, label, constraint);
    else
      UnresolvedRelation::add(kind, client, supplier, label, constraint);
  }
}

void UmlItem::importRealization(FileIn & in, Token & token, UmlItem * where)
{
  Q3CString client = token.valueOf("client");
  Q3CString supplier = token.valueOf("supplier");
  Q3CString label = token.valueOf("name");
  Q3CString constraint;
  
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if (s == "client")
	client = token.xmiIdref();
      else if (s == "supplier")
	supplier = token.xmiIdref();
      else if (s == "ownedrule") {
	constraint = UmlItem::readConstraint(in, token);
	continue;
      }
      
      if (! token.closed())
	in.finish(s);
    }
  }

  if (client.isEmpty())
    in.warning("'client' is missing");
  else {
    if (supplier.isEmpty())
      // Borland Together 2006 for Eclipse
      supplier = where->id();

    QMap<Q3CString, UmlItem *>::ConstIterator from = All.find(client);
    QMap<Q3CString, UmlItem *>::ConstIterator to = All.find(supplier);
  
    if ((from != All.end()) && (to != All.end()))
      (*from)->generalizeDependRealize(*to, in, 2, label, constraint);
    else
      UnresolvedRelation::add(2, client, supplier, label, constraint);
  }
}

void UmlItem::incoming(FileIn & in, Token & token, UmlItem * where)
{
  Incomings.insert(token.xmiIdref(), where);
  
  if (! token.closed())
    in.finish(token.what());
}

void UmlItem::outgoing(FileIn & in, Token & token, UmlItem * where)
{
  while (where->kind() == aRegion)
    // up to the state
    where = where->parent();

  Outgoings.insert(token.xmiIdref(), where);
  
  if (! token.closed())
    in.finish(token.what());
}

Q3CString UmlItem::legalName(Q3CString s)
{
  unsigned index;
  unsigned n = s.length();
  
  for (index = 0; index != n; index += 1) {
    char c = s.at(index);
    
    if (!(((c >= 'a') && (c <= 'z')) ||
	  ((c >= 'A') && (c <= 'Z')) ||
	  ((c >= '0') && (c <= '9')) ||
	  (c == '_')))
      s.replace(index, 1, "_");
  }
  
  return s;
}

bool UmlItem::fromEclipse()
{
  return FromEclipse;
}

Q3CString UmlItem::readConstraint(FileIn & in, Token & token)
{
  Q3CString constraint;
  
  if (! token.closed()) {
    Q3CString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      Q3CString s = token.what();
      
      if ((s == "specification") && (token.xmiType() == "uml:OpaqueExpression")) {
	constraint = token.valueOf("body");
	
	if (! token.closed()) {
	  while (in.read(), !token.close("specification")) {
	    s = token.what();
	    
	    if (s == "body")
	      constraint = in.body("body");
	    else if (! token.closed())
	      in.finish(s);
	  }
	}
      }
      else
	in.bypass(token);
    }
  }

  return constraint;
}

QMap<Q3CString, Q3CString> UmlItem::OpaqueDefs;

bool UmlItem::FromBouml;

bool UmlItem::FromEclipse;

QMap<Q3CString, UmlItem *> UmlItem::All;

QMap<Q3CString, PFunc> UmlItem::Functions;

QMap<Q3CString, UmlTypeSpec> UmlItem::PrimitiveTypes;

QMap<Q3CString,UmlItem*> UmlItem::Incomings;

QMap<Q3CString, UmlItem*> UmlItem::Outgoings;

